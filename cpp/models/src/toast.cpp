#include "toast.h"

Toast::Toast(QObject *parent) : QObject(parent)
{

}

Toast::Toast(const QString &toastType, const QString &toastString, const QString &toastId, const bool &toastIsActive, QObject *parent)
    : QObject(parent), m_toastType(toastType), m_toastString(toastString), m_toastId(toastId), m_toastIsActive(toastIsActive)
{ }

QString Toast::toastType() const
{
    return m_toastType;
}

QString Toast::toastString() const
{
    return m_toastString;
}

QString Toast::toastId() const
{
    return m_toastId;
}

bool Toast::toastIsActive() const
{
    return m_toastIsActive;
}

void Toast::setToastType(QString toastType)
{
    if (m_toastType == toastType)
        return;

    m_toastType = toastType;
    emit toastTypeChanged(m_toastType);
}

void Toast::setToastString(QString toastString)
{
    if (m_toastString == toastString)
        return;

    m_toastString = toastString;
    emit toastStringChanged(m_toastString);
}

void Toast::setToastId(QString toastId)
{
    if (m_toastId == toastId)
        return;

    m_toastId = toastId;
    emit toastIdChanged(m_toastId);
}

void Toast::setToastIsActive(bool toastIsActive)
{
    if (m_toastIsActive == toastIsActive)
        return;

    m_toastIsActive = toastIsActive;
    emit toastIsActiveChanged(m_toastIsActive);
}



