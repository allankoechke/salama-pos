#ifndef BACKENDMANAGER_H
#define BACKENDMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QProcess>
#include <QTimer>
#include <QJsonObject>
#include <QString>

class BackendManager : public QObject
{
    Q_OBJECT

public:
    explicit BackendManager(QObject *parent = nullptr);
    ~BackendManager();

    // Backend health and connection
    Q_INVOKABLE void checkHealth();
    Q_INVOKABLE bool isBackendRunning() const;
    Q_INVOKABLE void startBackend();
    Q_INVOKABLE void stopBackend();
    
    // API request methods (using signals for callbacks)
    Q_INVOKABLE void postRequest(const QString &endpoint, const QJsonObject &data);
    Q_INVOKABLE void getRequest(const QString &endpoint);
    
    // Convenience methods for common operations
    Q_INVOKABLE void login(const QString &username, const QString &password);
    
    // Properties
    Q_PROPERTY(bool backendRunning READ isBackendRunning NOTIFY backendRunningChanged)
    Q_PROPERTY(QString baseUrl READ baseUrl CONSTANT)
    Q_PROPERTY(int backendPort READ backendPort CONSTANT)

signals:
    void backendRunningChanged(bool running);
    void backendStarted();
    void backendStopped();
    void healthCheckFailed();
    void apiError(const QString &error);
    void apiResponse(const QString &endpoint, const QJsonObject &response);
    void apiLoginResponse(const QJsonObject &response);

private slots:
    void onHealthCheckReply();
    void onBackendProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onBackendProcessError(QProcess::ProcessError error);
    void performHealthCheck();

private:
    void setupBackendProcess();
    QString baseUrl() const;
    int backendPort() const;
    QUrl buildUrl(const QString &endpoint) const;
    
    QNetworkAccessManager *m_networkManager;
    QProcess *m_backendProcess;
    QTimer *m_healthCheckTimer;
    
    bool m_backendRunning;
    int m_backendPort;
    QString m_backendExecutablePath;
    
    QNetworkReply *m_currentReply;
    QString m_lastRequestEndpoint;
};

#endif // BACKENDMANAGER_H

