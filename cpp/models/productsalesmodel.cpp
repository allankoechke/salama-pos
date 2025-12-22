#include "productsalesmodel.h"

ProductSalesModel::ProductSalesModel(QObject *parent) : QAbstractListModel(parent)
{
    QJsonDocument doc = QJsonDocument::fromVariant("{ \"cash\": 0, \"mpesa\": 0, \"cheque\": 0, \"credit\": 0 }");
    m_json = doc.object();

    m_dateTime = new DateTime();
}

int ProductSalesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_productSales.size();
}

QVariant ProductSalesModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() > m_productSales.count())
        return QVariant();

    ProductSales * sales = m_productSales.at(index.row());

    if(role == ProductSaleIdRole)
        return sales->productSaleId();

    if(role == ProductBarcodeRole)
        return sales->productBarcode();

    if(role == ProductNameRole)
        return sales->productName();

    if(role == ProductUnitRole)
        return sales->productUnit();

    if(role == ProductQtyRole)
        return sales->productQty();

    if(role == ProductBpRole)
        return sales->productBp();

    if(role == ProductSpRole)
        return sales->productSp();

    if(role == ProductPaymentRole)
        return sales->productPayment();

    else return QVariant();
}

bool ProductSalesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    ProductSales * sales = m_productSales.at(index.row());
    bool changed = false;

    switch (role)
    {
    case ProductSaleIdRole:
    {
        if( sales->productSaleId() != value.toString())
        {
            sales->setProductSaleId(value.toString());
            changed = true;
        }

        break;
    }

    case ProductBarcodeRole:
    {
        if( sales->productBarcode() != value.toString())
        {
            sales->setProductBarcode(value.toString());
            changed = true;
        }

        break;
    }

    case ProductNameRole:
    {
        if( sales->productName() != value.toString())
        {
            sales->setProductName(value.toString());
            changed = true;
        }

        break;
    }

    case ProductUnitRole:
    {
        if( sales->productUnit() != value.toString())
        {
            sales->setProductUnit(value.toString());
            changed = true;
        }

        break;
    }

    case ProductQtyRole:
    {
        if( sales->productQty() != value.toString())
        {
            sales->setProductQty(value.toString().toInt());
            changed = true;
        }

        break;
    }

    case ProductBpRole:
    {
        if( sales->productBp() != value.toString())
        {
            sales->setProductBp(value.toString().toInt());
            changed = true;
        }

        break;
    }

    case ProductSpRole:
    {
        if( sales->productSp() != value.toString())
        {
            sales->setProductSp(value.toString().toInt());
            changed = true;
        }

        break;
    }

    case ProductPaymentRole:
    {
        if( sales->productPayment() != QJsonDocument::fromVariant(value).object())
        {
            sales->setProductPayment(QJsonDocument::fromVariant(value).object());
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

Qt::ItemFlags ProductSalesModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> ProductSalesModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[ProductSaleIdRole] = "p_saleId";
    roles[ProductBarcodeRole] = "p_barcode";
    roles[ProductNameRole] = "p_name";
    roles[ProductUnitRole] = "p_unit";
    roles[ProductQtyRole] = "p_qty";
    roles[ProductBpRole] = "p_bp";
    roles[ProductSpRole] = "p_sp";
    roles[ProductPaymentRole] = "p_paymentsJson";

    return roles;
}

void ProductSalesModel::loadSalesData()
{
    emit logDataChanged("INFO", "Starting ProductSalesModel::loadSalesData()");

    // qDebug() << " [INFO] Starting fetch of the day's sales data ...";

    QSqlDatabase mdb = QSqlDatabase::database();

    if(!mdb.isOpen())
        qDebug() << "[ERROR] Database not open/ready yet!";

    else
    {
        QString sql = "SELECT sales.sales_id,barcode,sales_date,product_bp,product_sp,sale_qty,cash,mpesa,cheque,credit FROM sales INNER JOIN payment ON sales.sales_id = payment.sales_id";
        QSqlQuery query, itemQuery;
        // QJsonDocument doc = QJsonDocument::fromVariant("{ \"cash\": 0, \"mpesa\": 0, \"cheque\": 0, \"credit\": 0 }");

        if(query.exec(sql))
        {
            // qDebug() << " [INFO] Successfully loaded todays sales ..";

            emit logDataChanged("INFO", "Successfully loaded todays sales");

            while(query.next())
            {
                QString sales_id = query.value(0).toString();
                QString barcode = query.value(1).toString();
                // QString sales_date = query.value(2).toString();
                int product_bp = query.value(3).toInt();
                int product_sp = query.value(4).toInt();
                int sale_qty = query.value(5).toInt();
                int cash = query.value(6).toInt();
                int mpesa = query.value(7).toInt();
                int cheque = query.value(8).toInt();
                int credit = query.value(9).toInt();

                int _ind = getSaleItemIndex(barcode);

                if(_ind == -1)
                {
                    m_json["cash"] = cash;
                    m_json["mpesa"] = mpesa;
                    m_json["cheque"] = cheque;
                    m_json["credit"] = credit;

                    itemQuery.prepare("SELECT product_name,product_unit FROM product WHERE barcode=:barcode");
                    itemQuery.bindValue(":barcode", barcode);

                    if(itemQuery.exec())
                    {
                        while (itemQuery.next()) {
                            QString name = itemQuery.value(0).toString();
                            QString unit = itemQuery.value(1).toString();

                            // qDebug() << "Name: " << name << "\tUnit: " << unit;

                            addSalesData(new ProductSales(sales_id,barcode,name,unit,sale_qty,product_bp,product_sp, m_json));
                            // qDebug() << m_productSales;
                        }
                    }

                    else
                    {
                        QString errStr = "[ERROR] Couldn't fetch item data from products : " + itemQuery.lastError().text();

                        // qDebug() << errStr;

                        emit logDataChanged("CRITICAL", errStr);
                    }
                }

                else
                {
                    // Update the current existing item details
                    int n_qty = sale_qty + data(this->index(_ind), ProductQtyRole).toInt();
                    setData(this->index(_ind), n_qty, ProductQtyRole);

                    // qDebug() << " [INFO] Item exist, updating ...";

                    emit logDataChanged("INFO", "Item exist in cart, updating");
                }
            }
        }

        else
        {
            QString errStr = "[ERROR] Could'nt load todays sale data! > " + query.lastError().text();

            // qDebug() << errStr;

            emit logDataChanged("CRITICAL", errStr);
        }
    }

    // qDebug() << " [INFO] Ending fetch of the day's sales data ...";

    emit logDataChanged("INFO", "Ending ProductSalesModel::loadSalesData()");
}

void ProductSalesModel::addSalesData(const QVariant &barcode, const int &qty, const QJsonObject &cost)
{
    emit logDataChanged("INFO", "Starting ProductSalesModel::addSalesData()");

    Q_UNUSED(cost)

    int index = getSaleItemIndex(barcode.toString());

    if(index != -1)
    {
        // Update the current existing item details
        int n_qty = qty + data(this->index(index), ProductQtyRole).toInt();
        setData(this->index(index), n_qty, ProductQtyRole);

        // qDebug() << " [INFO] Item <" << barcode <<"> exists, updating ...";
    } else {
        // qDebug() << " [INFO] Adding item <" << barcode <<"> to the Sales model ...";

        QString sql = "SELECT sales.sales_id,barcode,sales_date,product_bp,product_sp,sale_qty,cash,mpesa,cheque,credit FROM sales INNER JOIN payment ON sales.sales_id = payment.sales_id";
        QSqlQuery query, itemQuery;

        if(query.exec(sql))
        {
            // qDebug() << " [INFO] Successfully loaded todays sales ..";

            while(query.next())
            {
                QString sales_id = query.value(0).toString();
                QString barcode = query.value(1).toString();
                // QString sales_date = query.value(2).toString();
                int product_bp = query.value(3).toInt();
                int product_sp = query.value(4).toInt();
                int sale_qty = query.value(5).toInt();
                int cash = query.value(6).toInt();
                int mpesa = query.value(7).toInt();
                int cheque = query.value(8).toInt();
                int credit = query.value(9).toInt();

                m_json["cash"] = cash;
                m_json["mpesa"] = mpesa;
                m_json["cheque"] = cheque;
                m_json["credit"] = credit;

                itemQuery.prepare("SELECT product_name,product_unit FROM product WHERE barcode=:barcode");
                itemQuery.bindValue(":barcode", barcode);

                if(itemQuery.exec())
                {
                    while (itemQuery.next()) {
                        QString name = itemQuery.value(0).toString();
                        QString unit = itemQuery.value(1).toString();

                        // qDebug() << "Name: " << name << "\tUnit: " << unit;

                        addSalesData(new ProductSales(sales_id,barcode,name,unit,sale_qty,product_bp,product_sp, m_json));
                        // qDebug() << m_productSales;
                    }
                }

                else
                    emit logDataChanged("CRITICAL", "Couldn't fetch item data from products ==> " + itemQuery.lastError().text());
            }
        }
    }

    emit logDataChanged("INFO", "Ending ProductSalesModel::addSalesData()");
}

void ProductSalesModel::addSalesData(const QVariant &barcode, const int &qty)
{
    addSalesData(barcode, qty, m_json);
}

void ProductSalesModel::showTodaysSales()
{
    // Show today's sales
    QStringList todaysDateRange = m_dateTime->getTimestamp("today");
    executeQuery(todaysDateRange);
}

void ProductSalesModel::showYesterdaysSales()
{
    // Show yesterday's sales
    QStringList yesterdaysDateRange = m_dateTime->getTimestamp("yesterday");
    executeQuery(yesterdaysDateRange);
}

void ProductSalesModel::showThisWeeksSales()
{
    // Get this week's sales
    QStringList thisWeekDateRange = m_dateTime->getTimestamp("thisWeek");
    executeQuery(thisWeekDateRange);
}

void ProductSalesModel::showThisMonthsSales()
{
    // Get this month's sales
    QStringList thisMonthsDateRange = m_dateTime->getTimestamp("thisMonth");
    executeQuery(thisMonthsDateRange);
}

void ProductSalesModel::showThisYearsSales()
{
    // Get sales done this year
    QStringList thisYearsDateRange = m_dateTime->getTimestamp("thisYear");
    executeQuery(thisYearsDateRange);
}

void ProductSalesModel::showCustomDateSalesData(const QString &start, const QString &end)
{
    // Get sales done this year
    executeQuery({start, end});
}

void ProductSalesModel::getSalesSummary(const int &ind)
{
    Q_UNUSED(ind)
}

void ProductSalesModel::addSalesData(ProductSales *sales)
{
    const int index = m_productSales.size();

    beginInsertRows(QModelIndex(), index, index);
    m_productSales.append(sales);
    endInsertRows();
}

int ProductSalesModel::getSaleItemIndex(QString barcode)
{
    for(int i=0; i<m_productSales.size(); i++)
    {
        QString bcode = data(this->index(i), ProductBarcodeRole).toString();

        if(barcode == bcode)
            return i;
    }

    return -1;
}

bool ProductSalesModel::executeQuery(const QStringList &list)
{
    emit logDataChanged("INFO", "Starting ProductSalesModel::executeQuery");

    QSqlDatabase mdb = QSqlDatabase::database();

    int _count = 0;

    if(!mdb.isOpen())
        emit logDataChanged("FATAL", "[ERROR] Database not open/ready yet!");

    else
    {
        QSqlQuery itemQuery, query;
        clearModel();
        QString _sql = "SELECT sales.sales_id,barcode,sales_date,product_bp,product_sp,sale_qty,cash,mpesa,cheque,credit FROM sales INNER JOIN payment ON sales.sales_id = payment.sales_id WHERE sales_date > '" + list.at(0) + "' AND sales_date < '" + list.at(1) +"'";

        if(query.exec(_sql))
        {
            while(query.next())
            {
                QString sales_id = query.value(0).toString();
                QString barcode = query.value(1).toString();
                // QString sales_date = query.value(2).toString();
                int product_bp = query.value(3).toInt();
                int product_sp = query.value(4).toInt();
                int sale_qty = query.value(5).toInt();
                int cash = query.value(6).toInt();
                int mpesa = query.value(7).toInt();
                int cheque = query.value(8).toInt();
                int credit = query.value(9).toInt();

                int _ind = getSaleItemIndex(barcode);

                if(_ind == -1)
                {
                    m_json["cash"] = cash;
                    m_json["mpesa"] = mpesa;
                    m_json["cheque"] = cheque;
                    m_json["credit"] = credit;

                    itemQuery.prepare("SELECT product_name,product_unit FROM product WHERE barcode=:barcode");
                    itemQuery.bindValue(":barcode", barcode);

                    if(itemQuery.exec())
                    {
                        while (itemQuery.next()) {
                            QString name = itemQuery.value(0).toString();
                            QString unit = itemQuery.value(1).toString();

                            addSalesData(new ProductSales(sales_id,barcode,name,unit,sale_qty,product_bp,product_sp, m_json));
                            _count++;
                        }

                    }

                    else
                        emit logDataChanged("CRITICAL", "Couldn't fetch item data from products : " + itemQuery.lastError().text());

                }

                else
                {
                    // Update the current existing item details
                    int n_qty = sale_qty + data(this->index(_ind), ProductQtyRole).toInt();
                    setData(this->index(_ind), n_qty, ProductQtyRole);
                }
            }

            setCount(_count);
            return true;
        }

        else
        {
            // qDebug() << query.executedQuery();

            emit logDataChanged("CRITICAL", "Could'nt load sale data! ==> " + query.lastError().text());
        }
    }

    emit logDataChanged("INFO", "Ending ProductSalesModel::executeQuery");

    setCount(_count);
    return false;
}

void ProductSalesModel::clearModel()
{
    int index = m_productSales.size() - 1;

    if(index >= 0)
    {
        beginRemoveRows(QModelIndex(), 0, index);
        m_productSales.clear();
        endRemoveRows();
    } else {
        m_productSales.clear();
    }

    setCount(0);
}

int ProductSalesModel::count() const
{
    return m_count;
}

void ProductSalesModel::setCount(int count)
{
    if (m_count == count)
        return;

    m_count = count;
    emit countChanged(m_count);
}
