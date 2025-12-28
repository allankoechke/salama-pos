#ifndef STOCKITEMS_H
#define STOCKITEMS_H

#include <QObject>
#include <QDebug>
#include <QVariant>

class StockItems : public QObject
{
    Q_OBJECT


public:
    explicit StockItems(QObject *parent = nullptr);
    StockItems(const QString barCode, const QString itemName, const QString itemUnit, const float itemBp, const float itemSp, const QString itemCompany, const int itemQty, const QString lastUpdate, const QString itemCategory, QObject *parent = nullptr);

    Q_PROPERTY(QString barCode READ barCode WRITE setBarCode NOTIFY barCodeChanged)
    Q_PROPERTY(QString itemName READ itemName WRITE setItemName NOTIFY itemNameChanged)
    Q_PROPERTY(QString itemUnit READ itemUnit WRITE setItemUnit NOTIFY itemUnitChanged)
    Q_PROPERTY(float itemBp READ itemBp WRITE setItemBp NOTIFY itemBpChanged)
    Q_PROPERTY(float itemSp READ itemSp WRITE setItemSp NOTIFY itemSpChanged)
    Q_PROPERTY(int itemQty READ itemQty WRITE setItemQty NOTIFY itemQtyChanged)
    Q_PROPERTY(QString itemCompany READ itemCompany WRITE setItemCompany NOTIFY itemCompanyChanged)
    Q_PROPERTY(QString lastUpdate READ lastUpdate WRITE setLastUpdate NOTIFY lastUpdateChanged)
    Q_PROPERTY(QString itemCategory READ itemCategory WRITE setItemCategory NOTIFY itemCategoryChanged)

    // Getters
    QString barCode() const;

    QString itemName() const;

    QString itemUnit() const;

    float itemBp() const;

    float itemSp() const;

    int itemQty() const;

    QString itemCompany() const;

    QString lastUpdate() const;

    QString itemCategory() const;

    // Setters
    void setBarCode(QString barCode);

    void setItemName(QString itemName);

    void setItemUnit(QString itemUnit);

    void setItemBp(float itemBp);

    void setItemSp(float itemSp);

    void setItemQty(int itemQty);

    void setItemCompany(QString itemCompany);

    void setLastUpdate(QString lastUpdate);

    void setItemCategory(QString itemCategory);

signals:
    void barCodeChanged(QString barCode);
    void itemNameChanged(QString itemName);
    void itemUnitChanged(QString itemUnit);
    void itemBpChanged(float itemBp);
    void itemSpChanged(float itemSp);
    void itemQtyChanged(int itemQty);
    void itemCompanyChanged(QString itemCompany);
    void lastUpdateChanged(QString lastUpdate);
    void itemCategoryChanged(QString itemCategory);

private:
    QString m_barCode, m_itemName, m_itemUnit, m_itemCompany, m_lastUpdate, m_itemCategory;

    float m_itemBp, m_itemSp;

    int m_itemQty;
};

#endif // STOCKITEMS_H
