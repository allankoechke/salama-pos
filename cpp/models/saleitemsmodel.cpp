#include "saleitemsmodel.h"

saleItemsModel::saleItemsModel(QObject *parent) : QAbstractListModel(parent)
{
    m_dateTime = new DateTime();
}

int saleItemsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_saleItems.size();
}

QVariant saleItemsModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() > m_saleItems.count())
        return QVariant();

    SaleItems * sales = m_saleItems.at(index.row());

    if(role == SaleBarcodeRole)
        return sales->saleBarcode();

    if(role == SaleItemNameRole)
        return sales->saleItemName();

    if(role == SaleUnitRole)
        return sales->saleUnit();

    if(role == SaleIdRole)
        return sales->saleId();

    if(role == SaleBpRole)
        return sales->saleBp();

    if(role == SaleSpRole)
        return sales->saleSp();

    if(role == SaleQtyRole)
        return sales->saleQty();

    if(role == CostCashRole)
        return sales->costCash();

    if(role == CostMpesaRole)
        return sales->costMpesa();

    if(role == CostChequeRole)
        return sales->costCheque();

    if(role == CostCreditRole)
        return sales->costCredit();

    if(role == SaleDateRole)
        return sales->saleDate();


    else return QVariant();
}

bool saleItemsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    SaleItems * sales = m_saleItems.at(index.row());
    bool changed = false;

    switch (role)
    {
    case SaleBarcodeRole:
    {
        if( sales->saleBarcode() != value.toString())
        {
            sales->setSaleBarcode(value.toString());
            changed = true;
        }

        break;
    }

    case SaleItemNameRole:
    {
        if( sales->saleItemName() != value.toString())
        {
            sales->setSaleItemName(value.toString());
            changed = true;
        }

        break;
    }

    case SaleUnitRole:
    {
        if( sales->saleUnit() != value.toString())
        {
            sales->setSaleUnit(value.toString());
            changed = true;
        }

        break;
    }

    case SaleIdRole:
    {
        if( sales->saleId() != value.toString())
        {
            sales->setSaleId(value.toString());
            changed = true;
        }

        break;
    }

    case SaleDateRole:
    {
        if( sales->saleDate() != value.toString())
        {
            sales->setSaleDate(value.toString());
            changed = true;
        }

        break;
    }

    case SaleBpRole:
    {
        if( sales->saleBp() != value.toString().toInt())
        {
            sales->setSaleBp(value.toString().toInt());
            changed = true;
        }

        break;
    }

    case SaleSpRole:
    {
        if( sales->saleSp() != value.toString().toInt())
        {
            sales->setSaleSp(value.toString().toInt());
            changed = true;
        }

        break;
    }

    case SaleQtyRole:
    {
        if( sales->saleQty() != value.toString().toInt())
        {
            sales->setSaleQty(value.toString().toInt());
            changed = true;
        }

        break;
    }

    case CostCashRole:
    {
        if( sales->costCash() != value.toString().toInt())
        {
            sales->setCostCash(value.toString().toInt());
            changed = true;
        }

        break;
    }

    case CostMpesaRole:
    {
        if( sales->costMpesa() != value.toString().toInt())
        {
            sales->setCostMpesa(value.toString().toInt());
            changed = true;
        }

        break;
    }

    case CostChequeRole:
    {
        if( sales->costCheque() != value.toString().toInt())
        {
            sales->setCostCheque(value.toString().toInt());
            changed = true;
        }

        break;
    }

    case CostCreditRole:
    {
        if( sales->costCredit() != value.toString().toInt())
        {
            sales->setCostCredit(value.toString().toInt());
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

Qt::ItemFlags saleItemsModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> saleItemsModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[SaleBarcodeRole] = "sale_barcode";
    roles[SaleItemNameRole] = "sale_name";
    roles[SaleUnitRole] = "sale_unit";
    roles[SaleIdRole] = "sale_id";
    roles[SaleBpRole] = "sale_bp";
    roles[SaleSpRole] = "sale_sp";
    roles[SaleQtyRole] = "sale_qty";
    roles[CostCashRole] = "sale_cash";
    roles[CostMpesaRole] = "sale_mpesa";
    roles[CostChequeRole] = "sale_cheque";
    roles[CostCreditRole] = "sale_credit";
    roles[SaleDateRole] = "sale_date";

    return roles;
}

void saleItemsModel::addSaleItem(const QVariant &barcode, const int &qty, const QVariant &uname, const QVariant &saleid, const QVariant &dt)
{
    emit logDataChanged("INFO", "Starting saleItemsModel::addSaleItem()");

    Q_UNUSED(dt)

    QString dateToday = m_dateTime->getTimestamp("now").at(0);
    QSqlDatabase m_db = QSqlDatabase::database();

    if(m_db.isOpen())
    {
        QSqlQuery fetchItem;
        fetchItem.prepare("SELECT product.product_unit,product_bp,product_sp,stock_qty FROM product INNER JOIN stock ON product.barcode = stock.barcode WHERE product.barcode = :bCode");
        fetchItem.bindValue(":bCode", barcode.toString());

        if(fetchItem.exec())
        {
            while (fetchItem.next()) {
                // QString unit = fetchItem.value(0).toString();
                float bp = fetchItem.value(1).toFloat();
                float sp = fetchItem.value(2).toFloat();
                int currentStock = fetchItem.value(3).toInt();

                if(currentStock >= qty)
                {
                    QSqlQuery writeSales;
                    //writeSales.prepare("INSERT INTO sales(sales_id,barcode,sales_date,product_bp,product_sp,sale_qty,username) VALUES(:id"+saleid.toString()+",:barcode,:date,:bp,:sp,:qty,:uname)");
                    writeSales.prepare("INSERT INTO sales(sales_id,barcode,sales_date,product_bp,product_sp,sale_qty,username) VALUES(:id, :barcode, :date, :bp, :sp, :qty, :uname)");
                    writeSales.bindValue(":id", saleid.toString());
                    writeSales.bindValue(":barcode", barcode.toString());
                    writeSales.bindValue(":date", dateToday);
                    writeSales.bindValue(":bp", bp);
                    writeSales.bindValue(":sp", sp);
                    writeSales.bindValue(":qty", qty);
                    writeSales.bindValue(":uname", uname.toString());

                    if(writeSales.exec())
                    {
                        int c_qty = currentStock - qty;

                        QSqlQuery stock_query;
                        stock_query.prepare("UPDATE \"stock\" SET stock_qty=:stock_qty WHERE barcode=:barcode");
                        stock_query.bindValue(":barcode", barcode.toString());
                        stock_query.bindValue(":stock_qty", c_qty);

                        if(stock_query.exec())
                        {
                            // TODO: check on this
                            emit updateStockChanged(barcode.toString(), c_qty);
                            emit updateSalesModelChanged(barcode.toString(), qty);
                            m_db.commit();
                        }

                        else
                        {
                            m_db.rollback();
                            emit logDataChanged("CRITICAL", "Updating Stock after sale -> " + stock_query.lastError().text());
                        }

                        emit saleItemAddedChanged(true);
                    }

                    else
                    {
                        m_db.rollback();
                        emit saleItemAddedChanged(false);
                        emit logDataChanged("CRITICAL", "ERROR executing SQL: [" + writeSales.executedQuery() + "] ->" + writeSales.lastError().text());
                    }
                }
            }
        }
    }

    else
        emit logDataChanged("FATAL", "Database not open");

    emit logDataChanged("INFO", "Ending saleItemsModel::addSaleItem()");
}

void saleItemsModel::addMpesaSale(const QVariant &mpesaId, const QVariant &salesId)
{
    emit logDataChanged("INFO", "Starting saleItemsModel::addMpesaSale()");

    QSqlDatabase m_db = QSqlDatabase::database();

    QSqlQuery writeMpesa;
    writeMpesa.prepare("INSERT INTO mpesa(mpesa_id,sales_id) VALUES(:mpesa,:id)");
    writeMpesa.bindValue(":mpesa", mpesaId.toString());
    writeMpesa.bindValue(":id", salesId.toString());

    if(writeMpesa.exec())
    {
        m_db.commit();

        emit logDataChanged("INFO", "Success writing sales info to mpesa!");
    }

    else
    {
        m_db.rollback();
        emit logDataChanged("CRITICAL", "Error writing sale info to mpesa: " + writeMpesa.lastError().text());
    }

    emit logDataChanged("INFO", "Ending saleItemsModel::addMpesaSale()");
}

void saleItemsModel::addCreditSale(const QVariant &crediteeId, const QVariant &dueDate, const QVariant &salesId, const int &amount)
{
    emit logDataChanged("INFO", "Starting saleItemsModel::addCreditSale()");

    QSqlDatabase m_db = QSqlDatabase::database();
    // qDebug() << "Creditee ID: " << crediteeId << " : " << crediteeId.toString().toInt();
    QSqlQuery writeCredit, getCreditBal, setCreditBal;

    QString credBal = "SELECT amount_due FROM creditee WHERE national_id = :crediteeId";
    getCreditBal.prepare(credBal);
    getCreditBal.bindValue(":crediteeId", crediteeId.toString());

    if(getCreditBal.exec())
    {
        int bal = 0;

        while(getCreditBal.next())
        {
            bal = getCreditBal.value(0).toInt();
            break;
        }

        int newBal = bal + amount;
        setCreditBal.prepare("UPDATE creditee SET amount_due = :due WHERE national_id=:nat");
        setCreditBal.bindValue(":due", newBal);
        setCreditBal.bindValue(":nat", crediteeId.toString());

        if(setCreditBal.exec())
        {
            writeCredit.prepare("INSERT INTO credit(creditee_id,due_date,sales_id) VALUES(:creditee,:date,:id)");
            writeCredit.bindValue(":creditee", crediteeId.toString());
            writeCredit.bindValue(":date", dueDate.toString());
            writeCredit.bindValue(":id", salesId.toString());

            if(writeCredit.exec())
            {
                emit addCrediteePaymentChanged(true);

                emit logDataChanged("INFO", "Creditee Balance Updated after sale!");

                m_db.commit();
            }

            else
            {
                m_db.rollback();

                emit addCrediteePaymentChanged(false);

                emit logDataChanged("CRITICAL", "Error writing to Credit ==> " + writeCredit.lastError().text());
            }
        }

        else
        {
            m_db.rollback();

            emit logDataChanged("CRITICAL", "Failed to update Creditee Balance: " + setCreditBal.lastError().text());
        }
    }

    else
    {
        emit logDataChanged("CRITICAL", "Error fetching Creditee Balance: " + getCreditBal.lastError().text());
    }

    emit addCrediteePaymentChanged(false);

    emit logDataChanged("INFO", "Ending saleItemsModel::addCreditSale()");
}

void saleItemsModel::addPaymentSaleDetails(const QVariant &saleId, const QJsonObject &json)
{
    emit logDataChanged("INFO", "Starting saleItemsModel::addPaymentSaleDetails()");

    QSqlDatabase m_db = QSqlDatabase::database();

    QSqlQuery writePayment;
    writePayment.prepare("INSERT INTO payment(cash,mpesa,cheque,credit,sales_id) VALUES(:cash,:mpesa,:cheque,:credit,:id)");
    writePayment.bindValue(":cash", json.value("cash").toDouble());
    writePayment.bindValue(":mpesa", json.value("mpesa").toString());
    writePayment.bindValue(":cheque", json.value("cheque").toString());
    writePayment.bindValue(":credit", json.value("credit").toString());
    writePayment.bindValue(":id", saleId.toString());

    if(writePayment.exec())
    {
        m_db.commit();

        emit logDataChanged("INFO", ">> Sucess Adding to sale payments");

        emit paymentItemAdded(true);
    }

    else
    {
        m_db.rollback();

        emit paymentItemAdded(false);

        emit logDataChanged("CRITICAL", "Error executing SQL: " + writePayment.lastError().text());
    }

    emit logDataChanged("INFO", "Ending saleItemsModel::addPaymentSaleDetails()");
}

QString saleItemsModel::getUniqueSaleId()
{
    QString salesid = "AS" + QString::number(QDateTime::currentMSecsSinceEpoch());
    return salesid;
}

QString saleItemsModel::getCurrentTimeString()
{
    return QString::number(QDateTime::currentMSecsSinceEpoch());
}

QString saleItemsModel::getDayFromToday(const int &i)
{
    return m_dateTime->getDayFromToday(i);
}

QString saleItemsModel::generateMpesaId()
{
    return "MP"+QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch());
}

QVariantMap saleItemsModel::getItemSalesDetails(const QString &barcode, const QString &date)
{
    emit logDataChanged("INFO", "Starting saleItemsModel::getItemSalesDetails()");

    QSqlDatabase m_db = QSqlDatabase::database();

    QString upper = date + " 23:59:59+03";
    QString lower = date + " 00:00:00+03";

    QJsonArray arr;


    QSqlQuery query;
    QString sql = "SELECT sales_id,product_sp,sale_qty FROM \"sales\" WHERE barcode=:barcode AND sales_date BETWEEN :lower AND :upper";
    query.prepare(sql);
    query.bindValue(":barcode", barcode);
    query.bindValue(":lower", lower);
    query.bindValue(":upper", upper);

    if(query.exec())     
    {
        while(query.next())
        {
            QString salesId = query.value(0).toString();
            float sp = query.value(1).toInt();
            int qty = query.value(2).toInt();

            if( arr.size() == 0 )
            {
                QJsonObject obj;
                obj.insert("salesId", salesId);
                obj.insert("sp", sp);
                obj.insert("qty", qty);

                arr.append(obj);
            }

            else
            {
                bool found = false;

                for(int i=0; i<arr.size();i++)
                {
                    if(salesId == arr[i].toObject().value("salesId").toString())
                    {
                        QJsonObject _obj = arr[i].toObject();
                        float _sp = _obj.value("sp").toInt();
                        int _qty = _obj.value("qty").toInt();

                        found = true;

                        if(_sp == sp)
                        {
                            int nInt = _qty + qty;
                            _obj["qty"] = nInt;
                            arr[i] = _obj;
                        }

                        else
                        {
                            QJsonObject obj;
                            obj.insert("salesId", salesId);
                            obj.insert("sp", sp);
                            obj.insert("qty", qty);

                            arr.append(obj);
                        }
                    }

                    else
                    {
                        continue;
                    }
                }

                if( !found )
                {
                    QJsonObject obj;
                    obj.insert("salesId", salesId);
                    obj.insert("sp", sp);
                    obj.insert("qty", qty);

                    arr.append(obj);
                }
            }

        }

        emit logDataChanged("INFO", ">> Sucess fetching sales data");
    }

    else
    {
        emit logDataChanged("CRITICAL", "Error executing SQL: " + query.lastError().text());
    }

    emit logDataChanged("INFO", "Ending saleItemsModel::getItemSalesDetails()");

    QJsonObject json;
    json.insert("data", arr);
    return json.toVariantMap();
}

bool saleItemsModel::undoSale(const QString &barcode, const QString &salesid, const int &qty)
{
    emit logDataChanged("INFO", "Starting saleItemsModel::undoSale()");

    int soldQty = qty;

    QSqlDatabase m_db = QSqlDatabase::database();

    try {
        if(m_db.isOpen())
        {
            QSqlQuery query, query1, query2;

            // PAYMENTS TABLE
            query.prepare("SELECT cash,mpesa,cheque,credit FROM \"payment\" WHERE sales_id=:sales_id");
            query.bindValue(":sales_id", salesid);

            // SALES TABLE
            query1.prepare("SELECT id,product_sp,sale_qty FROM \"sales\" WHERE sales_id=:sales_id AND barcode=:barcode");
            query1.bindValue(":sales_id", salesid);
            query1.bindValue(":barcode", barcode);

            // STOCK TABLE
            query2.prepare("SELECT stock_qty FROM \"stock\" WHERE barcode=:barcode");
            query2.bindValue(":barcode", barcode);

            if(query.exec() && query1.exec() && query2.exec())
            {
                bool found = false;
                bool executedSuccessful = true;

                while(query.next())
                {
                    found = true;

                    float cash = query.value(0).toInt();
                    float mpesa = query.value(1).toInt();
                    // float cheque = query.value(2).toInt();
                    // float credit = query.value(3).toInt();
                    float subT = cash+mpesa;

                    bool found1 = false;

                    while (query1.next())
                    {
                        found1 = true;
                        int id = query1.value(0).toInt();
                        float sp = query1.value(1).toInt();
                        int s_qty = query1.value(2).toInt();
                        int n_qty, n_c, n_m, n_subT, processedQty=0;

                        QString sqlA="", sqlB="", sqlC="";
                        QSqlQuery qA, qB, qC;

                        if(soldQty < s_qty)
                        {
                            qDebug() << "Quantity less than";
                            n_subT = soldQty * sp;
                            n_qty = s_qty - soldQty;
                            processedQty = soldQty;

                            if( subT >= n_subT)
                            {
                                if(cash >= n_subT)
                                {
                                    n_c = cash-n_subT;
                                    n_subT -= n_subT;
                                    n_m = mpesa;
                                }

                                else
                                {
                                    n_c = 0;
                                    n_subT -= cash;
                                    n_m = mpesa-n_subT;
                                }

                                sqlA = "UPDATE \"sales\" SET sale_qty=:sale_qty WHERE  id=:id";
                                qA.prepare(sqlA);
                                qA.bindValue(":sale_qty", n_qty);
                                qA.bindValue(":id", id);

                                sqlB = "UPDATE \"payment\" SET cash=:cash, mpesa=:mpesa WHERE  sales_id=:sales_id";
                                qB.prepare(sqlB);
                                qB.bindValue(":cash", n_c);
                                qB.bindValue(":mpesa", n_m);
                                qB.bindValue(":sales_id", salesid);

                                if(mpesa>0 && n_m == 0)
                                {
                                    sqlC = "DELETE FROM \"mpesa\" WHERE sales_id=:sales_id";
                                    qC.prepare(sqlC);
                                    qC.bindValue(":sales_id", salesid);
                                }
                            }

                            else
                            {
                                // The subtotals are not enough for the task
                                break;
                            }
                        }

                        else if(soldQty == s_qty)
                        {
                            qDebug() << "Quantity is equal";
                            n_subT = soldQty * sp;
                            processedQty = soldQty;
                            n_qty = 0;

                            if( subT==n_subT )
                            {
                                sqlA = "DELETE FROM \"sales\" WHERE  id=:id";
                                qA.prepare(sqlA);
                                qA.bindValue(":id", id);

                                sqlB = "DELETE FROM \"payment\" WHERE  sales_id=:sales_id";
                                qB.prepare(sqlB);
                                qB.bindValue(":sales_id", salesid);

                                if(mpesa>0)
                                {
                                    sqlC = "DELETE FROM \"mpesa\" WHERE sales_id=:sales_id";
                                    qC.prepare(sqlC);
                                    qC.bindValue(":sales_id", salesid);
                                }
                            }

                            else
                            {
                                if( subT >= n_subT)
                                {
                                    if(cash >= n_subT)
                                    {
                                        n_c = cash-n_subT;
                                        n_subT -= n_subT;
                                        n_m = mpesa;
                                    }

                                    else
                                    {
                                        n_c = 0;
                                        n_subT -= cash;
                                        n_m = mpesa-n_subT;
                                    }

                                    sqlA = "UPDATE \"sales\" SET sale_qty=:sale_qty WHERE  id=:id";
                                    qA.prepare(sqlA);
                                    qA.bindValue(":sale_qty", n_qty);
                                    qA.bindValue(":id", id);

                                    sqlB = "UPDATE \"payment\" SET cash=:cash, mpesa=:mpesa WHERE  sales_id=:sales_id";
                                    qB.prepare(sqlB);
                                    qB.bindValue(":cash", n_c);
                                    qB.bindValue(":mpesa", n_m);
                                    qB.bindValue(":sales_id", salesid);

                                    if(mpesa>0 && n_m == 0)
                                    {
                                        sqlC = "DELETE FROM \"mpesa\" WHERE sales_id=:sales_id";
                                        qC.prepare(sqlC);
                                        qC.bindValue(":sales_id", salesid);
                                    }
                                }

                                else
                                {
                                    // The subtotals are not enough for the task
                                    break;
                                }
                            }

                        }

                        else
                        {
                            qDebug() << "Quantity is greater";
                        }

                        if(qA.exec())
                        {
                            qDebug() << "qA executed successfully!";
                            executedSuccessful = executedSuccessful && true;
                        }

                        else
                        {
                            executedSuccessful = executedSuccessful && false;
                            qDebug() << "-> qA = " << sqlA;
                            qDebug() << "[qA] Error executing SQL:: " << qA.executedQuery() << "\t" << qA.lastError();
                        }

                        if(qB.exec())
                        {
                            qDebug() << "qB executed successfully!";
                            executedSuccessful = executedSuccessful && true;
                        }

                        else
                        {
                            executedSuccessful = executedSuccessful && false;
                            qDebug() << "-> qA = " << sqlB;
                            qDebug() << "[qB] Error executing SQL:: " << qB.executedQuery() << "\t" << qB.lastError();
                        }

                        if(sqlC !="")
                        {
                            if(qC.exec())
                            {
                                qDebug() << "qC executed successfully!";
                                executedSuccessful = executedSuccessful && true;
                            }

                            else
                            {
                                executedSuccessful = executedSuccessful && false;
                                qDebug() << "-> qA = " << sqlC;
                                qDebug() << "[qC] Error executing SQL:: " << qC.executedQuery() << "\t" << qC.lastError();
                            }
                        }

                        if(!executedSuccessful)
                            break;

                        soldQty -= processedQty;

                        if(soldQty==0)
                            break;
                    }

                    if( !found1 )
                    {
                        qDebug() << "Didn't find any sale items.";
                        emit emitError("warning", "Couldn't find any matching items!");
                    }

                    if( executedSuccessful )
                    {
                        if(soldQty==0)
                        {
                            while(query2.next())
                            {
                                int stock_qty = query2.value(0).toInt();
                                int n_stock_qty = stock_qty + qty;

                                // qDebug() << query2.value(0).toInt() << "\t" << query2.value(0).toString();
                                // qDebug() << "Stock: " << stock_qty << "\tSold Qty: " << qty << "\tNew Stock: " << n_stock_qty;

                                QSqlQuery qA;
                                qA.prepare("UPDATE \"stock\" SET stock_qty=:stock_qty WHERE barcode=:barcode");
                                qA.bindValue(":stock_qty", n_stock_qty);
                                qA.bindValue(":barcode", barcode);

                                if(qA.exec())
                                {
                                    qDebug() << "Upadating stock executed successfully!";

                                    m_db.commit();
                                    qDebug() << "\t->Committed to database";
                                    emit emitError("info", "All changes saved!");
                                    return true;
                                }

                                else
                                {
                                    qDebug() << "[Upadating stock] Error executing SQL:: " << qA.executedQuery() << "\t" << qA.lastError();


                                    m_db.rollback();
                                    qDebug() << "\t->Rolling back database";
                                    return false;
                                }

                                break;
                            }
                        }

                        else
                        {
                            m_db.rollback();
                            emit emitError("warning", "Unable to undo all the sale quantity, not enough found!");
                            qDebug() << "Unable to undo all qty, arboting";
                            return false;
                        }
                    }

                    else
                    {
                        m_db.rollback();
                        qDebug() << "\t->Database Rollback!";
                    }

                    break;
                }

                if(!found)
                {
                    qDebug() << "No data found in the payment section!";
                }

            }

            else
            {
                qDebug() << "[MAIN] Error executing SQL: " << query.lastError() << "\t: " << query.executedQuery();
            }
        }

        else
            emit logDataChanged("FATAL", "Database not open");

    }  catch (const std::exception &e) {
        qDebug() << "Error encountered: " << e.what();
        emit emitError("warning", "Failed to process request!");
        return false;
    }

    emit logDataChanged("INFO", "Ending saleItemsModel::undoSale()");
    return false;
}
