#include "pingserverprocess.h"
#include "logger.h"

PingServerProcess::PingServerProcess(QObject *parent) : QObject(parent)
{
    m_pingUrl = "8.8.8.8";

    startPingCheck();
}

void PingServerProcess::startPingCheck(const int &pingIntervalInSecs)
{
    // Listen for messages from the ping process
    connect(&m_pingProcess, SIGNAL(readyRead()), this, SLOT(onPingFinished()));
    connect(&m_pingProcess, &QProcess::errorOccurred, this, [=](){
        Logger::logError("Error occurred when pinging");
    });
    // Connect the timer to the exec method that actually calls the ping process
    m_pingTimer.setInterval(pingIntervalInSecs * 1000);
    m_pingTimer.setSingleShot(false);
    connect(&m_pingTimer, SIGNAL(timeout()), this, SLOT(onStartPing()));
    m_pingTimer.start();
}

void PingServerProcess::onStartPing()
{
    if(isWaitingConnection)
    {
        isConnected=false;
        emit connectionStatusChanged(isConnected);
        // qDebug() << "No iternet Connection";
    } else {
        isWaitingConnection=true;
    }

    if(m_pingProcess.processId()==0)
    {
        QStringList arguments;
        arguments << "-n" << m_pingUrl << "-i" << "5" << "-w" << "1000";
        m_pingProcess.start("ping", arguments);
        // Debug() << "Ping Starting Status PID: " << m_pingProcess.pid();
    }

    else
    {
        // qDebug() << "Ping Running Status PID: " << m_pingProcess.pid();
    }

}

void PingServerProcess::onPingFinished()
{
    QByteArray response = m_pingProcess.readAll();
    QString responseStr(response);

    // qDebug() << "Ping Response: " << responseStr;

    if(!responseStr.contains("Network is unreachable"))
    {
        isConnected=true;
        isWaitingConnection=false;

        emit connectionStatusChanged(isConnected);
        // qDebug() << "Network Available";
    }

    if(responseStr.contains("100% loss"))
    {
        Logger::logWarning(QString("Ping failed: %1 is down").arg(m_pingUrl));
        emit pingFailed();
    }
}
