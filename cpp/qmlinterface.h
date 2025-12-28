#ifndef QMLINTERFACE_H
#define QMLINTERFACE_H

#include <QObject>
#include <QDebug>
#include <QSqlDatabase>
#include <QJsonObject>
#include <QJsonDocument>
#include <QRect>
#include <QIcon>
#include <QScreen>
#include <QFile>
#include <QIODevice>
#include <fstream>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QGuiApplication>
#include <QSettings>
#include <QNetworkInterface>
#include <QStandardPaths>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTextDocument>
#include <QTextStream>
#include <QPrinter>
#include <QDir>
#include <QStandardPaths>

#include "databaseinterface.h"
#include "serialportinterface.h"
#include "webapiinterface.h"
#include "websocketsinterface.h"
#include "datetime.h"
#include "pingserverprocess.h"
// #include "WebInterfaceRunnable.h"

class QmlInterface : public QObject
{
    Q_OBJECT

public:
    explicit QmlInterface(QObject *parent = nullptr);

    ~QmlInterface();

    Q_PROPERTY(bool isDarkTheme READ isDarkTheme WRITE setIsDarkTheme NOTIFY isDarkThemeChanged)
    Q_PROPERTY(bool isFirstTimeUse READ isFirstTimeUse WRITE setIsFirstTimeUse NOTIFY isFirstTimeUseChanged)
    Q_PROPERTY(int dueSoonReminders READ dueSoonReminders WRITE setDueSoonReminders NOTIFY dueSoonRemindersChanged)
    Q_PROPERTY(int overdueReminders READ overdueReminders WRITE setOverdueReminders NOTIFY overdueRemindersChanged)
    Q_PROPERTY(int solvedReminders READ solvedReminders WRITE setSolvedReminders NOTIFY solvedRemindersChanged)
    Q_PROPERTY(int salesNumbers READ salesNumbers WRITE setSalesNumbers NOTIFY salesNumbersChanged)
    Q_PROPERTY(int salesCost READ salesCost WRITE setSalesCost NOTIFY salesCostChanged)
    Q_PROPERTY(int sentMessages READ sentMessages WRITE setSentMessages NOTIFY sentMessagesChanged)
    Q_PROPERTY(int receivedMessages READ receivedMessages WRITE setReceivedMessages NOTIFY receivedMessagesChanged)
    Q_PROPERTY(int unreadMessages READ unreadMessages WRITE setUnreadMessages NOTIFY unreadMessagesChanged)
    Q_PROPERTY(QStringList plotXAxis READ plotXAxis WRITE setPlotXAxis NOTIFY plotXAxisChanged)
    Q_PROPERTY(QList<int> cashYAxis READ cashYAxis WRITE setCashYAxis NOTIFY cashYAxisChanged)
    Q_PROPERTY(QList<int> mpesaYAxis READ mpesaYAxis WRITE setMpesaYAxis NOTIFY mpesaYAxisChanged)
    Q_PROPERTY(QList<int> creditYAxis READ creditYAxis WRITE setCreditYAxis NOTIFY creditYAxisChanged)
    Q_PROPERTY(QList<int> chequeYAxis READ chequeYAxis WRITE setChequeYAxis NOTIFY chequeYAxisChanged)
    Q_PROPERTY(QList<int> creditPaidYAxis READ creditPaidYAxis WRITE setCreditPaidYAxis NOTIFY creditPaidYAxisChanged)
    Q_PROPERTY(int plotYmax READ plotYmax WRITE setPlotYmax NOTIFY plotYmaxChanged)

    // Configuration properties
    Q_PROPERTY(bool tablesCreated READ tablesCreated WRITE setTablesCreated NOTIFY tablesCreatedChanged)
    Q_PROPERTY(bool productsAdded READ productsAdded WRITE setProductsAdded NOTIFY productsAddedChanged)
    Q_PROPERTY(bool productTypeAdded READ productTypeAdded WRITE setProductTypeAdded NOTIFY productTypeAddedChanged)
    Q_PROPERTY(bool productStockAdded READ productStockAdded WRITE setProductStockAdded NOTIFY productStockAddedChanged)
    Q_PROPERTY(int versionInt READ versionInt WRITE setVersionInt NOTIFY versionIntChanged)
    Q_PROPERTY(std::string logFileName READ logFileName WRITE setLogFileName NOTIFY logFileNameChanged)
    Q_PROPERTY(bool databaseLoaded READ databaseLoaded WRITE setDatabaseLoaded NOTIFY databaseLoadedChanged)
    Q_PROPERTY(QString databaseConnectionErrorString READ databaseConnectionErrorString WRITE setDatabaseConnectionErrorString NOTIFY databaseConnectionErrorStringChanged)
    Q_PROPERTY(bool isInternetConnected READ isInternetConnected WRITE setIsInternetConnected NOTIFY isInternetConnectedChanged)

    Q_INVOKABLE QJsonObject getScreenSize();
    Q_INVOKABLE void fetchSavedSettings();
    Q_INVOKABLE void getSalesStatisticsForDashboard();
    Q_INVOKABLE void getMessagesStatisticsForDashboard(const QString &uname);
    Q_INVOKABLE void getRemindersStatisticsForDashboard();
    Q_INVOKABLE void getDashboardTableData();
    Q_INVOKABLE void downloadUpdate();
    Q_INVOKABLE void check4Update();
    Q_INVOKABLE void getSalesSummary(const int &ind);
    Q_INVOKABLE void installUpdate();
    Q_INVOKABLE void logToFile(const QString &level, const QString &log);
    Q_INVOKABLE void openLocation(const QString &path);
    // void logToFile(QString level, QString log);

    void setTabularData();

    bool isDarkTheme() const;

    bool isFirstTimeUse() const;

    int dueSoonReminders() const;

    int overdueReminders() const;

    int solvedReminders() const;

    int salesNumbers() const;

    int salesCost() const;

    int sentMessages() const;

    int receivedMessages() const;

    int unreadMessages() const;

    QStringList plotXAxis() const;

    QList<int> cashYAxis() const;

    QList<int> mpesaYAxis() const;

    QList<int> creditYAxis() const;

    QList<int> chequeYAxis() const;

    int plotYmax() const;

    bool tablesCreated() const;

    bool productsAdded() const;

    bool productTypeAdded() const;

    bool productStockAdded() const;

    int versionInt() const;

    void emitDatabaseState(const bool &state, const QString &status);

    std::string logFileName() const;

    bool databaseLoaded() const;

    QString databaseConnectionErrorString() const;

    bool isInternetConnected() const;

    QList<int> creditPaidYAxis() const;

public slots:
    void setIsDarkTheme(bool isDarkTheme);

    void setIsFirstTimeUse(bool isFirstTimeUse);

    void setDueSoonReminders(int dueSoonReminders);

    void setOverdueReminders(int overdueReminders);

    void setSolvedReminders(int solvedReminders);

    void setSalesNumbers(int salesNumbers);

    void setSalesCost(int salesCost);

    void setSentMessages(int setMessages);

    void setReceivedMessages(int receivedMessages);

    void setUnreadMessages(int unreadMessages);

    void setPlotXAxis(QStringList plotXAxis);

    void setCashYAxis(QList<int> cashYAxis);

    void setMpesaYAxis(QList<int> mpesaYAxis);

    void setCreditYAxis(QList<int> creditYAxis);

    void setChequeYAxis(QList<int> chequeYAxis);

    void setPlotYmax(int plotYmax);

    void setTablesCreated(bool tablesCreated);

    void setProductsAdded(bool productsAdded);

    void setProductTypeAdded(bool productTypeAdded);

    void setProductStockAdded(bool productStockAdded);

    void setVersionInt(int versionInt);

    //
    void onNewVersionAvailable(const QJsonObject &json);

    void setLogFileName(std::string logFileName);

    void setDatabaseLoaded(bool databaseLoaded);

    void setDatabaseConnectionErrorString(QString databaseConnectionErrorString);

    void setIsInternetConnected(bool isInternetConnected);

    void setCreditPaidYAxis(QList<int> creditPaidYAxis);

signals:
    void databaseReadyChanged(bool state, QString msg);

    void isDarkThemeChanged(bool isDarkTheme);

    void isFirstTimeUseChanged(bool isFirstTimeUse);

    void dueSoonRemindersChanged(int dueSoonReminders);

    void overdueRemindersChanged(int overdueReminders);

    void solvedRemindersChanged(int solvedReminders);

    void salesNumbersChanged(int salesNumbers);

    void salesCostChanged(int salesCost);

    void sentMessagesChanged(int setMessages);

    void receivedMessagesChanged(int receivedMessages);

    void unreadMessagesChanged(int unreadMessages);

    void plotXAxisChanged(QStringList plotXAxis);

    void cashYAxisChanged(QList<int> cashYAxis);

    void mpesaYAxisChanged(QList<int> mpesaYAxis);

    void creditYAxisChanged(QList<int> creditYAxis);

    void chequeYAxisChanged(QList<int> chequeYAxis);

    void dataReadyForPlotting();

    void plotYmaxChanged(int plotYmax);

    void tablesCreatedChanged(bool tablesCreated);

    void productsAddedChanged(bool productsAdded);

    void productTypeAddedChanged(bool productTypeAdded);

    void productStockAddedChanged(bool productStockAdded);

    void versionIntChanged(int versionInt);

    void started(int version);

    //
    void newVersionAvailableChanged(QVariant ver);
    void updateProgressChanged(int value);
    void downloadFinished(QString path);
    void downloadStarted();
    void salesSummaryCost(int cash,int mpesa,int cheque,int credit,int paid,int totals);

    void logFileNameChanged(std::string logFileName);
    void databaseConnectionChanged(bool state, QString msg);

    void databaseLoadedChanged(bool databaseLoaded);

    void databaseConnectionErrorStringChanged(QString databaseConnectionErrorString);
    void updateBarPlotsChanged();

    void isInternetConnectedChanged(bool isInternetConnected);

    void creditPaidYAxisChanged(QList<int> creditPaidYAxis);

private slots:
    void onLogsTimerTimeout();

    void onInternetConnectionStatusChanged(bool state);
    
    void onDatabaseConnectionError(const QString &errorMessage);

private:
    void initializeLogFileName();

    DatabaseInterface * m_databaseInterface;
    DateTime * m_dateTime;
    WebApiInterface * webInt;
    WebInterfaceRunnable * m_WebInterface;
    QSettings * m_settings;
    PingServerProcess *pingServer;

    bool m_isDarkTheme, m_isFirstTimeUse;
    int m_dueSoonReminders, m_overdueReminders, m_solvedReminders, m_salesNumbers, m_salesCost, m_setMessages, m_receivedMessages, m_unreadMessages;
    QStringList m_plotXAxis;
    QList<int> m_cashYAxis, m_mpesaYAxis, m_creditYAxis, m_chequeYAxis;
    int m_plotYmax, m_versionInt;
    bool m_tablesCreated, m_productsAdded, m_productTypeAdded, m_productStockAdded, m_logFileDayChanged = false;
    QJsonObject m_UpdateJSON;
    QString m_path, r_path, m_logsPath;
    std::string m_logFileName;

    std::ofstream logWriter;
    QTimer * logsTimer;
    bool m_databaseLoaded;
    QString m_databaseConnectionErrorString;
    bool m_isInternetConnected;
    QDir d;
    QList<int> m_creditPaidYAxis;
};

#endif // QMLINTERFACE_H
