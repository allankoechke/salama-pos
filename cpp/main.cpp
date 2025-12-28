#include <QApplication>
#include <QQmlContext>
#include <QQuickStyle>
#include <QQmlApplicationEngine>
#include <QtQml/qqml.h>
#include <QFont>
#include <QFontDatabase>
#include <QMessageBox>

// To add a console when running on windows
#ifdef Q_OS_WINDOWS
#ifdef DEBUG_MODE

#include <windows.h>
#include <stdio.h>

#endif
#endif

#include "qmlinterface.h"
#include "models/stockitemsmodel.h"
#include "models/checkoutitemsmodel.h"
#include "models/useraccountsmodel.h"
#include "models/completermodel.h"
#include "models/saleitemsmodel.h"
#include "models/productsalesmodel.h"
#include "models/crediteeaccountsmodel.h"
#include "models/alarmsmodel.h"
#include "processinterface.h"
#include "runguard.h"

#define APP_NAME "Salama POS App"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QQuickStyle::setStyle(QStringLiteral("Material"));

    int fontId = QFontDatabase::addApplicationFont(":/assets/fonts/montserrat/Montserrat-Regular.ttf");
    if(fontId != -1)
    {
        QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
        if(!fontFamilies.isEmpty())
        {
            app.setFont(QFont(fontFamilies.first()));
        }
        else
        {
            qDebug() << "Could not retrieve font family from loaded font!";
        }
    }
    else
    {
        qDebug() << "Could not load the specified font!";
    }

    // Make sure there is no other instance running
    RunGuard runGuard(APP_NAME);
    if (!runGuard.tryToRun()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Icon::Critical);
        msgBox.setText(
            QString(QObject::tr("Salama POS is already running, if you need to log in as another user, log out then in with the new account.\n")));
        msgBox.exec();
        return 127;
    }

    QQmlApplicationEngine engine;


    UserAccountsModel m_userAccounts;

    // Singletons
    QmlInterface qmlInterface;
    StockItemsModel m_stockModel;
    CheckoutItemsModel m_checkoutItems;
    // UserAccountsModel m_userAccounts;
    CompleterModel m_completer;
    saleItemsModel m_saleItem;
    ProductSalesModel m_pSale;
    CrediteeAccountsModel creditee;
    AlarmsModel m_alarmModel;

    engine.rootContext()->setContextProperty("QmlInterface", &qmlInterface);
    engine.rootContext()->setContextProperty("StockItemModel", &m_stockModel);
    engine.rootContext()->setContextProperty("CheckoutModel", &m_checkoutItems);
    engine.rootContext()->setContextProperty("AccountsModel", &m_userAccounts);
    engine.rootContext()->setContextProperty("CompleterModel", &m_completer);
    engine.rootContext()->setContextProperty("SalesModel", &m_saleItem);
    engine.rootContext()->setContextProperty("ProductSalesModel", &m_pSale);
    engine.rootContext()->setContextProperty("CrediteeModel", &creditee);
    engine.rootContext()->setContextProperty("AlarmsModel", &m_alarmModel);

    qmlRegisterType<ProcessInterface>("me.devqt.Controls",1,0,"Process");

    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
        {
            QCoreApplication::exit(-1);
        }
    }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
