#include "databaseinterface.h"
#include "logger.h"

#include <QDir>
#include <QProcessEnvironment>

DatabaseInterface::DatabaseInterface(QObject *parent) : QObject(parent)
{
    loadCredentialsFromEnvironment();
}

void DatabaseInterface::loadCredentialsFromEnvironment()
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    
    // Load from environment variables with defaults
    db_host = env.value("S_DB_HOST", "localhost");
    db_uname = env.value("S_DB_USERNAME", "postgres");
    db_name = env.value("S_DB_NAME", "salama");
    
    // Password is optional - empty means no password authentication
    // (useful for trust/peer authentication or development environments)
    db_pswd = env.value("S_DB_PASSWORD");
    
    QString configMsg = QString("Host: %1, Username: %2, Database: %3, Password: %4")
        .arg(db_host, db_uname, db_name, db_pswd.isEmpty() ? "Not set" : "Set");
    Logger::logDebug("Database configuration loaded", configMsg);
}


bool DatabaseInterface::initializeDatabase()
{
    QSqlDatabase m_db;

    try {
        m_db = QSqlDatabase::database("default");
        if(m_db.isValid())
        {
            QSqlDatabase::removeDatabase("default");
        }
        m_db = QSqlDatabase::addDatabase("QPSQL", "default");
        m_db.setHostName(db_host);
        m_db.setUserName(db_uname);
        m_db.setPassword(db_pswd);
        m_db.setDatabaseName(db_name);

        if(!m_db.open())
        {
            m_lastError = QString("Failed to connect to database.\n\n"
                                 "Host: %1\n"
                                 "Database: %2\n"
                                 "Username: %3\n\n"
                                 "Error: %4")
                         .arg(db_host, db_name, db_uname, m_db.lastError().text());
            emit connectionError(m_lastError);
            return false;
        }

        QSqlQuery query(m_db);
        QString sql;

        QFile file(":/sql/tables.sql");
        if(!file.open(QIODevice::ReadOnly))
        {
            m_lastError = "Failed to open tables.sql resource file.";
            emit connectionError(m_lastError);
            return false;
        }
        sql = file.readAll();
        file.close();

        auto sql_segments = sql.split("--comment");

        m_db.transaction();
        foreach (const QString &sql_str, sql_segments)
        {
            QString trimmed = sql_str.trimmed();
            if(!trimmed.isEmpty() && !query.exec(trimmed))
            {
                m_db.rollback();
                m_lastError = QString("Error executing SQL: %1").arg(query.lastError().text());
                Logger::logError("SQL execution failed", query.lastError().text());
                emit connectionError(m_lastError);
                return false;
            }
        }

        if(!m_db.commit())
        {
            m_lastError = QString("Failed to commit database transaction: %1").arg(m_db.lastError().text());
            emit connectionError(m_lastError);
            return false;
        }

        if(!m_db.isOpen())
        {
            m_lastError = QString("Database connection lost: %1").arg(m_db.lastError().text());
            emit connectionError(m_lastError);
            return false;
        }

        // Success
        m_lastError.clear();
        return true;

    } catch (std::exception &e) {
        m_lastError = QString("Exception while connecting to database: %1").arg(QString::fromUtf8(e.what()));
        emit connectionError(m_lastError);
        return false;
    }
}

QString DatabaseInterface::getLastError() const
{
    return m_lastError;
}
