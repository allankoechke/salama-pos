cmake_minimum_required(VERSION 3.16)

# Set version (can be overridden via -DPROJECT_VERSION=...)
if(NOT DEFINED PROJECT_VERSION_MAJOR)
    set(PROJECT_VERSION_MAJOR 1)
endif()
if(NOT DEFINED PROJECT_VERSION_MINOR)
    set(PROJECT_VERSION_MINOR 0)
endif()
if(NOT DEFINED PROJECT_VERSION_PATCH)
    set(PROJECT_VERSION_PATCH 0)
endif()
if(NOT DEFINED PROJECT_VERSION_NICKNAME)
    set(PROJECT_VERSION_NICKNAME "")
endif()

set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

project(salamapos VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Quick
    QuickControls2
    Charts
    Sql
    PrintSupport
    SerialPort
    WebSockets
)

qt_standard_project_setup()
set(CMAKE_AUTORCC ON)

# Generate version header
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cpp/version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/version.h"
    @ONLY
)

qt_add_executable(salamapos
    cpp/main.cpp
    cpp/databaseinterface.cpp
    cpp/datetime.cpp
    cpp/pingserverprocess.cpp
    cpp/processinterface.cpp
    cpp/qmlinterface.cpp
    cpp/runguard.cpp
    cpp/serialportinterface.cpp
    cpp/webapiinterface.cpp
    cpp/websocketsinterface.cpp

    cpp/models/alarmsmodel.cpp
    cpp/models/checkoutitemsmodel.cpp
    cpp/models/completermodel.cpp
    cpp/models/crediteeaccountsmodel.cpp
    cpp/models/productsalesmodel.cpp
    cpp/models/saleitemsmodel.cpp
    cpp/models/stockitemsmodel.cpp
    cpp/models/useraccountsmodel.cpp

    cpp/models/src/alarms.cpp
    cpp/models/src/checkoutitems.cpp
    cpp/models/src/completer.cpp
    cpp/models/src/crediteeaccount.cpp
    cpp/models/src/productsales.cpp
    cpp/models/src/saleitems.cpp
    cpp/models/src/stockitems.cpp
    cpp/models/src/useraccounts.cpp

    qml.qrc
)

target_include_directories(salamapos PRIVATE 
    cpp
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(salamapos PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Charts
    Qt6::Sql
    Qt6::PrintSupport
    Qt6::SerialPort
    Qt6::WebSockets
)

target_compile_definitions(salamapos PRIVATE QT_DEPRECATED_WARNINGS)


# # Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# # If you are developing for iOS or macOS you should consider setting an
# # explicit, fixed bundle identifier manually though.
# set_target_properties(salamapos PROPERTIES
# #    MACOSX_BUNDLE_GUI_IDENTIFIER co.ke.codeart.appsalamapos
#     MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
#     MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
#     MACOSX_BUNDLE TRUE
#     WIN32_EXECUTABLE TRUE
# )

# include(GNUInstallDirs)
# install(TARGETS salamapos
#     BUNDLE DESTINATION .
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
# )
