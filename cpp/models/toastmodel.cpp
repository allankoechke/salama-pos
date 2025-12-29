#include "toastmodel.h"

ToastModel::ToastModel(QObject *parent) : QAbstractListModel(parent)
{
}

int ToastModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_toastList.size();
}

QVariant ToastModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() > m_toastList.count())
        return QVariant();

    Toast * toast = m_toastList.at(index.row());

    if(role == ToastTypeRole)
        return toast->toastType();

    if(role == ToastStringRole)
        return toast->toastString();

    if(role == ToastIdRole)
        return toast->toastId();

    if(role == ToastIsActiveRole)
        return toast->toastIsActive();

    else return QVariant();
}

bool ToastModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Toast * toast = m_toastList.at(index.row());
    bool changed = false;

    switch (role)
    {
    case ToastTypeRole:
    {
        if( toast->toastType() != value.toString())
        {
            toast->setToastType(value.toString());
            changed = true;
        }

        break;
    }

    case ToastStringRole:
    {
        if( toast->toastString() != value.toString())
        {
            toast->setToastString(value.toString());
            changed = true;
        }

        break;
    }

    case ToastIdRole:
    {
        if( toast->toastId() != value.toString())
        {
            toast->setToastId(value.toString());
            changed = true;
        }

        break;
    }

    case ToastIsActiveRole:
    {
        if( toast->toastIsActive() != value.toBool())
        {
            toast->setToastIsActive(value.toBool());
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

Qt::ItemFlags ToastModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> ToastModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[ToastTypeRole] = "toast_type";
    roles[ToastStringRole] = "toast_text";
    roles[ToastIdRole] = "toast_id";
    roles[ToastIsActiveRole] = "toast_isActive";

    return roles;
}

void ToastModel::addToastItem(const QString &type, const QString &content)
{
    QString id = QString::number(QDateTime::currentMSecsSinceEpoch());
    addToastItem(new Toast(type, content, id, false));
}

void ToastModel::removeToastItem(const QVariant &id)
{
    int _id = getToastId(id);
    // qDebug() << "Index: " << _id << " for " << id;

    if(_id != -1)
    {
        beginRemoveRows(QModelIndex(), _id, _id);
        m_toastList.removeAt(_id);
        endRemoveRows();

        // qDebug() << "Toast Removed";
    }


    setSize(m_toastList.size());
}

void ToastModel::addToastItem(Toast *toast)
{
    const int index = m_toastList.size();
    beginInsertRows(QModelIndex(), index, index);
    m_toastList.append(toast);
    endInsertRows();

    setSize(m_toastList.size());
}

int ToastModel::getToastId(const QVariant &id)
{
    for(int i=0; i<m_toastList.size(); i++)
    {
        QVariant _id = data(this->index(i), ToastIdRole);

        if(id == _id)
            return i;
    }

    return -1;
}

int ToastModel::size() const
{
    return m_size;
}

void ToastModel::setSize(int size)
{
    if (m_size == size)
        return;

    m_size = size;
    emit sizeChanged(m_size);
}

