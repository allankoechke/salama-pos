#ifndef DATABASEINTERFACE_H
#define DATABASEINTERFACE_H

#include <QObject>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariantMap>
#include <QSqlDatabase>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <exception>
#include <QFile>
#include <QDate>
#include <QProcessEnvironment>


class DatabaseInterface : public QObject
{
    Q_OBJECT

public:
    explicit DatabaseInterface(QObject *parent = nullptr);

    // Initialize database connection and create tables
    // Returns true on success, false on failure
    // Use getLastError() to get error details on failure
    bool initializeDatabase();
    
    // Get connection error details for user display
    QString getLastError() const;

signals:
    void connectionError(const QString &errorMessage);

public slots:

private:
    void loadCredentialsFromEnvironment();
    
    QString db_pswd, db_name, db_uname, db_host;
    QString m_lastError;
};

#endif // DATABASEINTERFACE_H
