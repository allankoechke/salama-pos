#include "databaseinterface.h"

#include <QDir>

DatabaseInterface::DatabaseInterface(QObject *parent) : QObject(parent)
{
    qDebug() << "DIRS: " << QDir(":/").entryList(QDir::AllEntries | QDir::NoDotAndDotDot);


    QFile file(":/json/db-credentials.json");
    assert(file.exists());
    auto ok = file.open(QIODevice::ReadOnly|QIODevice::Text);
    Q_ASSERT(ok); // Assert the file opened

    QString jsonString = file.readAll();
    file.close();

    qDebug() << "File contents: " << jsonString;

    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
    QJsonObject jsonObj = doc.object();

    db_uname = jsonObj.value(QString("username")).toString();
    db_pswd = jsonObj.value(QString("password")).toString();
    db_name = jsonObj.value(QString("database")).toString();
}


QString DatabaseInterface::initializeDatabase()
{
    QSqlDatabase m_db;

    // TODO move db vars to env variables
    // S_DB_HOST
    // S_DB_USERNAME
    // S_DB_PASSWORD
    // S_DB_NAME

    try {
        m_db = QSqlDatabase::database("default");
        m_db = m_db.addDatabase("QPSQL");
        m_db.setHostName("localhost");
        m_db.setUserName(db_uname);
        m_db.setPassword(db_pswd);
        m_db.setDatabaseName("salama");

        if(!m_db.open())
        {
            return "false:Error Connecting Db => " + m_db.lastError().text();
        }

        else
        {
            Q_ASSERT(m_db.open());

            QSqlQuery query;
            QString sql;

            QFile file(":/sql/tables.sql");
            auto _ = file.open(QIODevice::ReadOnly);
            sql = file.readAll();

            auto sql_segments = sql.split("--comment");

            m_db.transaction();
            foreach (const QString &sql_str, sql_segments)
            {
                if(!query.exec(sql_str))
                {
                    m_db.rollback();
                    qDebug() << "Error executing : " << query.lastError().text();
                }
            }

            if(m_db.isOpen())
                return "true:OK";

            else
            {
                QString str = "Error Creating Db: " + query.lastError().text();
                return "false:" + str;
            }
        }

    } catch (std::exception &e) {
        QString err = ">> Error executing SQL: " + QString::fromUtf8(e.what());
        return "false:"+err;
    }

    return "false:Error Creating Database";
}
