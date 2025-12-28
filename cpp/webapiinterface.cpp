#include "webapiinterface.h"

WebApiInterface::WebApiInterface(QObject *parent) : QObject(parent)
{
    checkConnectivityTimer = new QTimer(this);
    // checkConnectivityTimer->setInterval(10000);
    // checkConnectivityTimer->start();
    // connect(checkConnectivityTimer, &QTimer::timeout, this, &WebApiInterface::onCheckConnectivityTimerTimeout);

}

void WebApiInterface::onCheckForUpdates(const int &cVersion)
{
    // Q_UNUSED(cVersion)

    connect(&m_networkManager, &QNetworkAccessManager::finished,
            this, [=](QNetworkReply* pReply){

        auto replyData = pReply->readAll();
        QJsonDocument document = QJsonDocument::fromJson(replyData.data());
        QJsonObject rootObj = document.object();

        // qDebug() << "Received Data: " << document;

        if(cVersion < rootObj.value("version").toInt())
        {
            qDebug() << ">> New version is available";
            emit newVersionAvailable(rootObj);
        }

        // emit a signal on getting full data
        pReply->deleteLater();


    });

    // QUrl url("https://bjgs657959.execute-api.us-east-2.amazonaws.com/default/checkUpdatesSalamaPOS");
    // QNetworkRequest request(url);
    // m_networkManager.get(request);
}

void WebApiInterface::onDownloadUpdate()
{

}

void WebApiInterface::onInstallUpdate()
{

}

void WebApiInterface::onWebRunnableFinished(const QString &str)
{
    Q_UNUSED(str)
}

void WebApiInterface::connect2Web(const QString &state, const QJsonObject &data)
{
    //    if(!m_isOnline)
    //    {
    //        m_isOnline = true;
    //        emit isOnlineChanged(true);
    //    }

    Q_UNUSED(state)
    Q_UNUSED(data)

    if(m_ThreadPool.activeThreadCount()==0) // && !pendingWebJob)
    {
        // pendingWebJob = true;

        // m_WebInterface->setValues( state, data);

        // m_ThreadPool.start(m_WebInterface);
    }
}

void WebApiInterface::onCheckConnectivityTimerTimeout()
{
    if(!hasPendingWebRequestForConnectivity)
    {
        hasPendingWebRequestForConnectivity = true;

        connect(&m_networkManager, &QNetworkAccessManager::finished,
                this, [=](QNetworkReply* pReply){

            auto replyData = pReply->readAll();
            auto data = replyData.data();
            QJsonDocument document = QJsonDocument::fromJson(data);
            // QJsonObject rootObj = document.object();

            // qDebug() << "Received Data: " << document;
            // qDebug() << "Data: " << data;

            pReply->deleteLater();

            hasPendingWebRequestForConnectivity = false;
        });

        // QUrl url("https://www.google.com/");
        // QNetworkRequest request(url);
        // m_networkManager.get(request);
    }
}

WebInterfaceRunnable::WebInterfaceRunnable(QObject *parent):QObject(parent)
{
    setAutoDelete(false);
}

void WebInterfaceRunnable::setValues(const QString &state, const QJsonObject &data)
{
    // m_url="https://webhooks.mongodb-realm.com/api/client/v2.0/app/salamapos-bjgmz/service/salamaPOS/incoming_webhook/WebHook-SalamaPOS";
    // m_data = data;
    // m_state = state;
}

void WebInterfaceRunnable::run()
{
    return;

    // TODO check on this block
    QNetworkAccessManager networkManager;
    QNetworkRequest request(m_url);

    QJsonDocument subJson(m_data["content"].toObject());

    QUrlQuery query;
    query.addQueryItem("state", m_state);
    query.addQueryItem("content", subJson.toJson());

    QByteArray postData = query.toString().toUtf8(); // doc.toJson(); // query.toString().toUtf8();

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QEventLoop loop;

    QTimer timer;

    timer.connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);

    networkManager.connect(&networkManager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    networkManager.connect(&networkManager, &QNetworkAccessManager::sslErrors, this, [=](){
        qDebug() << "SSL Errors!";
    });

    QNetworkReply * reply = networkManager.post(request, postData);

    timer.start(8000);

    loop.exec();

    if(timer.isActive())
    {
        timer.stop();

        if(reply)
        {
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

            // qDebug() << ".. Reply Code: " << statusCode;

            if(reply->error() == QNetworkReply::NoError)
            {
                if(statusCode != 200)
                {
                    // qDebug() << "Bad response";

                    emit finished("Error");
                }
                else
                {
                    QString replyStr=reply->readAll();

                    // qDebug() << "Reply String: " << replyStr;

                    if(replyStr.isEmpty()||replyStr.isNull())
                    {
                        emit finished("Error");
                    }
                    else
                    {
                        emit finished(replyStr);
                    }
                }
            }
            else
            {
                emit finished("Error");
            }

            reply->deleteLater();
        }
        else
        {
            qDebug() << ">> No reply";

            emit finished("Error");
        }
    }

    else
    {
        emit finished("Error");

        qDebug() << "Timer not started";
    }
}
