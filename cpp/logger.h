#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <fstream>
#include <QStandardPaths>
#include <QDir>

class Logger : public QObject
{
    Q_OBJECT

public:
    explicit Logger(QObject *parent = nullptr);
    ~Logger();

    // QML-invokable logging methods
    Q_INVOKABLE void debug(const QString &message, const QString &longMessage = QString());
    Q_INVOKABLE void info(const QString &message, const QString &longMessage = QString());
    Q_INVOKABLE void warning(const QString &message, const QString &longMessage = QString());
    Q_INVOKABLE void error(const QString &message, const QString &longMessage = QString());
    Q_INVOKABLE void critical(const QString &message, const QString &longMessage = QString());
    Q_INVOKABLE void fatal(const QString &message, const QString &longMessage = QString());

    // Static methods for C++ usage
    static void setInstance(Logger *instance);
    static Logger* instance();
    
    // Static convenience methods for C++ usage
    static void logDebug(const QString &message, const QString &longMessage = QString());
    static void logInfo(const QString &message, const QString &longMessage = QString());
    static void logWarning(const QString &message, const QString &longMessage = QString());
    static void logError(const QString &message, const QString &longMessage = QString());
    static void logCritical(const QString &message, const QString &longMessage = QString());
    static void logFatal(const QString &message, const QString &longMessage = QString());

private:
    void log(const QString &level, const QString &shortMessage, const QString &longMessage = QString());
    void ensureLogFileOpen();
    QString getLogFilePath();
    
    std::ofstream m_logWriter;
    QString m_logsPath;
    QString m_currentLogFileName;
    QDir m_logDir;
    
    static Logger* s_instance;
};

#endif // LOGGER_H

