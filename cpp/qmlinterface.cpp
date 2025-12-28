#include "qmlinterface.h"
#include <QDir>
#include <QFileInfo>
#include <QDesktopServices>
#include <QMessageBox>

QmlInterface::QmlInterface(QObject *parent) : QObject(parent)
{
    setDatabaseLoaded(false);
    setDatabaseConnectionErrorString("No connection made yet!");
    setIsInternetConnected(false);

    // Ensure logs directory has been created
    QString logsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    d.mkpath(logsPath+"/SalamaPOS/logs");
    m_logsPath = logsPath+"/SalamaPOS/logs";
    logToFile("INFO", "\n\n----------------------------------------\nStarting SalamaPOS Application\n----------------------------------------\n\n");

    QString versionString = "21.01.01";

    // Set App Company Details
    qApp->setApplicationName("Salama P.O.S.");
    qApp->setApplicationVersion(versionString);
    qApp->setApplicationDisplayName("Salama P.O.S. v21.01.01");
    qApp->setOrganizationName("lalanke");
    qApp->setWindowIcon(QIcon(":/assets/images/6.png"));
    qApp->setOrganizationDomain("lalanke.com");

    m_settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(),qApp->applicationDisplayName());

    // Retrieve settings
    setIsDarkTheme(m_settings->value("Theme/isDarkTheme", false).toBool());
    setVersionInt(m_settings->value("General/version", 3).toInt());

    // Initialize database
    m_databaseInterface = new DatabaseInterface(parent);
    m_WebInterface = new WebInterfaceRunnable(this);
    m_dateTime = new DateTime();
    webInt = new WebApiInterface();
    pingServer = new PingServerProcess(this);

    connect(this, &QmlInterface::started, webInt, &WebApiInterface::onCheckForUpdates);
    connect(webInt, &WebApiInterface::newVersionAvailable, this, &QmlInterface::onNewVersionAvailable);
    connect(m_WebInterface, &WebInterfaceRunnable::finished, webInt, &WebApiInterface::onWebRunnableFinished);
    connect(pingServer, &PingServerProcess::connectionStatusChanged, this, &QmlInterface::onInternetConnectionStatusChanged);
    connect(m_databaseInterface, &DatabaseInterface::connectionError, this, &QmlInterface::onDatabaseConnectionError);

    QString status = m_databaseInterface->initializeDatabase();

    if(status.split(":").at(0) == "true")
    {
        qDebug() << "Database Creation OK";
        emitDatabaseState(true, "");
        logToFile("INFO", "DatabaseInterface::initializeDatabase() => Database Connection Successful");
        setTabularData();
    }

    else
    {
        QString errorMsg = status.split(":").at(1);
        logToFile("FATAL", "DatabaseInterface::initializeDatabase() => Could not create a database Connection ==> " + errorMsg);
        emitDatabaseState(false, errorMsg);
        // Error dialog will be shown via connectionError signal
    }

    // Set default max
    setPlotYmax(10);
    setVersionInt(2);

    m_path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    d.mkpath(m_path+"/downloads");
}

QmlInterface::~QmlInterface()
{
    logToFile("INFO", "\n\n----------------------------------------\nEnding SalamaPOS Application Session\n----------------------------------------\n\n");

    m_databaseInterface->deleteLater();
    m_dateTime->deleteLater();
    webInt->deleteLater();
    m_settings->deleteLater();
    m_WebInterface->deleteLater();
    // logsTimer->deleteLater();

    if(logWriter.is_open())
        logWriter.close();

    qDebug() << "Exitting from QmlInterface Destructor!";
}

void QmlInterface::emitDatabaseState(const bool &state, const QString &status)
{
    qDebug() << "Database Initialization: State = " << state;

    emit databaseReadyChanged(state, status);

    setDatabaseLoaded(state);

    setDatabaseConnectionErrorString(status);
}

std::string QmlInterface::logFileName() const
{
    return m_logFileName;
}

bool QmlInterface::databaseLoaded() const
{
    return m_databaseLoaded;
}

QString QmlInterface::databaseConnectionErrorString() const
{
    return m_databaseConnectionErrorString;
}

bool QmlInterface::isInternetConnected() const
{
    return m_isInternetConnected;
}

QList<int> QmlInterface::creditPaidYAxis() const
{
    return m_creditPaidYAxis;
}

QJsonObject QmlInterface::getScreenSize()
{
    QJsonDocument doc = QJsonDocument::fromVariant("{ \"width\":0, \"height\":0}");
    QJsonObject size = doc.object();

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    int height = screenGeometry.height();
    int width = screenGeometry.width();

    size["width"] = width;
    size["height"] = height;

    // DONT DELETE POINTER, application will crash
    // screen->deleteLater();

    // logToFile("INFO", "Screen Dimensions At Query Time: "+QString::number(width)+"x"+QString::number(height));

    return size;
}

void QmlInterface::fetchSavedSettings()
{
    logToFile("INFO", "Starting QmlInterface::fetchSavedSettings()");

    setIsDarkTheme(m_settings->value("Theme/isDarkTheme", true).toBool());
    setTablesCreated(m_settings->value("Config/isTablesCreated", false).toBool());
    setProductsAdded(m_settings->value("Config/isProductsAdded", false).toBool());
    setProductTypeAdded(m_settings->value("Config/isProductTypeAdded", false).toBool());
    setProductStockAdded(m_settings->value("Config/isProductStockAdded", false).toBool());

    logToFile("INFO", "Ending QmlInterface::fetchSavedSettings()");
}

void QmlInterface::getSalesStatisticsForDashboard()
{
    logToFile("INFO", "Starting QmlInterface::getSalesStatisticsForDashboard()");

    // Get the sales quantities and cost for the day!
    QSqlDatabase db = QSqlDatabase::database();
    // qDebug() << " >> Fetching Sales Statistics";

    if(db.isOpen())
    {
        QSqlQuery query;
        auto dateToday = m_dateTime->getTimestamp("today");
        QString sql = "SELECT product_sp,sale_qty from sales WHERE sales_date BETWEEN :startDate AND :endDate";

        int counter = 0, totals = 0;

        query.prepare(sql);
        query.bindValue(":startDate", dateToday.at(0));
        query.bindValue(":endDate", dateToday.at(1));

        if(query.exec())
        {
            while(query.next())
            {
                int sp = query.value(0).toInt();
                int qty = query.value(1).toInt();

                totals += sp * qty;
                counter += qty;
            }

            setSalesCost(totals);
            setSalesNumbers(counter);
        }

        else
        {
            logToFile("CRITICAL", "Error fetchin data from the database: [" + query.executedQuery() + "] -> " + query.lastError().text());
        }
    }


    logToFile("INFO", "Ending QmlInterface::getSalesStatisticsForDashboard()");
}

void QmlInterface::getMessagesStatisticsForDashboard(const QString &uname)
{
    logToFile("INFO", "Starting QmlInterface::getMessagesStatisticsForDashboard()");

    // Get messages (sent, received, pending) for the current logged user
    QSqlDatabase db = QSqlDatabase::database();

    // TODO clean this up, remove unnecessary queries and use prepared statements
    if(db.isOpen())
    {
        QSqlQuery query;
        auto dateToday = m_dateTime->getTimestamp("today");
        QString sql = "SELECT has_read from messages WHERE receiver = :receiver";
        QString sql2 = "SELECT * from messages WHERE sender = :sender";

        int sent = 0, received = 0, pending = 0;

        query.prepare(sql);
        query.bindValue(":receiver", uname);

        if(query.exec())
        {
            QSqlQuery query2;
            query2.prepare(sql2);
            query2.bindValue(":sender", uname);

            if(query2.exec())
            {
                while(query2.next())
                {
                    int hasRead = query2.value(0).toBool();

                    if(!hasRead)
                        pending += 1;

                    received += 1;
                }
            }
            {
                int hasRead = query.value(0).toBool();

                if(!hasRead)
                    pending += 1;

                received += 1;
            }

            setReceivedMessages(received);
            setUnreadMessages(pending);

            query.prepare(sql2);
            query.bindValue(":sender", uname);

            if(query.exec())
            {
                while(query.next())
                {
                    sent += 1;
                }

                setSentMessages(sent);
            }

            else
            {
                logToFile("CRITICAL", "Error fetching data from the database: [" + query.executedQuery() + "] -> " + query.lastError().text());
            }
        }
    }

    logToFile("INFO", "Ending QmlInterface::getMessagesStatisticsForDashboard()");
}

void QmlInterface::getRemindersStatisticsForDashboard()
{
    logToFile("INFO", "Starting QmlInterface::getRemindersStatisticsForDashboard()");

    // Get reminder & notification numbers for the current logged in user
    QSqlDatabase db = QSqlDatabase::database();

    if(db.isOpen())
    {
        QSqlQuery query;
        auto dateToday = m_dateTime->getTimestamp("now");
        QString sql = "SELECT solved from notifications WHERE notify_date < :notifyDate";
        QString sql2 = "SELECT * from notifications WHERE notify_date > :notifyDate";

        int due = 0, overdue = 0, solved = 0;

        query.prepare(sql);
        query.bindValue(":notifyDate", dateToday.at(0));

        if(query.exec())
        {
            while(query.next())
            {
                int isSolved = query.value(0).toBool();

                if(isSolved)
                    solved += 1;

                else
                    overdue += 1;
            }

            setOverdueReminders(overdue);
            setSolvedReminders(solved);

            QSqlQuery query2;
            query2.prepare(sql2);
            query2.bindValue(":notifyDate", dateToday.at(0));

            if(query2.exec())
            {
                while(query2.next())
                {
                    due += 1;
                }

                setDueSoonReminders(due);

                // qDebug() << "Due: " << due << "\tOverdue: " << overdue << "\tSolved: " << solved;
            }

            else
            {
                logToFile("CRITICAL", "Error fetchin data from the database: [" + query.executedQuery() + "] -> " + query.lastError().text());
            }
        }

        else
        {
            logToFile("CRITICAL", "Error fetchin data from the database: [" + query.executedQuery() + "] -> " + query.lastError().text());
        }
    }

    logToFile("INFO", "Ending QmlInterface::getRemindersStatisticsForDashboard()");
}

void QmlInterface::getDashboardTableData()
{
    logToFile("INFO", "Starting QmlInterface::getDashboardTableData()");

    QDateTime dt = QDateTime::currentDateTime();
    QStringList plot;
    QSqlDatabase db = QSqlDatabase::database();

    if(db.isOpen())
    {
        QList<int> cash, mpesa, credit, cheque, credit_paid;
        QString dt_str;

        for(int i=6; i>=0; i--)
        {
            int _cash = 0, _mpesa = 0, _cheque = 0, _credit = 0, _credit_paid=0;

            if(i==1)
            {
                dt_str = "Yesterday";
                plot.append(dt_str);
            }

            else if(i == 0)
            {
                dt_str = "Today";
                plot.append(dt_str);
            }

            else
            {
                auto n_dt = dt.addDays(-i);
                dt_str = n_dt.toString("ddd MMM, yyyy");
                plot.append(dt_str);
            }

            auto n_dt = dt.addDays(-i);
            QString end = n_dt.toString("yyyy-MM-dd ") + "23:59:59+03";
            QString start = n_dt.toString("yyyy-MM-dd ") + "00:00:00+03";

            QString sql = "SELECT cash, mpesa, cheque, credit FROM payment INNER JOIN sales ON payment.sales_id=sales.sales_id WHERE sales_date BETWEEN :startDate AND :endDate";
            QString sql0 = "SELECT payment_amount FROM credit_payments WHERE payment_timestamp BETWEEN :startDate AND :endDate";
            
            QSqlQuery query, query0;
            query.prepare(sql);
            query.bindValue(":startDate", start);
            query.bindValue(":endDate", end);

            query0.prepare(sql0);
            query0.bindValue(":startDate", start);
            query0.bindValue(":endDate", end);

            if(query.exec())
            {
                // qDebug() << ":- Loaded ";
                while(query.next())
                {
                    _cash += query.value(0).toInt();
                    _mpesa += query.value(1).toInt();
                    _cheque += query.value(2).toInt();
                    _credit += query.value(3).toInt();

                    // qDebug() << "Cash: " << _cash << "\tMpesa: " << _mpesa << "\tCheque: " << _cheque << "\tCredit: " << _credit;
                }

                if(query0.exec())
                {
                    while(query0.next())
                    {
                        _credit_paid += query0.value(0).toInt();
                    }

                } else {
                    logToFile("CRITICAL", "QmlInterface::getDashboardTableData() => Couldn't load re-payment_history data from db :" + query.lastError().text());
                }

            } else {
                logToFile("CRITICAL", "QmlInterface::getDashboardTableData() => Couldn't load payment data from db :" + query.lastError().text());
            }

            m_plotXAxis.clear();
            setPlotXAxis(plot);

            cash.append(_cash);
            mpesa.append(_mpesa);
            cheque.append(_cheque);
            credit.append(_credit);
            credit_paid.append(_credit_paid);

            qDebug() << "\n" << dt_str << " Statistics: ";
            qDebug() << "\tCash Sales: " << _cash;
            qDebug() << "\tMpesa Sales: " << _mpesa;
            qDebug() << "\tCheque Sales: " << _cheque;
            qDebug() << "\tCredit Taken: " << _credit;
            qDebug() << "\tCredit Paid: " << _credit_paid;

        }

        m_cashYAxis.clear();
        m_mpesaYAxis.clear();
        m_creditYAxis.clear();
        m_chequeYAxis.clear();
        m_creditPaidYAxis.clear();
        setPlotYmax(10);

        setCashYAxis(cash);
        setCreditYAxis(credit);
        setChequeYAxis(cheque);
        setMpesaYAxis(mpesa);
        setCreditPaidYAxis(credit_paid);

        std::sort(cash.begin(), cash.end());
        std::sort(mpesa.begin(), mpesa.end());
        std::sort(cheque.begin(), cheque.end());
        std::sort(credit.begin(), credit.end());
        std::sort(credit_paid.begin(), credit_paid.end());

        if(cash.at(cash.size() - 1) > plotYmax())
            setPlotYmax(cash.at(cash.size() - 1) + 10);

        if(mpesa.at(mpesa.size() - 1) > plotYmax())
            setPlotYmax(mpesa.at(mpesa.size() - 1) + 10);

        if(cheque.at(cheque.size() - 1) > plotYmax())
            setPlotYmax(cheque.at(cheque.size() - 1) + 10);

        if(credit.at(credit.size() - 1) > plotYmax())
            setPlotYmax(credit.at(credit.size() - 1) + 10);

        if(credit_paid.at(credit_paid.size() - 1) > plotYmax())
            setPlotYmax(credit_paid.at(credit_paid.size() - 1) + 10);

        emit dataReadyForPlotting();
    }

    getSalesStatisticsForDashboard();

    logToFile("INFO", "Ending QmlInterface::getDashboardTableData()");
}

void QmlInterface::onNewVersionAvailable(const QJsonObject &json)
{

    m_UpdateJSON = json;

    // Validate the filename
    if(m_UpdateJSON.value("filename").toString().isEmpty())
    {
        logToFile("CRITICAL", "QmlInterface::onNewVersionAvailable() => Filename is empty!");
        return;
    }

    // Validate the path
    if(m_path.isEmpty())
    {
        logToFile("CRITICAL", "QmlInterface::onNewVersionAvailable() => Path is empty!");
        return;
    }

    // Validate filename is valid, does not end up leaving the base directory
    if(m_UpdateJSON.value("filename").toString().contains(".."))
    {
        logToFile("CRITICAL", "QmlInterface::onNewVersionAvailable() => Filename is invalid!");
        return;
    }

    r_path = m_path+"/downloads/" + m_UpdateJSON.value("filename").toString();

    if(QFileInfo::exists(r_path))
    {
        emit downloadFinished(r_path);
        qDebug() << ">> File already downloaded, ready to install";
    }

    else
    {
        emit newVersionAvailableChanged(json.value("versionString").toString());
        qDebug() << ">> Update ready, start download";
    }
}

void QmlInterface::setLogFileName(std::string logFileName)
{
    if (m_logFileName == logFileName)
        return;

    m_logFileDayChanged = true;
    m_logFileName = logFileName;
    emit logFileNameChanged(m_logFileName);
}

void QmlInterface::setDatabaseLoaded(bool databaseLoaded)
{
    if (m_databaseLoaded == databaseLoaded)
        return;

    m_databaseLoaded = databaseLoaded;
    emit databaseLoadedChanged(m_databaseLoaded);
}

void QmlInterface::setDatabaseConnectionErrorString(QString databaseConnectionErrorString)
{
    if (m_databaseConnectionErrorString == databaseConnectionErrorString)
        return;

    m_databaseConnectionErrorString = databaseConnectionErrorString;
    emit databaseConnectionErrorStringChanged(m_databaseConnectionErrorString);
}

void QmlInterface::setIsInternetConnected(bool isInternetConnected)
{
    if (m_isInternetConnected == isInternetConnected)
        return;

    m_isInternetConnected = isInternetConnected;
    emit isInternetConnectedChanged(m_isInternetConnected);
}

void QmlInterface::setCreditPaidYAxis(QList<int> creditPaidYAxis)
{
    if (m_creditPaidYAxis == creditPaidYAxis)
        return;

    m_creditPaidYAxis = creditPaidYAxis;
    emit creditPaidYAxisChanged(m_creditPaidYAxis);
}

void QmlInterface::onLogsTimerTimeout()
{
    QString logName = QDateTime::currentDateTime().toString("yyyy-MM-dd")+"_app.log";

    QString path = m_logsPath + "/" + logName;

    setLogFileName(path.toStdString());

    qDebug() << "File Name: " << path;
}

void QmlInterface::onInternetConnectionStatusChanged(bool state)
{
    setIsInternetConnected(state);
    // qDebug() << "Is Connected? " << state;
}

void QmlInterface::onDatabaseConnectionError(const QString &errorMessage)
{
    logToFile("ERROR", "Database connection error dialog shown: " + errorMessage);
    
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Icon::Critical);
    msgBox.setWindowTitle("Database Connection Error");
    msgBox.setText("Failed to connect to the database.");
    msgBox.setInformativeText(errorMessage + "\n\nPlease check your database configuration and ensure:\n"
                                            "- PostgreSQL server is running\n"
                                            "- Database credentials are correct\n"
                                            "- Environment variables are set (S_DB_HOST, S_DB_USERNAME, S_DB_PASSWORD, S_DB_NAME)");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}

void QmlInterface::initializeLogFileName()
{
    if(logWriter.is_open())
    {
        logWriter.close();
    }

    QString logName = QDateTime::currentDateTime().toString("yyyy-MM-dd")+"_app.log";

    logWriter.open((m_logsPath+"/"+logName).toStdString(), std::ios::out | std::ios::app | std::ios::binary | std::ios::ate);

    if(logWriter.is_open())
    {
        logWriter << "File opened!";

        m_logFileDayChanged = false;
    }
}

void QmlInterface::downloadUpdate()
{
    /// Start update download for a newer version
    /// If update started,

    qDebug() << ">> Downloading Update";

    QNetworkAccessManager * network = new QNetworkAccessManager(this);
    QUrl url(m_UpdateJSON.value("link").toString());
    QNetworkRequest request(url);

    // QJsonDocument doc(m_UpdateJSON);
    qDebug() << m_UpdateJSON.value("link").toString();
    QString pth = m_path+"/downloads/"+m_UpdateJSON.value("filename").toString();

    emit downloadStarted();

    connect(network, &QNetworkAccessManager::finished, this, [=](QNetworkReply *reply) {
        auto app = reply->readAll(); //.data();

        QFile file(pth);
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(app);
            file.close();
            qDebug() << "Update download finished!";

            emit downloadFinished(m_UpdateJSON.value("filename").toString());
        }

        else
        {
            qDebug() << "Couldn't open file! " << file.errorString();
        }


        reply->deleteLater();
    });

    QNetworkReply *reply = network->get(request);

    connect(reply, &QNetworkReply::downloadProgress, this, [=](qint64 val, qint64 max){
        emit updateProgressChanged(int((val/float(max))*100));
    });

}

void QmlInterface::check4Update()
{
    emit started(versionInt());
}

void QmlInterface::installUpdate()
{
    // TODO ...
    // qDebug() << "Update file location: " << r_path;

    // QDesktopServices::openUrl(QUrl(m_path + "/downloads/"));
    // std::string str = r_path.toStdString();
    // const char* cmd = str.c_str();
    // system(cmd);
    // TODO implement update installation
}

void QmlInterface::getSalesSummary(const int &ind)
{
    logToFile("INFO", "Starting QmlInterface::getSalesSummary()");

    QSqlDatabase db = QSqlDatabase::database();

    if(db.isOpen())
    {
        QString start, end;

        if(ind == 0)
        {
            auto x = m_dateTime->getTimestamp("today");
            start = x.at(0);
            end = x.at(1);
        }

        else if(ind == 1)
        {
            auto x = m_dateTime->getTimestamp("yesterday");
            start = x.at(0);
            end = x.at(1);
        }

        else if(ind == 2)
        {
            auto x = m_dateTime->getTimestamp("thisWeek");
            start = x.at(0);
            end = x.at(1);
        }

        else if(ind == 3)
        {
            auto x = m_dateTime->getTimestamp("thisMonth");
            start = x.at(0);
            end = x.at(1);
        }

        else if(ind == 4)
        {
            auto x = m_dateTime->getTimestamp("thisYear");
            start = x.at(0);
            end = x.at(1);
        }

        else
        {
            auto x = m_dateTime->getTimestamp("today");
            start = x.at(0);
            end = x.at(1);
        }

        QSqlQuery query, query1;
        QString sql = "SELECT sum(cash), sum(mpesa), sum(cheque), sum(credit) FROM payment WHERE sales_id IN (SELECT DISTINCT sales_id FROM sales WHERE sales_date BETWEEN :startDate AND :endDate)";
        QString sql1 = "SELECT sum(payment_amount) FROM credit_payments WHERE payment_timestamp BETWEEN :startDate AND :endDate";

        query.prepare(sql);
        query.bindValue(":startDate", start);
        query.bindValue(":endDate", end);

        query1.prepare(sql1);
        query1.bindValue(":startDate", start);
        query1.bindValue(":endDate", end);

        if(query.exec() && query1.exec())
        {
            int cash =0, mpesa=0,cheque=0, credit=0, paid=0, totals=0;

            while(query.next())
            {
                cash = query.value(0).toInt();
                mpesa = query.value(1).toInt();
                cheque = query.value(2).toInt();
                credit = query.value(3).toInt();

                break;
            }

            while(query1.next())
            {
                paid = query1.value(0).toInt();

                break;
            }

            totals = cash + mpesa + cheque + paid;

            emit salesSummaryCost(cash,mpesa,cheque,credit,paid,totals);

            logToFile("INFO","Sales Summary loading successful!");
        }

        else
        {
            logToFile("CRITICAL", "Error Fetching Sales Summary : " + query.lastError().text() + " --- " + query1.lastError().text());
        }
    }

    logToFile("INFO", "Ending QmlInterface::getSalesSummary()");
}

void QmlInterface::setTabularData()
{
    logToFile("INFO", "Starting QmlInterface::setTabularData()");

    QFile file(":/sql/type.sql");
    file.open(QIODevice::ReadOnly);
    QString sql = file.readAll();
    file.close();

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;

    bool isDbSetUp = true; // m_settings->value("Config/isDbSetUp", false).toBool();

    if(db.isOpen() && !isDbSetUp)
    {
        if(query.exec(sql))
        {
            file.setFileName(":/sql/item.sql");
            auto _ = file.open(QIODevice::ReadOnly);
            sql = file.readAll();
            file.close();

            if(query.exec(sql))
            {
                file.setFileName(":/sql/stock.sql");
                auto _ = file.open(QIODevice::ReadOnly);
                sql = file.readAll();
                file.close();

                if(query.exec(sql))
                {
                    qDebug() << "[INFO] All DB is set up!";
                    m_settings->setValue("Config/isDbSetUp", true); // Save the new state
                }

                else
                    qDebug() << "[ERROR] Stock Data Exec: " << query.lastError().text();
            }

            else
                qDebug() << "[ERROR] Item Data Exec: " << query.lastError().text();
        }

        else
            qDebug() << "[ERROR] Type Data Exec: " << query.lastError().text();
    }


    logToFile("INFO", "Ending QmlInterface::setTabularData()");
}

bool QmlInterface::isDarkTheme() const
{
    return m_isDarkTheme;
}

bool QmlInterface::isFirstTimeUse() const
{
    return m_isFirstTimeUse;
}

int QmlInterface::dueSoonReminders() const
{
    return m_dueSoonReminders;
}

int QmlInterface::overdueReminders() const
{
    return m_overdueReminders;
}

int QmlInterface::solvedReminders() const
{
    return m_solvedReminders;
}

int QmlInterface::salesNumbers() const
{
    return m_salesNumbers;
}

int QmlInterface::salesCost() const
{
    return m_salesCost;
}

int QmlInterface::sentMessages() const
{
    return m_setMessages;
}

int QmlInterface::receivedMessages() const
{
    return m_receivedMessages;
}

int QmlInterface::unreadMessages() const
{
    return m_unreadMessages;
}

QStringList QmlInterface::plotXAxis() const
{
    return m_plotXAxis;
}

QList<int> QmlInterface::cashYAxis() const
{
    return m_cashYAxis;
}

QList<int> QmlInterface::mpesaYAxis() const
{
    return m_mpesaYAxis;
}

QList<int> QmlInterface::creditYAxis() const
{
    return m_creditYAxis;
}

QList<int> QmlInterface::chequeYAxis() const
{
    return m_chequeYAxis;
}

int QmlInterface::plotYmax() const
{
    return m_plotYmax;
}

bool QmlInterface::tablesCreated() const
{
    return m_tablesCreated;
}

bool QmlInterface::productsAdded() const
{
    return m_productsAdded;
}

bool QmlInterface::productTypeAdded() const
{
    return m_productTypeAdded;
}

bool QmlInterface::productStockAdded() const
{
    return m_productStockAdded;
}

int QmlInterface::versionInt() const
{
    return m_versionInt;
}

void QmlInterface::setIsDarkTheme(bool isDarkTheme)
{
    if (m_isDarkTheme == isDarkTheme)
        return;

    m_isDarkTheme = isDarkTheme;
    // qDebug() << "Before isDarkTheme? " << m_settings->value("Theme/isDarkTheme").toBool();
    m_settings->setValue("Theme/isDarkTheme", m_isDarkTheme); // Save the new state
    // qDebug() << "After isDarkTheme? " << m_settings->value("Theme/isDarkTheme").toBool();
    emit isDarkThemeChanged(m_isDarkTheme);
}

void QmlInterface::setIsFirstTimeUse(bool isFirstTimeUse)
{
    if (m_isFirstTimeUse == isFirstTimeUse)
        return;

    m_isFirstTimeUse = isFirstTimeUse;
    emit isFirstTimeUseChanged(m_isFirstTimeUse);
}

void QmlInterface::setDueSoonReminders(int dueSoonReminders)
{
    if (m_dueSoonReminders == dueSoonReminders)
        return;

    m_dueSoonReminders = dueSoonReminders;
    emit dueSoonRemindersChanged(m_dueSoonReminders);
}

void QmlInterface::setOverdueReminders(int overdueReminders)
{
    if (m_overdueReminders == overdueReminders)
        return;

    m_overdueReminders = overdueReminders;
    emit overdueRemindersChanged(m_overdueReminders);
}

void QmlInterface::setSolvedReminders(int solvedReminders)
{
    if (m_solvedReminders == solvedReminders)
        return;

    m_solvedReminders = solvedReminders;
    emit solvedRemindersChanged(m_solvedReminders);
}

void QmlInterface::setSalesNumbers(int salesNumbers)
{
    if (m_salesNumbers == salesNumbers)
        return;

    m_salesNumbers = salesNumbers;
    emit salesNumbersChanged(m_salesNumbers);
}

void QmlInterface::setSalesCost(int salesCost)
{
    if (m_salesCost == salesCost)
        return;

    m_salesCost = salesCost;
    emit salesCostChanged(m_salesCost);
}

void QmlInterface::setReceivedMessages(int receivedMessages)
{
    if (m_receivedMessages == receivedMessages)
        return;

    m_receivedMessages = receivedMessages;
    emit receivedMessagesChanged(m_receivedMessages);
}

void QmlInterface::setSentMessages(int setMessages)
{
    if (m_setMessages == setMessages)
        return;

    m_setMessages = setMessages;
    emit sentMessagesChanged(m_setMessages);
}

void QmlInterface::setUnreadMessages(int unreadMessages)
{
    if (m_unreadMessages == unreadMessages)
        return;

    m_unreadMessages = unreadMessages;
    emit unreadMessagesChanged(m_unreadMessages);
}

void QmlInterface::setPlotXAxis(QStringList plotXAxis)
{
    if (m_plotXAxis == plotXAxis)
        return;

    m_plotXAxis = plotXAxis;
    emit plotXAxisChanged(m_plotXAxis);
}

void QmlInterface::setCashYAxis(QList<int> cashYAxis)
{
    if (m_cashYAxis == cashYAxis)
        return;

    m_cashYAxis = cashYAxis;
    emit cashYAxisChanged(m_cashYAxis);
}

void QmlInterface::setMpesaYAxis(QList<int> mpesaYAxis)
{
    if (m_mpesaYAxis == mpesaYAxis)
        return;

    m_mpesaYAxis = mpesaYAxis;
    emit mpesaYAxisChanged(m_mpesaYAxis);
}

void QmlInterface::setCreditYAxis(QList<int> creditYAxis)
{
    if (m_creditYAxis == creditYAxis)
        return;

    m_creditYAxis = creditYAxis;
    emit creditYAxisChanged(m_creditYAxis);
}

void QmlInterface::setChequeYAxis(QList<int> chequeYAxis)
{
    if (m_chequeYAxis == chequeYAxis)
        return;

    m_chequeYAxis = chequeYAxis;
    emit chequeYAxisChanged(m_chequeYAxis);
}

void QmlInterface::setPlotYmax(int plotYmax)
{
    if (m_plotYmax == plotYmax)
        return;

    m_plotYmax = plotYmax;
    emit plotYmaxChanged(m_plotYmax);
}

void QmlInterface::setTablesCreated(bool tablesCreated)
{
    if (m_tablesCreated == tablesCreated)
        return;

    m_tablesCreated = tablesCreated;
    emit tablesCreatedChanged(m_tablesCreated);
}

void QmlInterface::setProductsAdded(bool productsAdded)
{
    if (m_productsAdded == productsAdded)
        return;

    m_productsAdded = productsAdded;
    emit productsAddedChanged(m_productsAdded);
}

void QmlInterface::setProductTypeAdded(bool productTypeAdded)
{
    if (m_productTypeAdded == productTypeAdded)
        return;

    m_productTypeAdded = productTypeAdded;
    emit productTypeAddedChanged(m_productTypeAdded);
}

void QmlInterface::setProductStockAdded(bool productStockAdded)
{
    if (m_productStockAdded == productStockAdded)
        return;

    m_productStockAdded = productStockAdded;
    emit productStockAddedChanged(m_productStockAdded);
}

void QmlInterface::setVersionInt(int versionInt)
{
    if (m_versionInt == versionInt)
        return;

    m_versionInt = versionInt;
    emit versionIntChanged(m_versionInt);
}

void QmlInterface::logToFile(const QString &level, const QString &log)
{
    if(level == "INFO")
    {
        qInfo() << "INFO\t->" << log;
    } else if (level == "CRITICAL")
    {
        qCritical() << "CRITICAL\t->" + log;
    } else if (level == "FATAL")
    {
        QString s = "FATAL\t->" + log;
        std::string s_std= s.toStdString();
        const char* err = s_std.c_str();
        qFatal(err);
    } else {
        qDebug() << "DEBUG\t->" << log;
    }

    try
    {
        if(!logWriter.is_open())
        {
            QString logName = QDateTime::currentDateTime().toString("yyyy-MM-dd")+"_app.log";
            logWriter.open((m_logsPath+"/"+logName).toStdString(), std::ios::out | std::ios::app | std::ios::binary | std::ios::ate);

            QString content=QString("%1 \t [%2]\tLog: %3 \r\n").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd_hh:mm:ss.z"), level, log);
            logWriter << content.toStdString();
            logWriter.close();
        }

        else
        {
            QString content=QString("%1 \t [%2]\tLog: %3 \r\n").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd_hh:mm:ss.z"), level, log);
            logWriter << content.toStdString();
            logWriter.close();
        }
    }

    catch(...)
    {
        qDebug() << "Log Writer Threw an Error!";
    }
}

void QmlInterface::openLocation(const QString &path)
{
    auto p = path.split('/'); // .removeLast() // .join('/');
    p.removeLast();
    auto r = p.join('/');

    qDebug() << "Folder: " << r;
    QDesktopServices::openUrl(r);
}
