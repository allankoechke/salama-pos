#ifndef TOAST_H
#define TOAST_H

#include <QObject>

class Toast : public QObject
{
    Q_OBJECT

public:
    explicit Toast(QObject *parent = nullptr);
    Toast(const QString &toastType, const QString &toastString, const QString &toastId, const bool &toastIsActive, QObject *parent = nullptr);

    Q_PROPERTY(QString toastType READ toastType WRITE setToastType NOTIFY toastTypeChanged)
    Q_PROPERTY(QString toastString READ toastString WRITE setToastString NOTIFY toastStringChanged)
    Q_PROPERTY(QString toastId READ toastId WRITE setToastId NOTIFY toastIdChanged)
    Q_PROPERTY(bool toastIsActive READ toastIsActive WRITE setToastIsActive NOTIFY toastIsActiveChanged)


    QString toastType() const;

    QString toastString() const;

    QString toastId() const;

    bool toastIsActive() const;

    void setToastType(QString toastType);

    void setToastString(QString toastString);

    void setToastId(QString toastId);

    void setToastIsActive(bool toastIsActive);

signals:
    void toastTypeChanged(QString toastType);
    void toastStringChanged(QString toastString);
    void toastIdChanged(QString toastId);
    void toastIsActiveChanged(bool toastIsActive);

private:
    QString m_toastType, m_toastString, m_toastId;

    bool m_toastIsActive;
};

#endif // TOAST_H

