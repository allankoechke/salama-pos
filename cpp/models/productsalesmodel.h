#ifndef PRODUCTSALESMODEL_H
#define PRODUCTSALESMODEL_H

#include <QObject>
#include <QDebug>
#include <QAbstractListModel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "src/productsales.h"
#include "../datetime.h"

class ProductSalesModel : public QAbstractListModel
{
    Q_OBJECT

    enum ProductSalesRoles {
        ProductSaleIdRole = Qt::UserRole+1,
        ProductBarcodeRole,
        ProductNameRole,
        ProductUnitRole,
        ProductQtyRole,
        ProductBpRole,
        ProductSpRole,
        ProductPaymentRole
    };

public:
    explicit ProductSalesModel(QObject *parent = nullptr);

    // QAbstractListModel overrides
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QHash<int, QByteArray> roleNames() const;

    Q_PROPERTY(int count READ count WRITE setCount NOTIFY countChanged)

    // Q_INVOKABLES
    Q_INVOKABLE void loadSalesData();
    Q_INVOKABLE void addSalesData(const QVariant &barcode, const int &qty, const QJsonObject &cost);
    Q_INVOKABLE void addSalesData(const QVariant &barcode, const int &qty);
    Q_INVOKABLE void showTodaysSales();
    Q_INVOKABLE void showYesterdaysSales();
    Q_INVOKABLE void showThisWeeksSales();
    Q_INVOKABLE void showThisMonthsSales();
    Q_INVOKABLE void showThisYearsSales();
    Q_INVOKABLE void showCustomDateSalesData(const QString& start, const QString& end);
    Q_INVOKABLE void getSalesSummary(const int &ind);


    // Internals
    bool executeQuery(const QStringList &list);
    void addSalesData(ProductSales *sales);
    int getSaleItemIndex(QString barcode);
    void clearModel();

    int count() const;

public slots:
    void setCount(int count);

signals:
    void salesSummaryCost(int cash, int mpesa, int cheque, int credit, int paid, int totals);
    void logDataChanged(QString level, QString info);

    void countChanged(int count);

private:
    QList<ProductSales *> m_productSales;

    QJsonObject m_json;

    DateTime * m_dateTime;
    int m_count;
};

#endif // PRODUCTSALESMODEL_H
