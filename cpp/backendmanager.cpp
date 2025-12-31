#include "backendmanager.h"
#include "logger.h"
#include <QStandardPaths>
#include <QDir>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

BackendManager::BackendManager(QObject *parent)
    : QObject(parent)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_backendProcess(new QProcess(this))
    , m_healthCheckTimer(new QTimer(this))
    , m_backendRunning(false)
    , m_backendPort(7090)
    , m_currentReply(nullptr)
{
    // Determine backend executable path
    QString appDir = QCoreApplication::applicationDirPath();
    m_backendExecutablePath = appDir + "/salama-pos-backend";
    
    // On Windows, add .exe extension
#ifdef Q_OS_WINDOWS
    m_backendExecutablePath += ".exe";
#endif
    
    // Setup backend process
    setupBackendProcess();
    
    // Setup health check timer (check every 5 seconds)
    m_healthCheckTimer->setInterval(5000);
    m_healthCheckTimer->setSingleShot(false);
    connect(m_healthCheckTimer, &QTimer::timeout, this, &BackendManager::performHealthCheck);
    
    // Initial health check
    QTimer::singleShot(1000, this, &BackendManager::checkHealth);
}

BackendManager::~BackendManager()
{
    if (m_backendProcess && m_backendProcess->state() != QProcess::NotRunning) {
        m_backendProcess->terminate();
        if (!m_backendProcess->waitForFinished(3000)) {
            m_backendProcess->kill();
        }
    }
}

void BackendManager::setupBackendProcess()
{
    // Set backend to run detached so it continues even if app closes
    m_backendProcess->setProgram(m_backendExecutablePath);
    
    // Set environment variables for database connection
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    // Database env vars are already set, backend will read them
    
    // Set backend port via environment
    env.insert("SALAMA_BACKEND_PORT", QString::number(m_backendPort));
    m_backendProcess->setProcessEnvironment(env);
    
    // Connect signals
    connect(m_backendProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &BackendManager::onBackendProcessFinished);
    connect(m_backendProcess, &QProcess::errorOccurred,
            this, &BackendManager::onBackendProcessError);
    
    // Start process in detached mode (runs independently)
    m_backendProcess->setProcessChannelMode(QProcess::MergedChannels);
}

QString BackendManager::baseUrl() const
{
    return QString("http://127.0.0.1:%1").arg(m_backendPort);
}

int BackendManager::backendPort() const
{
    return m_backendPort;
}

QUrl BackendManager::buildUrl(const QString &endpoint) const
{
    QString url = baseUrl();
    if (!endpoint.startsWith("/")) {
        url += "/";
    }
    url += endpoint;
    return QUrl(url);
}

bool BackendManager::isBackendRunning() const
{
    return m_backendRunning;
}

void BackendManager::checkHealth()
{
    performHealthCheck();
}

void BackendManager::performHealthCheck()
{
    QUrl url = buildUrl("/api/v1/health");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    
    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &BackendManager::onHealthCheckReply);
    
    // Set timeout
    QTimer::singleShot(3000, reply, [reply]() {
        if (reply->isRunning()) {
            reply->abort();
        }
    });
}

void BackendManager::onHealthCheckReply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;
    
    bool wasRunning = m_backendRunning;
    
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(data, &error);
        
        if (error.error == QJsonParseError::NoError && doc.isObject()) {
            QJsonObject obj = doc.object();
            if (obj.value("status").toString() == "ok") {
                m_backendRunning = true;
                if (!wasRunning) {
                    emit backendRunningChanged(true);
                    emit backendStarted();
                    Logger::logInfo("Backend health check passed");
                }
                // Start periodic health checks
                if (!m_healthCheckTimer->isActive()) {
                    m_healthCheckTimer->start();
                }
                reply->deleteLater();
                return;
            }
        }
    }
    
    // Health check failed
    m_backendRunning = false;
    if (wasRunning) {
        emit backendRunningChanged(false);
        emit backendStopped();
        Logger::logWarning("Backend health check failed", reply->errorString());
        emit healthCheckFailed();
    }
    
    reply->deleteLater();
}

void BackendManager::startBackend()
{
    if (m_backendProcess->state() != QProcess::NotRunning) {
        Logger::logInfo("Backend process already running");
        return;
    }
    
    Logger::logInfo("Starting backend process", m_backendExecutablePath);
    
    // Start process detached so it runs independently
    bool started = m_backendProcess->startDetached();
    
    if (started) {
        Logger::logInfo("Backend process started successfully");
        // Wait a bit then check health
        QTimer::singleShot(2000, this, &BackendManager::checkHealth);
    } else {
        Logger::logError("Failed to start backend process", m_backendProcess->errorString());
        emit apiError("Failed to start backend: " + m_backendProcess->errorString());
    }
}

void BackendManager::stopBackend()
{
    if (m_backendProcess->state() == QProcess::NotRunning) {
        return;
    }
    
    Logger::logInfo("Stopping backend process");
    m_backendProcess->terminate();
    
    if (!m_backendProcess->waitForFinished(3000)) {
        m_backendProcess->kill();
        m_backendProcess->waitForFinished(1000);
    }
    
    m_backendRunning = false;
    emit backendRunningChanged(false);
    emit backendStopped();
}

void BackendManager::onBackendProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    m_backendRunning = false;
    emit backendRunningChanged(false);
    emit backendStopped();
    
    if (exitStatus == QProcess::CrashExit) {
        Logger::logError("Backend process crashed", QString("Exit code: %1").arg(exitCode));
    } else {
        Logger::logInfo("Backend process finished", QString("Exit code: %1").arg(exitCode));
    }
}

void BackendManager::onBackendProcessError(QProcess::ProcessError error)
{
    Logger::logError("Backend process error", QString::number(error));
    m_backendRunning = false;
    emit backendRunningChanged(false);
}

void BackendManager::postRequest(const QString &endpoint, const QJsonObject &data)
{
    if (!m_backendRunning) {
        emit apiError("Backend is not running");
        return;
    }
    
    QUrl url = buildUrl(endpoint);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    
    QJsonDocument doc(data);
    QByteArray postData = doc.toJson();
    
    m_lastRequestEndpoint = endpoint;
    QNetworkReply *reply = m_networkManager->post(request, postData);
    
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonParseError error;
            QJsonDocument doc = QJsonDocument::fromJson(responseData, &error);
            
            if (error.error == QJsonParseError::NoError && doc.isObject()) {
                emit apiResponse(endpoint, doc.object());
            } else {
                QString errorMsg = "Invalid JSON response";
                emit apiError(errorMsg);
            }
        } else {
            QString errorMsg = reply->errorString();
            emit apiError(errorMsg);
        }
        reply->deleteLater();
    });
}

void BackendManager::getRequest(const QString &endpoint)
{
    if (!m_backendRunning) {
        emit apiError("Backend is not running");
        return;
    }
    
    QUrl url = buildUrl(endpoint);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    
    m_lastRequestEndpoint = endpoint;
    QNetworkReply *reply = m_networkManager->get(request);
    
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonParseError error;
            QJsonDocument doc = QJsonDocument::fromJson(responseData, &error);
            
            if (error.error == QJsonParseError::NoError && doc.isObject()) {
                emit apiResponse(endpoint, doc.object());
            } else {
                QString errorMsg = "Invalid JSON response";
                emit apiError(errorMsg);
            }
        } else {
            QString errorMsg = reply->errorString();
            emit apiError(errorMsg);
        }
        reply->deleteLater();
    });
}

void BackendManager::putRequest(const QString &endpoint, const QJsonObject &data)
{
    if (!m_backendRunning) {
        emit apiError("Backend is not running");
        return;
    }
    
    QUrl url = buildUrl(endpoint);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    
    QJsonDocument doc(data);
    QByteArray putData = doc.toJson();
    
    m_lastRequestEndpoint = endpoint;
    QNetworkReply *reply = m_networkManager->sendCustomRequest(request, "PUT", putData);
    
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonParseError error;
            QJsonDocument doc = QJsonDocument::fromJson(responseData, &error);
            
            if (error.error == QJsonParseError::NoError && doc.isObject()) {
                emit apiResponse(endpoint, doc.object());
            } else {
                QString errorMsg = "Invalid JSON response";
                emit apiError(errorMsg);
            }
        } else {
            QString errorMsg = reply->errorString();
            emit apiError(errorMsg);
        }
        reply->deleteLater();
    });
}

void BackendManager::deleteRequest(const QString &endpoint)
{
    if (!m_backendRunning) {
        emit apiError("Backend is not running");
        return;
    }
    
    QUrl url = buildUrl(endpoint);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    
    m_lastRequestEndpoint = endpoint;
    QNetworkReply *reply = m_networkManager->sendCustomRequest(request, "DELETE");
    
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonParseError error;
            QJsonDocument doc = QJsonDocument::fromJson(responseData, &error);
            
            if (error.error == QJsonParseError::NoError && doc.isObject()) {
                emit apiResponse(endpoint, doc.object());
            } else {
                QString errorMsg = "Invalid JSON response";
                emit apiError(errorMsg);
            }
        } else {
            QString errorMsg = reply->errorString();
            emit apiError(errorMsg);
        }
        reply->deleteLater();
    });
}

void BackendManager::login(const QString &username, const QString &password)
{
    QJsonObject data;
    data["username"] = username;
    data["password"] = password;
    
    QString endpoint = "/api/v1/salamapos/auth/login";
    m_lastRequestEndpoint = endpoint;
    
    if (!m_backendRunning) {
        emit apiError("Backend is not running");
        return;
    }
    
    QUrl url = buildUrl(endpoint);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    
    QJsonDocument doc(data);
    QByteArray postData = doc.toJson();
    
    QNetworkReply *reply = m_networkManager->post(request, postData);
    
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonParseError error;
            QJsonDocument doc = QJsonDocument::fromJson(responseData, &error);
            
            if (error.error == QJsonParseError::NoError && doc.isObject()) {
                emit apiLoginResponse(doc.object());
            } else {
                QString errorMsg = "Invalid JSON response";
                emit apiError(errorMsg);
            }
        } else {
            QString errorMsg = reply->errorString();
            emit apiError(errorMsg);
        }
        reply->deleteLater();
    });
}

