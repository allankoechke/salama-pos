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

private:
    void log(const QString &level, const QString &shortMessage, const QString &longMessage = QString());
    void ensureLogFileOpen();
    QString getLogFilePath();
    
    std::ofstream m_logWriter;
    QString m_logsPath;
    QString m_currentLogFileName;
    QDir m_logDir;
};

#endif // LOGGER_H

