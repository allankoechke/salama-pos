#include "databaseinterface.h"

#include <QDir>
#include <QProcessEnvironment>

DatabaseInterface::DatabaseInterface(QObject *parent) : QObject(parent)
{
    loadCredentialsFromEnvironment();
}

void DatabaseInterface::loadCredentialsFromEnvironment()
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    
    // Load from environment variables with defaults (except password)
    db_host = env.value("S_DB_HOST", "localhost");
    db_uname = env.value("S_DB_USERNAME", "postgres");
    db_name = env.value("S_DB_NAME", "salama");
    
    // Password must be provided via environment variable (no default for security)
    db_pswd = env.value("S_DB_PASSWORD");
    
    if(db_pswd.isEmpty())
    {
        qWarning() << "S_DB_PASSWORD environment variable is not set. Database connection may fail.";
        m_lastError = "Database password not configured. Please set S_DB_PASSWORD environment variable.";
    }
    
    qDebug() << "Database configuration loaded:";
    qDebug() << "  Host:" << db_host;
    qDebug() << "  Username:" << db_uname;
    qDebug() << "  Database:" << db_name;
    qDebug() << "  Password:" << (db_pswd.isEmpty() ? "NOT SET" : "SET");
}


QString DatabaseInterface::initializeDatabase()
{
    // Check if password is set
    if(db_pswd.isEmpty())
    {
        m_lastError = "Database password not configured. Please set S_DB_PASSWORD environment variable.";
        emit connectionError(m_lastError);
        return "false:" + m_lastError;
    }

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
            return "false:" + m_lastError;
        }

        QSqlQuery query(m_db);
        QString sql;

        QFile file(":/sql/tables.sql");
        if(!file.open(QIODevice::ReadOnly))
        {
            m_lastError = "Failed to open tables.sql resource file.";
            emit connectionError(m_lastError);
            return "false:" + m_lastError;
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
                qDebug() << "Error executing SQL: " << query.lastError().text();
                emit connectionError(m_lastError);
                return "false:" + m_lastError;
            }
        }

        if(!m_db.commit())
        {
            m_lastError = QString("Failed to commit database transaction: %1").arg(m_db.lastError().text());
            emit connectionError(m_lastError);
            return "false:" + m_lastError;
        }

        if(m_db.isOpen())
        {
            m_lastError.clear();
            return "true:OK";
        }
        else
        {
            m_lastError = QString("Database connection lost: %1").arg(m_db.lastError().text());
            emit connectionError(m_lastError);
            return "false:" + m_lastError;
        }

    } catch (std::exception &e) {
        m_lastError = QString("Exception while connecting to database: %1").arg(QString::fromUtf8(e.what()));
        emit connectionError(m_lastError);
        return "false:" + m_lastError;
    }

    m_lastError = "Unknown error creating database";
    emit connectionError(m_lastError);
    return "false:" + m_lastError;
}

QString DatabaseInterface::getLastError() const
{
    return m_lastError;
}
