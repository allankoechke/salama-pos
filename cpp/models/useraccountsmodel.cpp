#include "useraccountsmodel.h"
#include "logger.h"

#include <memory>

UserAccountsModel::UserAccountsModel(QObject *parent) : QAbstractListModel(parent)
{

    QFile file(":/json/logged_user.json");
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    QString jsonString = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
    m_loggedInUser = doc.object();

    QFile status_file(":/json/status.json");
    status_file.open(QIODevice::ReadOnly|QIODevice::Text);
    QString jsonStatus = status_file.readAll();
    status_file.close();

    QJsonDocument docStatus = QJsonDocument::fromJson(jsonStatus.toUtf8());
    m_status = docStatus.object();

    m_dateTime = new DateTime();
}

int UserAccountsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return mUserAccounts.size();
}

QVariant UserAccountsModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() > mUserAccounts.count())
        return QVariant();

    UserAccounts * accounts = mUserAccounts.at(index.row());

    // shared/unique ptr

    if(role == UserFirstnameRole)
        return accounts->userFirstname();

    if(role == UserLastnameRole)
        return accounts->userLastname();

    if(role == UserUsernameRole)
        return accounts->userUsername();

    if(role == UserPhoneNoRole)
        return accounts->userPhoneNo();

    if(role == UserPasswordRole)
        return accounts->userPassword();

    if(role == UserDateAddedRole)
        return accounts->userDateAdded();

    if(role == CanAddUsersRole)
        return accounts->canAddUsers();

    if(role == CanRemoveUsersRole)
        return accounts->canRemoveUsers();

    if(role == CanAddItemsRole)
        return accounts->canAddItems();

    if(role == CanRemoveItemsRole)
        return accounts->canRemoveItems();

    if(role == CanAddStockRole)
        return accounts->canAddStock();

    if(role == CanRemoveStockRole)
        return accounts->canRemoveStock();

    if(role == CanUndoSalesRole)
        return accounts->canUndoSales();

    if(role == CanBackupDbRole)
        return accounts->canBackupDb();

    if(role == ChangePasswordRole)
        return accounts->changePassword();

    if(role == RolesStringRole)
        return accounts->rolesString();

    else return QVariant();
}

bool UserAccountsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    UserAccounts * accounts = mUserAccounts.at(index.row());
    bool changed = false;

    switch (role)
    {
    case UserFirstnameRole:
    {
        if( accounts->userFirstname() != value.toString())
        {
            accounts->setUserFirstname(value.toString());
            changed = true;
        }

        break;
    }

    case UserLastnameRole:
    {
        if( accounts->userLastname() != value.toString())
        {
            accounts->setUserLastname(value.toString());
            changed = true;
        }

        break;
    }

    case UserUsernameRole:
    {
        if( accounts->userUsername() != value.toString())
        {
            accounts->setUserUsername(value.toString());
            changed = true;
        }

        break;
    }

    case UserPhoneNoRole:
    {
        if( accounts->userPhoneNo() != value.toString())
        {
            accounts->setUserPhoneNo(value.toString());
            changed = true;
        }

        break;
    }

    case UserPasswordRole:
    {
        if( accounts->userPassword() != value.toString())
        {
            accounts->setUserPassword(value.toString());
            changed = true;
        }

        break;
    }


    case UserDateAddedRole:
    {
        if( accounts->userDateAdded() != value.toString())
        {
            accounts->setUserDateAdded(value.toString());
            changed = true;
        }

        break;
    }

    case CanAddUsersRole:
    {
        if( accounts->canAddUsers() != value.toBool())
        {
            accounts->setCanAddUsers(value.toBool());
            changed = true;
        }

        break;
    }

    case CanRemoveUsersRole:
    {
        if( accounts->canRemoveUsers() != value.toBool())
        {
            accounts->setCanRemoveUsers(value.toBool());
            changed = true;
        }

        break;
    }

    case CanAddItemsRole:
    {
        if( accounts->canAddItems() != value.toBool())
        {
            accounts->setCanAddItems(value.toBool());
            changed = true;
        }

        break;
    }

    case CanRemoveItemsRole:
    {
        if( accounts->canRemoveItems() != value.toBool())
        {
            accounts->setCanRemoveItems(value.toBool());
            changed = true;
        }

        break;
    }

    case CanAddStockRole:
    {
        if( accounts->canAddStock() != value.toBool())
        {
            accounts->setCanAddStock(value.toBool());
            changed = true;
        }

        break;
    }

    case CanRemoveStockRole:
    {
        if( accounts->canRemoveStock() != value.toBool())
        {
            accounts->setCanRemoveStock(value.toBool());
            changed = true;
        }

        break;
    }

    case CanUndoSalesRole:
    {
        if( accounts->canUndoSales() != value.toBool())
        {
            accounts->setCanUndoSales(value.toBool());
            changed = true;
        }

        break;
    }

    case CanBackupDbRole:
    {
        if( accounts->canBackupDb() != value.toBool())
        {
            accounts->setCanBackupDb(value.toBool());
            changed = true;
        }

        break;
    }

    case ChangePasswordRole:
    {
        if( accounts->changePassword() != value.toBool())
        {
            accounts->setChangePassword(value.toBool());
            changed = true;
        }

        break;
    }

    case RolesStringRole:
    {
        if( accounts->rolesString() != value.toString())
        {
            accounts->setRolesString(value.toString());
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

Qt::ItemFlags UserAccountsModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> UserAccountsModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[UserFirstnameRole] = "user_fname";
    roles[UserLastnameRole] = "user_lname";
    roles[UserUsernameRole] = "user_username";
    roles[UserPhoneNoRole] = "user_phoneNo";
    roles[UserPasswordRole] = "user_password";
    roles[UserDateAddedRole] = "date_added";
    roles[CanAddUsersRole] = "can_addUsers";
    roles[CanRemoveUsersRole] = "can_removeUsers";
    roles[CanAddItemsRole] = "can_addProducts";
    roles[CanRemoveItemsRole] = "can_removeProducts";
    roles[CanAddStockRole] = "can_addStock";
    roles[CanRemoveStockRole] = "can_removeStock";
    roles[CanUndoSalesRole] = "can_undoSales";
    roles[CanBackupDbRole] = "can_backup";
    roles[ChangePasswordRole] = "to_changePassword";
    roles[RolesStringRole] = "user_roles";

    return roles;
}

void UserAccountsModel::addNewUserAccount(const QVariant &userFirstname, const QVariant &userLastname, const QVariant &userUsername, const QVariant &userPassword, const QVariant &userPhoneNo, const QVariant &userDateAdded)
{
    emit logDataChanged("INFO", "Starting AddNewUserAccount()");

    Q_UNUSED(userDateAdded)

    if(getUserIndex(userUsername.toString()) != -1)
    {
        emit usernameExistsChanged(true);

        // qDebug() << ">> Duplicate username ...";

        emit logDataChanged("WARNING", "Username entered exists in the database");
    }

    else
    {
        QString dateToday = m_dateTime->getTimestamp("now").at(0);
        QSqlDatabase m_db = QSqlDatabase::database();

        QString password = hashPassword(userPassword.toString());

        if(m_db.isOpen())
        {
            QSqlQuery query;
            query.prepare("INSERT INTO \"users\"(firstname,lastname,username,password,phone_no,date_added) VALUES (:firstname,:lastname,:username,:password,:phone_no,:date_added)");
            query.bindValue(":firstname", userFirstname.toString());
            query.bindValue(":lastname", userLastname.toString());
            query.bindValue(":username", userUsername.toString());
            query.bindValue(":password", password);
            query.bindValue(":phone_no", userPhoneNo.toString());
            query.bindValue(":date_added", dateToday);

            QSqlQuery priviledges_query;
            priviledges_query.prepare("INSERT INTO \"priviledges\"(username) VALUES (:username)");
            priviledges_query.bindValue(":username", userUsername.toString());

            if(query.exec() && priviledges_query.exec())
            {
                m_db.commit();

                // qDebug() << ">> New User Added";

                addNewUserAccount(new UserAccounts(userFirstname.toString(), userLastname.toString(), userUsername.toString(), userPhoneNo.toString(), password, dateToday, true, false, true, false, true, false, false, false, false, getUserRoleAsAString(true, false, true, false, true, false, false, false)));

                emit userAddedChanged(true);

                emit logDataChanged("INFO", "New User Account Added Successfully");

            }

            else
            {
                m_db.rollback();

                emit userAddedChanged(false);

                QString errorStr = "Error executing SQL: " + query.lastError().text() + " :: " + priviledges_query.lastError().text();

                emit logDataChanged("CRITICAL", errorStr);
            }
        }
    }

    emit logDataChanged("INFO", "Ending AddNewUserAccount()");
}

void UserAccountsModel::updatePassword(const QVariant &userUsername, const QVariant &passwordOld, const QVariant &passwordNew)
{
    emit logDataChanged("INFO", "Starting updatePassword()");

    QSqlDatabase m_db = QSqlDatabase::database();

    QString oldPassword = "";
    QString newPassword = hashPassword(passwordNew.toString());

    int index = getUserIndex(userUsername.toString());

    if(index != -1)
    {
        if(m_db.isOpen())
        {
            QSqlQuery query;
            query.prepare("SELECT password FROM \"users\" WHERE username=:username");
            query.bindValue(":username", userUsername.toString());

            if(query.exec())
            {
                if(query.size() > 0 && query.first())
                {
                    QString p = query.value(0).toString();
                    QString salt = p.split(":").size() >= 2 ? p.split(":").at(1) : "";
                    oldPassword = hashPassword(passwordOld.toString(), salt);

                    Logger::logDebug("Password change: Salt generated", salt);
                    Logger::logDebug("Password change: Old password hash", p);

                    if( p != oldPassword) {

                        emit userPasswordChanged(false);

                        emit userPasswordChangeError("Could not authenticate");

                        return;
                    }
                } else {
                    emit userPasswordChanged(false);

                    emit userPasswordChangeError("Could not authenticate");

                    return;
                }
            }

            else
            {
                emit userPasswordChanged(false);

                emit userPasswordChangeError("Could not authenticate!");

                return;
            }

            QSqlQuery query1;
            query1.prepare("UPDATE \"users\" set password=:password WHERE username=:username");
            query1.bindValue(":password", newPassword);
            query1.bindValue(":username", userUsername.toString());

            if(query1.exec())
            {
                m_db.commit();

                // qDebug() << ">> User Password Updated";

                setData(this->index(index), QVariant::fromValue(newPassword), UserPasswordRole);

                emit userPasswordChanged(true);

                emit logDataChanged("INFO", "User Password Updated");
            }

            else
            {
                m_db.rollback();

                emit userPasswordChanged(false);

                emit userPasswordChangeError("Failed to update password");

                QString errorStr = "Error executing SQL: " + query.lastError().text();

                // qDebug() << errorStr;

                emit logDataChanged("CRITICAL", errorStr);
            }
        }
    }

    else
    {
        // qDebug() << ">> User index could not be found ...";

        emit logDataChanged("CRITICAL", "User index could not be found");
    }

    emit logDataChanged("INFO", "Ending updatePassword()");

}

void UserAccountsModel::removeUserAccount(const QVariant &userUsername, QVariant index)
{
    emit logDataChanged("INFO", "Starting removeUserAccount()");

    QSqlDatabase m_db = QSqlDatabase::database();

    if(m_db.isOpen())
    {
        QSqlQuery query;
        query.prepare("DELETE FROM \"users\" WHERE username=:username");
        query.bindValue(":username", userUsername.toString());

        if(query.exec())
        {
            m_db.commit();

            // qDebug() << ">> New User Deleted";

            emit logDataChanged("INFO", "New User Deleted");

            removeUserAccount(index.toString().toInt());

            emit userRemovedChanged(true);

        }

        else
        {
            m_db.rollback();

            emit userRemovedChanged(false);

            auto errStr = "Error executing SQL: " + query.lastError().text();

            // qDebug() << errStr;

            emit logDataChanged("CRITICAL", errStr);
        }
    }

    emit logDataChanged("INFO", "Ending removeUserAccount()");
}

void UserAccountsModel::updateUserAccount(const QVariant &userFirstname, const QVariant &userLastname, const QVariant &userUsername, const QVariant &userPhoneNo, const QVariant &orig_username)
{
    emit logDataChanged("INFO", "Starting updateUserAccount() -> Account Details");

    if(userUsername != orig_username && getUserIndex(userUsername.toString()) != -1)
        emit usernameExistsChanged(true);

    else
    {
        QSqlDatabase m_db = QSqlDatabase::database();

        int index_ = getUserIndex(userUsername.toString());

        if(index_ != -1)
            if(m_db.isOpen())
            {
                QSqlQuery query;
                query.prepare("UPDATE \"users\" SET firstname=:firstname,lastname=:lastname,username=:username,phone_no=:phone_no WHERE username=:orig_username");
                query.bindValue(":firstname", userFirstname.toString());
                query.bindValue(":lastname", userLastname.toString());
                query.bindValue(":username", userUsername.toString());
                query.bindValue(":phone_no", userPhoneNo.toString());
                query.bindValue(":orig_username", orig_username.toString());

                if(query.exec())
                {
                    m_db.commit();

                    // qDebug() << " [INFO] User Details updated";

                    emit logDataChanged("INFO","User Details updated");

                    setData(this->index(index_), userFirstname, UserFirstnameRole);
                    setData(this->index(index_), userLastname, UserLastnameRole);
                    setData(this->index(index_), userUsername, UserUsernameRole);
                    setData(this->index(index_), userPhoneNo, UserPhoneNoRole);

                    emit userUpdatedChanged(true);

                    if(m_loggedInUser.value("username").toString() == orig_username.toString())
                    {
                        m_loggedInUser["firstname"] = data(this->index(index_), UserFirstnameRole).toString();
                        m_loggedInUser["lastname"] = data(this->index(index_), UserLastnameRole).toString();
                        m_loggedInUser["username"] = data(this->index(index_), UserUsernameRole).toString();
                        m_loggedInUser["phone_no"] = data(this->index(index_), UserPhoneNoRole).toString();

                        emit logged_inUserChanged();
                    }

                }

                else
                {
                    m_db.rollback();

                    emit userUpdatedChanged(false);

                    auto errStr = "Error executing SQL: " + query.lastError().text();

                    // qDebug() << errStr;

                    emit logDataChanged("CRITICAL", errStr);
                }
            }

            else
                Logger::logWarning("User account index not found");

        else
            Logger::logWarning("User account index not found");
    }

    emit logDataChanged("INFO", "Ending updateUserAccount() -> Account Details");
}

void UserAccountsModel::updateUserAccount(const QVariant &userUsername, const bool &canAddUsers, const bool &canRemoveUsers, const bool &canAddItems, const bool &canRemoveItems, const bool &canAddStock, const bool &canRemoveStock, const bool &canUndoSales, const bool &canBackupDb)
{
    emit logDataChanged("INFO", "Starting updateUserAccount() -> Account Priviledges");

    int index_ = getUserIndex(userUsername.toString());

    if(index_ == -1)
        emit userPriviledgesChanged(false);

    else
    {
        QSqlDatabase m_db = QSqlDatabase::database();

        if(m_db.isOpen())
        {
            QSqlQuery query;
            query.prepare("UPDATE priviledges SET can_add_user=:can_add_user,can_remove_user=:can_remove_user,can_add_product=:can_add_product,can_remove_product=:can_remove_product, can_add_stock=:can_add_stock,can_remove_stock=:can_remove_stock,can_remove_sales=:can_remove_sales,can_backup=:can_backup WHERE username=:username");
            query.bindValue(":can_add_user", stringifyBool(canAddUsers));
            query.bindValue(":can_remove_user", stringifyBool(canRemoveUsers));
            query.bindValue(":can_add_product", stringifyBool(canAddItems));
            query.bindValue(":can_remove_product", stringifyBool(canRemoveItems));
            query.bindValue(":can_add_stock", stringifyBool(canAddStock));
            query.bindValue(":can_remove_stock", stringifyBool(canRemoveStock));
            query.bindValue(":can_remove_sales", stringifyBool(canUndoSales));
            query.bindValue(":can_backup", stringifyBool(canBackupDb));
            query.bindValue(":username", userUsername.toString());

            if(query.exec())
            {
                m_db.commit();

                // qDebug() << " [Info] User Priviledges updated";

                emit logDataChanged("INFO", "User Priviledges updated");

                setData(this->index(index_), canAddUsers, CanAddUsersRole);
                setData(this->index(index_), canRemoveUsers, CanRemoveUsersRole);
                setData(this->index(index_), canAddItems, CanAddItemsRole);
                setData(this->index(index_), canRemoveItems, CanRemoveItemsRole);
                setData(this->index(index_), canAddStock, CanAddStockRole);
                setData(this->index(index_), canRemoveStock, CanRemoveStockRole);
                setData(this->index(index_), canUndoSales, CanUndoSalesRole);
                setData(this->index(index_), canBackupDb, CanBackupDbRole);

                QString str = getUserRoleAsAString(canAddUsers, canRemoveUsers, canAddItems, canRemoveItems, canAddStock, canRemoveStock, canUndoSales, canBackupDb);
                setData(this->index(index_), str, RolesStringRole);

                emit userPriviledgesChanged(true);

                if(m_loggedInUser.value("username").toString() == userUsername.toString())
                {
                    QJsonDocument doc(m_loggedInUser);
                    QJsonObject docObj = doc.object();

                    docObj["canAddUser"] = data(this->index(index_), CanAddUsersRole).toBool();
                    docObj["canRemoveUsers"] = data(this->index(index_), CanRemoveUsersRole).toBool();
                    docObj["canAddItems"] = data(this->index(index_), CanAddItemsRole).toBool();
                    docObj["canRemoveItems"] = data(this->index(index_), CanRemoveItemsRole).toBool();
                    docObj["canAddStock"] = data(this->index(index_), CanAddStockRole).toBool();
                    docObj["canRemoveStock"] = data(this->index(index_), CanRemoveStockRole).toBool();
                    docObj["canUndoSales"] = data(this->index(index_), CanUndoSalesRole).toBool();
                    docObj["canBackupDb"] = data(this->index(index_), CanBackupDbRole).toBool();

                    setLoggedInUser(docObj);

                    emit logged_inUserChanged();
                }
            }

            else
            {
                m_db.rollback();

                emit userPriviledgesChanged(false);

                auto errStr = "Error executing SQL: " + query.lastError().text() + " :: " + query.executedQuery();

                // qDebug() << errStr;

                emit logDataChanged("CRITICAL", errStr);
            }
        }
    }

    emit logDataChanged("INFO", "Ending updateUserAccount() -> Account Priviledges");
}

void UserAccountsModel::markAccountForDeleting(const QVariant &userUsername)
{
    emit logDataChanged("INFO", "Starting markAccountForDeleting()");

    QSqlDatabase m_db = QSqlDatabase::database();

    int index = getUserIndex(userUsername.toString());

    if(index != -1)
    {
        if(m_db.isOpen())
        {
            QSqlQuery query;
            query.prepare("UPDATE \"users\" set to_delete_account=:to_delete_account WHERE username=:username");
            query.bindValue(":to_delete_account", true);
            query.bindValue(":username", userUsername.toString());

            if(query.exec())
            {
                m_db.commit();

                // qDebug() << ">> To delete Account Updated";

                removeUserAccount(index);

                emit toDeleteAccountChanged(true);

                emit logDataChanged("INFO", "To delete Account Updated");
            }

            else
            {
                m_db.rollback();

                emit toDeleteAccountChanged(false);

                auto errStr = "Error executing SQL: " + query.lastError().text();

                // qDebug() << errStr;

                emit logDataChanged("CRITICAL", errStr);
            }
        }
    }

    else
    {
        // qDebug() << ">> User index could not be found ...";

        emit logDataChanged("WARNING", "User index could not be found");
    }

    emit logDataChanged("INFO", "Ending markAccountForDeleting()");
}

void UserAccountsModel::loadAllUserAccounts()
{
    emit logDataChanged("INFO", "Starting loadAllUserAccounts()");

    QSqlDatabase m_db = QSqlDatabase::database();

    if(m_db.isOpen())
    {
        QSqlQuery query;

        const QString sql = "SELECT firstname,lastname,users.username,password,phone_no,date_added, can_add_user, can_remove_user, can_add_product,\
            can_remove_product, can_add_stock, can_remove_stock, can_remove_sales, can_backup, to_change_password FROM \"users\" INNER \
            JOIN \"priviledges\" ON users.username = priviledges.username";


        if(query.exec(sql))
        {
            while(query.next())
            {
                QString userFirstName = query.value(0).toString();
                QString userLastname = query.value(1).toString();
                QString userUsername = query.value(2).toString();
                QString password = query.value(3).toString();
                QString userPhoneNo = query.value(4).toString();
                QString userDateAdded = query.value(5).toString();
                bool canAddUsers = query.value(6).toBool();
                bool canRemoveUsers = query.value(7).toBool();
                bool canAddItems = query.value(8).toBool();
                bool canRemoveItems = query.value(9).toBool();
                bool canAddStock = query.value(10).toBool();
                bool canRemoveStock = query.value(11).toBool();
                bool canUndoSales = query.value(12).toBool();
                bool canBackupDb = query.value(13).toBool();
                bool changePassword = query.value(14).toBool();

                QString role = getUserRoleAsAString(canAddUsers, canRemoveUsers, canAddItems, canRemoveItems, canAddStock, canRemoveStock, canUndoSales, canBackupDb);

                addNewUserAccount(new UserAccounts(userFirstName, userLastname, userUsername, userPhoneNo, password, userDateAdded, canAddUsers, canRemoveUsers, canAddItems, canRemoveItems, canAddStock, canRemoveStock, canUndoSales, canBackupDb,changePassword, role));

            }
            emit userAccountsLoaded(true);

        }

        else
        {
            emit userAccountsLoaded(false);

            auto errStr = "Error executing SQL: " + query.lastError().text();

            // qDebug() << errStr;

            emit logDataChanged("CRITICAL", errStr);
        }
    }

    emit logDataChanged("INFO", "Ending loadAllUserAccounts()");
}

void UserAccountsModel::loginUser(const QVariant &uname, const QVariant &pswd)
{
    emit logDataChanged("INFO", "Starting loginUser()");

    const int ind = getUserIndex(uname.toString());

    if(ind == -1)
    {
        emit loggingInUsernameStatus(false);

        emit logDataChanged("INFO", "User not in the database");
        // qDebug() << ">> User not in the database";
    }

    else
    {
        emit loggingInUsernameStatus(true);

        QString savedP = data(this->index(ind), UserPasswordRole).toString();

        if(login(savedP, pswd.toString()))
        {
            // qDebug() << " [DEBUG] Password correct!";

            emit loggingInPasswordStatus(true);

            QJsonDocument doc(m_loggedInUser);
            QJsonObject docObj = doc.object();

            docObj["firstname"] = data(this->index(ind), UserFirstnameRole).toString();
            docObj["lastname"] = data(this->index(ind), UserLastnameRole).toString();
            docObj["username"] = data(this->index(ind), UserUsernameRole).toString();
            docObj["phone_no"] = data(this->index(ind), UserPhoneNoRole).toString();
            docObj["canAddUser"] = data(this->index(ind), CanAddUsersRole).toBool();
            docObj["canRemoveUsers"] = data(this->index(ind), CanRemoveUsersRole).toBool();
            docObj["canAddItems"] = data(this->index(ind), CanAddItemsRole).toBool();
            docObj["canRemoveItems"] = data(this->index(ind), CanRemoveItemsRole).toBool();
            docObj["canAddStock"] = data(this->index(ind), CanAddStockRole).toBool();
            docObj["canRemoveStock"] = data(this->index(ind), CanRemoveStockRole).toBool();
            docObj["canUndoSales"] = data(this->index(ind), CanUndoSalesRole).toBool();
            docObj["canBackupDb"] = data(this->index(ind), CanBackupDbRole).toBool();

            setLoggedInUser(docObj);

            emit logged_inUserChanged();
        }

        else
        {
            emit loggingInPasswordStatus(false);

            // qDebug() << ">> Wrong User password";
        }
    }

    emit logDataChanged("INFO", "Ending loginUser()");
}

QJsonObject UserAccountsModel::getUpdatedPriviledges(int index)
{
    emit logDataChanged("INFO", "Starting getUpdatedPriviledges()");

    if(index < mUserAccounts.size())
    {
        m_status["canAddUser"] = data(this->index(index), CanAddUsersRole).toBool();
        m_status["canRemoveUsers"] = data(this->index(index), CanRemoveUsersRole).toBool();
        m_status["canAddItems"] = data(this->index(index), CanAddItemsRole).toBool();
        m_status["canRemoveItems"] = data(this->index(index), CanRemoveItemsRole).toBool();
        m_status["canAddStock"] = data(this->index(index), CanAddStockRole).toBool();
        m_status["canRemoveStock"] = data(this->index(index), CanRemoveStockRole).toBool();
        m_status["canUndoSales"] = data(this->index(index), CanUndoSalesRole).toBool();
        m_status["canBackupDb"] = data(this->index(index), CanBackupDbRole).toBool();

        return m_status;
    }

    // qDebug() << " [ERROR] Invalid User Account index...";

    emit logDataChanged("WARNING", "Invalid User Account index");

    emit logDataChanged("INFO", "Ending getUpdatedPriviledges()");

    return QJsonObject();
}

void UserAccountsModel::addNewUserAccount(UserAccounts *user)
{
    const int index = mUserAccounts.size();

    beginInsertRows(QModelIndex(), index, index);
    mUserAccounts.append(user);
    endInsertRows();
}

void UserAccountsModel::removeUserAccount(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    mUserAccounts.removeAt(index);
    endRemoveRows();
}

QString UserAccountsModel::hashPassword(const QString &pswd)
{
    QString salt = QString::number(QDateTime::currentSecsSinceEpoch());
    Logger::logDebug("Password hash: Salt generated", salt);
    QCryptographicHash hash(QCryptographicHash::Sha3_256);
    hash.addData(pswd.toUtf8() + salt.toUtf8());
    auto hashedPassword = hash.result().toHex()+":"+salt;
    Logger::logDebug("Password hash: Final hash generated");
    return hashedPassword;
}

QString UserAccountsModel::hashPassword(const QString &pswd, const QString &salt)
{
    QCryptographicHash hash(QCryptographicHash::Sha3_256);
    hash.addData(pswd.toUtf8() + salt.toUtf8());
    auto hashedPassword = hash.result().toHex()+":"+salt;
    Logger::logDebug("Password hash: Final hash generated");
    return hashedPassword;
}

bool UserAccountsModel::login(const QString &savedPswd, const QString &inputPswd)
{
    try {
        QString _pswd = savedPswd.split(":").at(0);
        QString _salt = savedPswd.split(":").at(1);

        QCryptographicHash hash(QCryptographicHash::Sha3_256);
        hash.addData(inputPswd.toUtf8() + _salt.toUtf8());

        if(hash.result().toHex() == _pswd)
            return true;

        else
            return false;
    } catch(char e) {
        return false;
    }
}

int UserAccountsModel::getUserIndex(const QString &username)
{
    QString _username;

    for(int i=0; i<mUserAccounts.size(); i++)
    {
        _username = data(this->index(i), UserUsernameRole).toString();

        if(username == _username)
            return i;
    }

    return -1;
}

QString UserAccountsModel::stringifyBool(const bool &state)
{
    if(state)
        return "true";

    return "false";
}

QString UserAccountsModel::getUserRoleAsAString(const bool &canAddUsers, const bool &canRemoveUsers, const bool &canAddItems, const bool &canRemoveItems, const bool &canAddStock, const bool &canRemoveStock, const bool &canUndoSales, const bool &canBackupDb)
{
    QString _string;

    _string += canAddUsers? "Can create accounts, ":"";
    _string += canRemoveUsers? "Can delete accounts, ":"";
    _string += canAddItems? "Can add products, ":"";
    _string += canRemoveItems? "Can delete products, ":"";
    _string += canAddStock? "Can add stock, ":"";
    _string += canRemoveStock? "Can delete stock, ":"";
    _string += canUndoSales? "Can delete sales, ":"";
    _string += canBackupDb? "Can backup database": "";

    if(_string.toStdString()[_string.length()-2] == ',')
        _string = _string.left(_string.length()-2);

    if(_string == "")
        _string = "---";

    return _string;
}

QJsonObject UserAccountsModel::loggedInUser() const
{
    return m_loggedInUser;
}

void UserAccountsModel::setLoggedInUser(QJsonObject loggedInUser)
{
    if (m_loggedInUser == loggedInUser)
        return;

    m_loggedInUser = loggedInUser;
    emit loggedInUserChanged(m_loggedInUser);
}
