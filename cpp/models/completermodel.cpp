#include "completermodel.h"

CompleterModel::CompleterModel(QObject *parent) : QAbstractListModel(parent)
{

}

int CompleterModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_completer.size();
}

QVariant CompleterModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() > m_completer.count())
        return QVariant();

    Completer * completer = m_completer.at(index.row());

    if(role == CompleterNameRole)
        return completer->name();

    if(role == CompleterKeyRole)
        return completer->key();

    if(role == CompleterSpRole)
        return completer->sp();


    else return QVariant();
}

bool CompleterModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Completer * completer = m_completer.at(index.row());
    bool changed = false;

    switch (role)
    {
    case CompleterNameRole:
    {
        if( completer->name() != value.toString())
        {
            completer->setName(value.toString());
            changed = true;
        }

        break;
    }

    case CompleterKeyRole:
    {
        if( completer->key() != value.toString())
        {
            completer->setKey(value.toString());
            changed = true;
        }

        break;
    }

    case CompleterSpRole:
    {
        if( completer->sp() != value.toFloat())
        {
            completer->setSp(value.toFloat());
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

Qt::ItemFlags CompleterModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> CompleterModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[CompleterNameRole] = "name";
    roles[CompleterKeyRole] = "key";
    roles[CompleterSpRole] = "sp";

    return roles;
}

void CompleterModel::addCompleterItems(const QVariant &str)
{
    emit logDataChanged("INFO", "Starting CompleterModel::addCompleterItems()");

    // Clear existing model
    if(m_completer.size() != 0)
        clearModel();

    if(str.toString() != "" /* str.toString() != " " */)
    {
        // Create database object
        QSqlDatabase m_db = QSqlDatabase::database();

        QSqlQuery query;
        QString sql = "SELECT barcode, product_name, product_unit, product_company, product_sp FROM product WHERE product_name ILIKE :productname";

        query.prepare(sql);
        query.bindValue(":productname", str.toString() + "%");

        if(query.exec())
        {
            while (query.next())
            {
                QString bcode = query.value(0).toString();
                QString name = query.value(1).toString();
                QString unit = query.value(2).toString();
                QString company = query.value(3).toString();
                float sp = query.value(4).toInt();

                QString _c = (company=="" || company==" ")? "":(" ("+company+")");

                QString _name = unit + " " + name + _c;

                // qDebug() << "Key: " << bcode << "\tStr: " << _name;

                addItem(new Completer(_name, bcode, sp));
            }

            setCompleterSize(size());
        }

        else
        {
            auto errStr = "Error Executing SQL: [" + query.executedQuery() + "] -> " + query.lastError().text();

            // qDebug() << errStr;

            emit logDataChanged("CRITICAL", errStr);
        }
    }

    emit logDataChanged("INFO", "Ending CompleterModel::addCompleterItems()");
}

void CompleterModel::removeCompleterItem(const QVariant &str)
{
    const int index = findItemIndex(str.toString());

    if(index != -1)
        removeItem(index);

    setCompleterSize(size());
}

int CompleterModel::size()
{
    return m_completer.size();
}

QString CompleterModel::getKey(int index)
{
    QString key = data(this->index(index), CompleterKeyRole).toString();

    return key;
}

QVariantMap CompleterModel::getData(int index)
{
    QString key = data(this->index(index), CompleterKeyRole).toString();
    QString name = data(this->index(index), CompleterNameRole).toString();

    QJsonObject obj;
    obj.insert("name", name);
    obj.insert("key", key);

    return obj.toVariantMap();
}

int CompleterModel::findItemIndex(const QString &str)
{
    if(str != "")
    {
        for(int i=0; i<m_completer.size(); i++)
        {
            QString _key = data(this->index(i), CompleterKeyRole).toString();

            if(str ==  _key)
                return i;
        }
    }

    return -1;
}

void CompleterModel::addItem(Completer *completer)
{
    const int index = m_completer.size();

    beginInsertRows(QModelIndex(), index, index);
    m_completer.append(completer);
    endInsertRows();
}

void CompleterModel::removeItem(int index)
{
    beginRemoveRows(QModelIndex() , index, index);
    m_completer.removeAt(index);
    endRemoveRows();
}

void CompleterModel::clearModel()
{
    for(int i=m_completer.size()-1; i>=0; i--)
    {
        removeItem(i);
    }

    setCompleterSize(size());
}

int CompleterModel::completerSize() const
{
    return m_completerSize;
}

void CompleterModel::setCompleterSize(int completerSize)
{
    if (m_completerSize == completerSize)
        return;

    m_completerSize = completerSize;
    emit completerSizeChanged(m_completerSize);
}
