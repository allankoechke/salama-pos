#include "crediteeaccountsmodel.h"

CrediteeAccountsModel::CrediteeAccountsModel(QObject *parent) : QAbstractListModel(parent)
{
    dateTime = new DateTime();
}

int CrediteeAccountsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_crediteeAccount.size();
}

QVariant CrediteeAccountsModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() > m_crediteeAccount.count())
        return QVariant();

    CrediteeAccount * creditee = m_crediteeAccount.at(index.row());

    if(role == FirstNameRole)
        return creditee->firstName();

    if(role == LastNameRole)
        return creditee->lastName();

    if(role == MobileNoRole)
        return creditee->mobileNo();

    if(role == IdNoRole)
        return creditee->idNo();

    if(role == DebtAmountRole)
        return creditee->debtAmount();

    else return QVariant();
}

bool CrediteeAccountsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    CrediteeAccount * creditee = m_crediteeAccount.at(index.row());
    bool changed = false;

    switch (role)
    {
    case FirstNameRole:
    {
        if( creditee->firstName() != value.toString())
        {
            creditee->setFirstName(value.toString());
            changed = true;
        }

        break;
    }

    case LastNameRole:
    {
        if( creditee->lastName() != value.toString())
        {
            creditee->setLastName(value.toString());
            changed = true;
        }

        break;
    }

    case IdNoRole:
    {
        if( creditee->idNo() != value.toString())
        {
            creditee->setIdNo(value.toString());
            changed = true;
        }

        break;
    }

    case MobileNoRole:
    {
        if( creditee->mobileNo() != value.toString())
        {
            creditee->setMobileNo(value.toString());
            changed = true;
        }

        break;
    }

    case DebtAmountRole:
    {
        if( creditee->debtAmount() != value.toString().toInt())
        {
            creditee->setDebtAmount(value.toString().toInt());
            changed = true;
        }

        break;
    }

    }

    if(changed)
    {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }

    return false;
}

Qt::ItemFlags CrediteeAccountsModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> CrediteeAccountsModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[FirstNameRole] = "c_fname";
    roles[LastNameRole] = "c_lname";
    roles[MobileNoRole] = "c_mobile";
    roles[IdNoRole] = "c_idNo";
    roles[DebtAmountRole] = "c_debt";

    return roles;
}

void CrediteeAccountsModel::addNewCreditee(const QString &fname, const QString &lname, const QString &mobile, const QString &idNo)
{
    emit logDataChanged("INFO", "Starting CrediteeAccountsModel::addNewCreditee()");

    QSqlDatabase db = QSqlDatabase::database();

    if(getIndex(idNo) != -1)
    {
        emit idExists();

        emit logDataChanged("WARNING", "User ID No. exists");
    }

    else
        if(db.isOpen())
        {
            QString dateToday = dateTime->getTimestamp("now").at(0);
            QString sql = "INSERT INTO creditee(firstname,lastname,national_id,phone_no,amount_due,date_added) VALUES(:fname, :lname, :idNo, :mobile, 0, :dateToday)";
            QSqlQuery query;
            query.prepare(sql);
            query.bindValue(":fname", fname);
            query.bindValue(":lname", lname);
            query.bindValue(":idNo", idNo);
            query.bindValue(":mobile", mobile);
            query.bindValue(":dateToday", dateToday);

            if(query.exec())
            {
                db.commit();

                // qDebug() << " [INFO] Creditee user added to database";

                emit logDataChanged("INFO", "Creditee user added to database");

                addNewCreditee(new CrediteeAccount(fname, lname, mobile, idNo, 0));

                emit crediteeAdded(true);
            }

            else
            {
                db.rollback();

                auto errStr = " [ERROR] Could't execute SQL (" + query.executedQuery() + ") : " + query.lastError().text();

                // qDebug() << errStr;

                emit logDataChanged("CRITICAL", errStr);

                emit crediteeAdded(false);
            }
        }

        else
        {
            // qDebug() << " [ERROR] Database not open!";

            emit crediteeAdded(false);
        }

    emit logDataChanged("INFO", "Ending CrediteeAccountsModel::addNewCreditee()");
}

void CrediteeAccountsModel::updateCreditee(const QString &fname, const QString &lname, const QString &mobile, const QString &idNo, const QString &orig_id)
{
    emit logDataChanged("INFO", "Starting CrediteeAccountsModel::updateCreditee()");

    QSqlDatabase db = QSqlDatabase::database();

    if(idNo != orig_id && getIndex(idNo) != -1)
        emit idExists();

    else
        if(db.isOpen())
        {
            QString sql = "UPDATE creditee set firstname=:fname,lastname=:lname,national_id=:idNo,phone_no=:mobile WHERE national_id=:orig_id";
            QSqlQuery query;

            query.prepare(sql);
            query.bindValue(":fname", fname);
            query.bindValue(":lname", lname);
            query.bindValue(":idNo", idNo);
            query.bindValue(":mobile", mobile);
            query.bindValue(":orig_id", orig_id);

            if(query.exec())
            {
                db.commit();

                // qDebug() << " [INFO] Creditee user updated in the database";

                emit logDataChanged("INFO", "Creditee user updated in the database");

                int index = getIndex(orig_id);

                setData(this->index(index), QVariant::fromValue(fname), FirstNameRole);
                setData(this->index(index), QVariant::fromValue(lname), LastNameRole);
                setData(this->index(index), QVariant::fromValue(mobile), MobileNoRole);
                setData(this->index(index), QVariant::fromValue(idNo), IdNoRole);

                emit crediteeUpdated(true);
            }

            else
            {
                db.rollback();

                auto errStr = " [ERROR] Could't execute SQL (" + query.executedQuery() + ") : " + query.lastError().text();

                // qDebug() << errStr;

                emit crediteeUpdated(false);

                emit logDataChanged("CRITICAL", errStr);
            }
        }

        else
        {
            // qDebug() << " [ERROR] Database not open!";

            emit crediteeUpdated(false);
        }

    emit logDataChanged("INFO", "Ending CrediteeAccountsModel::updateCreditee()");
}

void CrediteeAccountsModel::loadCrediteeAccounts()
{
    emit logDataChanged("INFO", "Starting CrediteeAccountsModel::loadCrediteeAccounts()");

    QSqlDatabase db = QSqlDatabase::database();

    if(m_crediteeAccount.size() != 0)
    {
        for(int i=m_crediteeAccount.size()-1; i>=0; i--)
        {
            removeCreditee(i);
        }
    }

    if(db.isOpen())
    {
        // QString dateToday = dateTime->getTimestamp("now").at(0);
        QString sql = "SELECT firstname,lastname,national_id,phone_no,amount_due FROM creditee";
        QSqlQuery query;

        query.prepare(sql);

        if(query.exec())
        {

            while (query.next()) {
                QString fname = query.value(0).toString();
                QString lname = query.value(1).toString();
                QString idNo = query.value(2).toString();
                QString mobile = query.value(3).toString();
                int due = query.value(4).toInt();

                addNewCreditee(new CrediteeAccount(fname, lname, mobile, idNo, due));
            }

            // qDebug() << " [INFO] Creditee user Accounts loaded";

            emit logDataChanged("INFO", "Creditee user Accounts loaded");
        }

        else
        {
            QString errStr = " [ERROR] Could't execute SQL (" + query.executedQuery() + ") : " + query.lastError().text();

            // qDebug() << errStr;

            emit logDataChanged("CRITICAL", errStr);
        }
    }

    else
    {
        qDebug() << " [ERROR] Database not open!";
    }

    emit logDataChanged("INFO", "Ending CrediteeAccountsModel::loadCrediteeAccounts()");
}

void CrediteeAccountsModel::getPaymentHistory(const QString &idNo)
{
    emit logDataChanged("INFO", "Starting CrediteeAccountsModel::getPaymentHistory()");

    QSqlDatabase db = QSqlDatabase::database();

    if(db.isOpen())
    {
        QString sql = "SELECT payment_timestamp,payment_amount ,payment_due FROM credit_payments WHERE creditee_id = :idNo";
        QSqlQuery query;

        query.prepare(sql);
        query.bindValue(":idNo", idNo);

        if(query.exec())
        {

            while (query.next()) {
                QString date = query.value(0).toString();
                int paid = query.value(1).toInt();
                int due = query.value(2).toInt();

                QDateTime dt = QDateTime::fromString(date, "yyyy-MM-ddThh:mm:ss.zzz");

                emit paymentReceived(dt.toString(" dd MMM, yyyy hh:mm AP"), paid, due);
            }

            // qDebug() << " [INFO] Creditee user Accounts loaded";

            emit logDataChanged("INFO", "Creditee user Accounts loaded");
        }

        else
        {
            QString errStr = " [ERROR] Could't execute SQL (" + query.executedQuery() + ") : " + query.lastError().text();

            // qDebug() << errStr;

            emit logDataChanged("CRITICAL", errStr);
        }
    }

    else
    {
        qDebug() << " [ERROR] Database not open!";
    }

    emit logDataChanged("INFO", "Ending CrediteeAccountsModel::getPaymentHistory()");
}

bool CrediteeAccountsModel::repayDebt(const QString &crediteeId, const int &debt, const int &debtPaid)
{
    emit logDataChanged("INFO", "Starting CrediteeAccountsModel::repayDebt()");

    // QSqlDatabase::database().transaction();
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();

    int due = (debt-debtPaid <= 0)? 0:debt-debtPaid;
    int paid = debtPaid > debt ? debt:debtPaid;
    QString dateToday = dateTime->getTimestamp("now").at(0);

    if(db.isOpen())
    {
        QString sql = "UPDATE creditee SET amount_due = :due WHERE national_id = :crediteeId";
        QString sql2 = "INSERT INTO credit_payments(payment_timestamp,creditee_id,payment_amount,payment_due) VALUES (:dateToday, :crediteeId, :paid, :due)";

        QSqlQuery query, query1;

        query1.prepare(sql2);
        query1.bindValue(":crediteeId", crediteeId);
        query1.bindValue(":dateToday", dateToday);
        query1.bindValue(":paid", paid);
        query1.bindValue(":due", due);

        query.prepare(sql);
        query.bindValue(":crediteeId", crediteeId);
        query.bindValue(":due", due);

        if(query.exec() && query1.exec())
        {
            db.commit();

            int i = getIndex(crediteeId);
            if(i != -1)
                setData(QModelIndex(this->index(i)), due, DebtAmountRole);

            else
                loadCrediteeAccounts();

            emit giveBalanceChanged(debtPaid - debt);

            // qDebug() << " [INFO] Creditee debt repayment successful";

            emit logDataChanged("INFO", "Creditee debt repayment successful");

            emit debRepaymentChanged(true);

            return true;
        }

        else
        {
            db.rollback();

            QString errStr;

            if(query.lastError().text() != "")
            {
                errStr = " [ERROR] Could't execute SQL (" + query.executedQuery() + ") : " + query.lastError().text();
                emit logDataChanged("CRITICAL", errStr);
            }

            else
            {
                errStr = " [ERROR] Could't execute SQL (" + query1.executedQuery() + ") : " + query1.lastError().text();
                emit logDataChanged("CRITICAL", errStr);
            }
        }
    }

    else
    {
        qDebug() << " [ERROR] Database not open!";
    }

    db.rollback();

    emit logDataChanged("INFO", "Ending CrediteeAccountsModel::repayDebt()");

    return false;
}

void CrediteeAccountsModel::addNewCreditee(CrediteeAccount *creditee)
{
    const int index = m_crediteeAccount.size();

    beginInsertRows(QModelIndex(), index, index);
    m_crediteeAccount.append(creditee);
    endInsertRows();
}

void CrediteeAccountsModel::removeCreditee(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    m_crediteeAccount.removeAt(index);
    endRemoveRows();
}

int CrediteeAccountsModel::getIndex(QVariant id)
{
    for(int i=0; i<m_crediteeAccount.size(); i++)
    {
        QVariant _id = data(this->index(i), IdNoRole);

        if(id == _id)
            return i;
    }

    return -1;
}
