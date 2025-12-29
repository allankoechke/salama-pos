#ifndef TOASTMODEL_H
#define TOASTMODEL_H

#include <QObject>
#include <QDebug>
#include <QDateTime>
#include <QAbstractListModel>

#include "src/toast.h"

class ToastModel : public QAbstractListModel
{
    Q_OBJECT

    enum ToastModelRoles{
        ToastTypeRole = Qt::UserRole + 1,
        ToastStringRole,
        ToastIdRole,
        ToastIsActiveRole
    };

public:
    explicit ToastModel(QObject *parent = nullptr);

    // QAbstractListModel overrides
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role);

    Qt::ItemFlags flags(const QModelIndex &index) const;

    QHash<int, QByteArray> roleNames() const;

    // Property
    Q_PROPERTY(int size READ size WRITE setSize NOTIFY sizeChanged)

    // Q_INVOKABLES
    Q_INVOKABLE void addToastItem(const QString &type, const QString &content);
    Q_INVOKABLE void removeToastItem(const QVariant &id);

    // Internals
    void addToastItem(Toast * toast);
    int getToastId(const QVariant &id);

    int size() const;

public slots:
    void setSize(int size);

signals:
    void logDataChanged(QString level, QString info);
    void sizeChanged(int size);

private:
    QList<Toast *> m_toastList;

    int m_size;
};

#endif // TOASTMODEL_H

