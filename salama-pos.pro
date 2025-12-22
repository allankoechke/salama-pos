QT += quick widgets charts sql quickcontrols2  printsupport

CONFIG += c++11 # qwt

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        cpp/databaseinterface.cpp \
        cpp/datetime.cpp \
        cpp/main.cpp \
        cpp/models/alarmsmodel.cpp \
        cpp/models/checkoutitemsmodel.cpp \
        cpp/models/completermodel.cpp \
        cpp/models/crediteeaccountsmodel.cpp \
        cpp/models/productsalesmodel.cpp \
        cpp/models/saleitemsmodel.cpp \
        cpp/models/src/alarms.cpp \
        cpp/models/src/checkoutitems.cpp \
        cpp/models/src/completer.cpp \
        cpp/models/src/crediteeaccount.cpp \
        cpp/models/src/productsales.cpp \
        cpp/models/src/saleitems.cpp \
        cpp/models/src/stockitems.cpp \
        cpp/models/src/useraccounts.cpp \
        cpp/models/stockitemsmodel.cpp \
        cpp/models/useraccountsmodel.cpp \
        cpp/pingserverprocess.cpp \
        cpp/processinterface.cpp \
        cpp/qmlinterface.cpp \
        # cpp/qwtbarplot.cpp \
        # cpp/qwtbarplotdata.cpp \
        cpp/runguard.cpp \
        cpp/serialportinterface.cpp \
        cpp/webapiinterface.cpp \
        cpp/websocketsinterface.cpp

RESOURCES += qml.qrc

unix
{
    # include (/usr/local/qwt-6.1.5/features/qwt.prf)
}

win32
{
    # include (c:/qwt-6.1.4/features/qwt.prf)
}

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    cpp/databaseinterface.h \
    cpp/datetime.h \
    cpp/models/alarmsmodel.h \
    cpp/models/checkoutitemsmodel.h \
    cpp/models/completermodel.h \
    cpp/models/crediteeaccountsmodel.h \
    cpp/models/productsalesmodel.h \
    cpp/models/saleitemsmodel.h \
    cpp/models/src/alarms.h \
    cpp/models/src/checkoutitems.h \
    cpp/models/src/completer.h \
    cpp/models/src/crediteeaccount.h \
    cpp/models/src/productsales.h \
    cpp/models/src/saleitems.h \
    cpp/models/src/stockitems.h \
    cpp/models/src/useraccounts.h \
    cpp/models/stockitemsmodel.h \
    cpp/models/useraccountsmodel.h \
    cpp/pingserverprocess.h \
    cpp/processinterface.h \
    cpp/qmlinterface.h \
    # cpp/qwtbarplot.h \
    # cpp/qwtbarplotdata.h \
    cpp/runguard.h \
    cpp/serialportinterface.h \
    cpp/webapiinterface.h \
    cpp/websocketsinterface.h

DISTFILES +=

android {
    ANDROID_ABIS = armeabi-v7a
}

