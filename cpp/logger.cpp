#include "logger.h"
#include <QDebug>
#include <QDateTime>

Logger::Logger(QObject *parent) : QObject(parent)
{
    // Ensure logs directory exists
    QString logsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    m_logsPath = logsPath + "/SalamaPOS/logs";
    m_logDir = QDir(m_logsPath);
    m_logDir.mkpath(m_logsPath);
    
    ensureLogFileOpen();
}

Logger::~Logger()
{
    if(m_logWriter.is_open())
    {
        m_logWriter.close();
    }
}

QString Logger::getLogFilePath()
{
    QString logName = QDateTime::currentDateTime().toString("yyyy-MM-dd") + "_app.log";
    return m_logsPath + "/" + logName;
}

void Logger::ensureLogFileOpen()
{
    QString logFilePath = getLogFilePath();
    
    // Check if we need to open a new file (different day)
    if(m_currentLogFileName != logFilePath)
    {
        if(m_logWriter.is_open())
        {
            m_logWriter.close();
        }
        
        m_currentLogFileName = logFilePath;
        m_logWriter.open(logFilePath.toStdString(), std::ios::out | std::ios::app | std::ios::binary | std::ios::ate);
        
        if(!m_logWriter.is_open())
        {
            qWarning() << "Failed to open log file:" << logFilePath;
        }
    }
    else if(!m_logWriter.is_open())
    {
        // Same day but file was closed, reopen it
        m_logWriter.open(logFilePath.toStdString(), std::ios::out | std::ios::app | std::ios::binary | std::ios::ate);
        
        if(!m_logWriter.is_open())
        {
            qWarning() << "Failed to open log file:" << logFilePath;
        }
    }
}

void Logger::log(const QString &level, const QString &shortMessage, const QString &longMessage)
{
    ensureLogFileOpen();
    
    // Format: [timestamp] [log level] [Short Message] \n\t [Long Message if available]
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    QString logLine = QString("[%1] [%2] %3").arg(timestamp, level, shortMessage);
    
    // Add long message if available
    if(!longMessage.isEmpty())
    {
        logLine += QString("\n\t> %1").arg(longMessage);
    }
    
    logLine += "\n";
    
    // Write to file
    if(m_logWriter.is_open())
    {
        m_logWriter << logLine.toStdString();
        m_logWriter.flush();
    }
    
    // Also output to console with appropriate Qt logging
    if(level == "DEBUG")
    {
        qDebug() << logLine.trimmed();
    }
    else if(level == "INFO")
    {
        qInfo() << logLine.trimmed();
    }
    else if(level == "WARNING")
    {
        qWarning() << logLine.trimmed();
    }
    else if(level == "ERROR")
    {
        qCritical() << logLine.trimmed();
    }
    else if(level == "CRITICAL")
    {
        qCritical() << logLine.trimmed();
    }
    else if(level == "FATAL")
    {
        qFatal("%s", logLine.toStdString().c_str());
    }
}

void Logger::debug(const QString &message, const QString &longMessage)
{
    log("DEBUG", message, longMessage);
}

void Logger::info(const QString &message, const QString &longMessage)
{
    log("INFO", message, longMessage);
}

void Logger::warning(const QString &message, const QString &longMessage)
{
    log("WARNING", message, longMessage);
}

void Logger::error(const QString &message, const QString &longMessage)
{
    log("ERROR", message, longMessage);
}

void Logger::critical(const QString &message, const QString &longMessage)
{
    log("CRITICAL", message, longMessage);
}

void Logger::fatal(const QString &message, const QString &longMessage)
{
    log("FATAL", message, longMessage);
}

