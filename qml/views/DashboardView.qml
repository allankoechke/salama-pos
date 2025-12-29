import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4 as Controls2

import "../components"

Item {
    id: root

    Layout.fillHeight: true
    Layout.fillWidth: true

    ColumnLayout
    {
        anchors.fill: parent
        anchors.margins: 30
        spacing: 10

        Item{
            Layout.fillWidth: true
            Layout.preferredHeight: 200

            RowLayout
            {
                anchors.fill: parent
                spacing: 10

                AppPaper
                {
                    color: QmlInterface.isDarkTheme? "#29292d":"white"
                    radius: 5
                    elevation: 6

                    Layout.fillHeight: true
                    Layout.preferredWidth: 300

                    // Sales summary
                    ColumnLayout
                    {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 10

                        AppPaper
                        {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 40

                            radius: 5
                            elevation: 4
                            color: menuColor

                            AppText
                            {
                                text: qsTr("Sales today")
                                color: "white"
                                size: 18

                                anchors.centerIn: parent
                            }
                        }

                        Item {
                            id: sales_pane
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            ColumnLayout
                            {
                                anchors.fill: parent
                                spacing: 0

                                Item
                                {
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 25

                                    RowLayout
                                    {
                                        anchors.fill: parent
                                        spacing: 0

                                        AppText
                                        {
                                            size: 15
                                            color: QmlInterface.isDarkTheme? "#f4f4f4":"#535353"
                                            text: qsTr("SALES QTY")

                                            Layout.preferredWidth: messages_pane.width/2
                                            horizontalAlignment: AppText.AlignHCenter
                                            Layout.alignment: Qt.AlignVCenter
                                        }

                                        Rectangle
                                        {
                                            color: "silver"
                                            Layout.fillHeight: true
                                            width: 1
                                        }

                                        AppText
                                        {
                                            size: 15
                                            color: QmlInterface.isDarkTheme? "#f4f4f4":"#535353"
                                            text: qsTr("T. COST")

                                            Layout.preferredWidth: messages_pane.width/2
                                            horizontalAlignment: AppText.AlignHCenter
                                            Layout.alignment: Qt.AlignVCenter
                                        }
                                    }
                                }

                                Rectangle
                                {
                                    height: 1; color: "silver"; Layout.fillWidth: true
                                }

                                Item
                                {
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true

                                    RowLayout
                                    {
                                        anchors.fill: parent
                                        spacing: 0

                                        AppText
                                        {
                                            size: 30
                                            color: QmlInterface.isDarkTheme? "#f4f4f4":"#535353"
                                            text: QmlInterface.salesNumbers

                                            Layout.preferredWidth: sales_pane.width/2
                                            horizontalAlignment: AppText.AlignHCenter
                                            Layout.alignment: Qt.AlignVCenter
                                        }

                                        Rectangle
                                        {
                                            color: "silver"
                                            Layout.fillHeight: true
                                            width: 1
                                        }

                                        AppText
                                        {
                                            size: 30
                                            color: QmlInterface.isDarkTheme? "#f4f4f4":"#535353"
                                            text: QmlInterface.salesCost

                                            Layout.preferredWidth: sales_pane.width/2
                                            horizontalAlignment: AppText.AlignHCenter
                                            Layout.alignment: Qt.AlignVCenter
                                        }
                                    }
                                }
                            }
                        }

                        Rectangle
                        {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 35

                            radius: height/2
                            color: Qt.lighter(menuColor, 1.5)


                            AppText
                            {
                                color: "#f4f4f4"
                                size: 20
                                text: qsTr("Generate report ")

                                anchors.left: parent.left
                                anchors.leftMargin: 20
                                anchors.verticalCenter: parent.verticalCenter
                            }

                            Rectangle
                            {
                                color: Qt.darker(Qt.lighter(menuColor, 1.5), 1.2)
                                radius: 20; height: 40; width: 40

                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter

                                AppIcon
                                {
                                    color: "#f4f4f4"
                                    size: 18
                                    icon: "\uf105"

                                    anchors.centerIn: parent
                                }
                            }

                            MouseArea
                            {
                                anchors.fill: parent
                                onClicked: {
                                    console.log("Open the SalesSummaryDialog()")
                                    QmlInterface.logToFile("INFO", "Open the SalesSummaryDialog()")
                                    salesSummaryDialog.show();
                                    console.log("Load Sales for SalesSummaryDialog()")
                                    QmlInterface.logToFile("INFO", "Load Sales for SalesSummaryDialog()")
                                    salesSummaryDialog.call_getSalesSummary()
                                    console.log("Finished Opening SalesSummaryDialog()")
                                    QmlInterface.logToFile("INFO", "Finished Opening SalesSummaryDialog()")
                                }
                            }
                        }
                    }
                }


                AppPaper
                {
                    color: QmlInterface.isDarkTheme? "#29292d":"white"
                    radius: 5
                    elevation: 6

                    Layout.fillHeight: true
                    Layout.preferredWidth: 300

                    // Reminders
                    ColumnLayout
                    {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 10

                        AppPaper
                        {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 40

                            radius: 5
                            elevation: 4
                            color: menuColor

                            AppText
                            {
                                text: qsTr("Notifications & Reminders")
                                color: "white"
                                size: 18

                                anchors.centerIn: parent
                            }
                        }


                        Item
                        {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            Layout.leftMargin: 20

                            RowLayout
                            {
                                anchors.fill: parent
                                spacing: 5

                                AppText
                                {
                                    id: due_soon
                                    color: QmlInterface.isDarkTheme? "#f4f4f4":"#2e2e2e"
                                    size: 35
                                    text: QmlInterface.dueSoonReminders

                                    verticalAlignment: AppText.AlignBottom
                                    horizontalAlignment: AppText.AlignRight
                                    Layout.fillHeight: true
                                    Layout.preferredWidth: 100 // solved.width > due_soon.width? (overdue.width > solved.width? overdue.width+20:solved.width+20):(overdue.width > due_soon.width? overdue.width+20:due_soon.width+20)
                                }

                                AppText
                                {
                                    color: QmlInterface.isDarkTheme? "#f4f4f4":"#2e2e2e"
                                    size: 12
                                    text: qsTr("DUE SOON")

                                    verticalAlignment: AppText.AlignBottom
                                    horizontalAlignment: AppText.AlignLeft
                                    Layout.fillHeight: true
                                    Layout.fillWidth: true
                                }
                            }
                        }

                        Item
                        {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            Layout.leftMargin: 20

                            RowLayout
                            {
                                anchors.fill: parent
                                spacing: 5

                                AppText
                                {
                                    id: overdue
                                    color: QmlInterface.isDarkTheme? "#f4f4f4":"#2e2e2e"
                                    size: 35
                                    text: QmlInterface.overdueReminders

                                    verticalAlignment: AppText.AlignBottom
                                    horizontalAlignment: AppText.AlignRight
                                    Layout.fillHeight: true
                                    Layout.preferredWidth: 100 // solved.width > due_soon.width? (overdue.width > solved.width? overdue.width+20:solved.width+20):(overdue.width > due_soon.width? overdue.width+20:due_soon.width+20)
                                }

                                AppText
                                {
                                    color: QmlInterface.isDarkTheme? "#f4f4f4":"#2e2e2e"
                                    size: 12
                                    text: qsTr("OVERDUE")

                                    verticalAlignment: AppText.AlignBottom
                                    horizontalAlignment: AppText.AlignLeft
                                    Layout.fillHeight: true
                                    Layout.fillWidth: true
                                }
                            }
                        }

                        Item
                        {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            Layout.leftMargin: 20

                            RowLayout
                            {
                                anchors.fill: parent
                                spacing: 5

                                AppText
                                {
                                    id: solved
                                    color: QmlInterface.isDarkTheme? "#f4f4f4":"#2e2e2e"
                                    size: 35
                                    text: QmlInterface.solvedReminders

                                    verticalAlignment: AppText.AlignBottom
                                    horizontalAlignment: AppText.AlignRight
                                    Layout.fillHeight: true
                                    Layout.preferredWidth: 100 // solved.width > due_soon.width? (overdue.width > solved.width? overdue.width+20:solved.width+20):(overdue.width > due_soon.width? overdue.width+20:due_soon.width+20)
                                }

                                AppText
                                {
                                    color: QmlInterface.isDarkTheme? "#f4f4f4":"#2e2e2e"
                                    size: 12
                                    text: qsTr("SOLVED")

                                    verticalAlignment: AppText.AlignBottom
                                    horizontalAlignment: AppText.AlignLeft
                                    Layout.fillHeight: true
                                    Layout.fillWidth: true
                                }
                            }
                        }


                    }

                }

                AppPaper
                {
                    color: QmlInterface.isDarkTheme? "#29292d":"white"
                    radius: 5
                    elevation: 6

                    Layout.fillHeight: true
                    Layout.preferredWidth: 300

                    // Reminders
                    ColumnLayout
                    {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 10

                        AppPaper
                        {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 40

                            radius: 5
                            elevation: 4
                            color: menuColor

                            AppText
                            {
                                text: qsTr("MESSAGES & INBOX")
                                color: "white"
                                size: 18

                                anchors.centerIn: parent
                            }
                        }

                        Item {
                            id: messages_pane
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            ColumnLayout
                            {
                                anchors.fill: parent
                                spacing: 0

                                Item
                                {
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 25

                                    RowLayout
                                    {
                                        anchors.fill: parent
                                        spacing: 0

                                        AppText
                                        {
                                            size: 15
                                            color: QmlInterface.isDarkTheme? "#f4f4f4":"#535353"
                                            text: qsTr("SENT")

                                            Layout.preferredWidth: messages_pane.width/2
                                            horizontalAlignment: AppText.AlignHCenter
                                            Layout.alignment: Qt.AlignVCenter
                                        }

                                        Rectangle
                                        {
                                            color: "silver"
                                            Layout.fillHeight: true
                                            width: 1
                                        }

                                        AppText
                                        {
                                            size: 15
                                            color: QmlInterface.isDarkTheme? "#f4f4f4":"#535353"
                                            text: qsTr("RECEIVED")

                                            Layout.preferredWidth: messages_pane.width/2
                                            horizontalAlignment: AppText.AlignHCenter
                                            Layout.alignment: Qt.AlignVCenter
                                        }
                                    }
                                }

                                Rectangle
                                {
                                    height: 1; color: "silver"; Layout.fillWidth: true
                                }

                                Item
                                {
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true

                                    RowLayout
                                    {
                                        anchors.fill: parent
                                        spacing: 0

                                        AppText
                                        {
                                            size: 30
                                            color: QmlInterface.isDarkTheme? "#f4f4f4":"#535353"
                                            text: QmlInterface.sentMessages

                                            Layout.preferredWidth: messages_pane.width/2
                                            horizontalAlignment: AppText.AlignHCenter
                                            Layout.alignment: Qt.AlignVCenter
                                        }

                                        Rectangle
                                        {
                                            color: "silver"
                                            Layout.fillHeight: true
                                            width: 1
                                        }

                                        AppText
                                        {
                                            size: 30
                                            color: QmlInterface.isDarkTheme? "#f4f4f4":"#535353"
                                            text: QmlInterface.receivedMessages

                                            Layout.preferredWidth: messages_pane.width/2
                                            horizontalAlignment: AppText.AlignHCenter
                                            Layout.alignment: Qt.AlignVCenter
                                        }
                                    }
                                }
                            }
                        }

                        Item
                        {
                            id: p
                            Layout.fillWidth: true
                            Layout.preferredHeight: 45

                            RowLayout
                            {
                                anchors.fill: parent
                                spacing: 5

                                AppText
                                {
                                    color: QmlInterface.isDarkTheme? "#f4f4f4":"#2e2e2e"
                                    size: 20
                                    text: qsTr("Unread/pending ")

                                    verticalAlignment: AppText.AlignVCenter
                                    horizontalAlignment: AppText.AlignRight
                                    Layout.fillHeight: true
                                    Layout.fillWidth: true
                                }

                                AppPaper
                                {
                                    color: Qt.darker(menuColor, 1.5)
                                    elevation: 6
                                    radius: 25
                                    Layout.preferredHeight: 50
                                    Layout.preferredWidth: 50
                                    Layout.rightMargin: 5

                                    AppText
                                    {
                                        color: "#f4f4f4"
                                        size: 18
                                        text: QmlInterface.unreadMessages

                                        anchors.centerIn: parent
                                    }
                                }
                            }
                        }

                    }
                }

                AppPaper
                {
                    color: QmlInterface.isDarkTheme? "#29292d":"white"
                    radius: 5
                    elevation: 6

                    Layout.fillHeight: true
                    Layout.minimumWidth: 300
                    Layout.fillWidth: true

                    // Reminders
                    ColumnLayout
                    {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 10

                        AppPaper
                        {
                            visible: false
                            Layout.fillWidth: true
                            Layout.preferredHeight: 40

                            radius: 5
                            elevation: 4
                            color: menuColor

                            AppText
                            {
                                text: qsTr("Shortcuts")
                                color: "white"
                                size: 18

                                anchors.centerIn: parent
                            }
                        }

                        Controls2.ScrollView {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            Controls2.ScrollBar.vertical.policy: Controls2.ScrollBar.AlwaysOn

                            spacing: 5
                            clip: true

                            ColumnLayout
                            {
                                anchors.fill: parent
                                spacing: 5


                                DashboardShortcutButton
                                {
                                    icon: "\uf002"
                                    label: qsTr("Search item")

                                    onClicked: {
                                        QmlInterface.logToFile("INFO", "QML => DashboardView::SearchItem ...")
                                    }
                                }

                                DashboardShortcutButton
                                {
                                    visible: isAdmin
                                    icon: "\uf234"
                                    label: qsTr("Add new user")

                                    onClicked: {
                                        if(loggedUser_canAddAccounts)
                                        {
                                            userAccountPopup.open()

                                            QmlInterface.logToFile("INFO", "QML => DashboardView::AddNewUser User has rights to add users")
                                        }

                                        else
                                        {
                                            ToastModel.addToastItem("error", "User has no rights to add users")

                                            QmlInterface.logToFile("INFO", "QML => DashboardView::AddNewUser User has no rights to add users")
                                        }
                                    }
                                }

                                DashboardShortcutButton
                                {
                                    visible: loggedUser_canAddStock
                                    icon: "\uf5ff"
                                    label: qsTr("Add Item's Stock")

                                    onClicked: {
                                        navBarIndex = 2; // Switch to item stock window
                                        stackStockView.currentScreen = 1;
                                        QmlInterface.logToFile("INFO", "QML => DashboardView::AddStock Switching to Stock Window")
                                    }
                                }

                                DashboardShortcutButton
                                {
                                    visible: isAdmin
                                    icon: "\uf124"
                                    label: qsTr("Send a message")

                                    onClicked: {
                                        navBarIndex = 4;
                                        QmlInterface.logToFile("INFO", "QML => DashboardView::SendMessage Clicked")
                                    }
                                }

                                DashboardShortcutButton
                                {
                                    icon: "\uf186"
                                    label: qsTr("Change color theme")

                                    onClicked: {
                                        QmlInterface.isDarkTheme = !QmlInterface.isDarkTheme;
                                    }
                                }

                                DashboardShortcutButton
                                {
                                    icon: "\uf021"
                                    label: qsTr("Sync to online database")

                                    onClicked: {
                                        // TODO
                                        QmlInterface.logToFile("INFO", "QML => DashboardView::SyncToOnlineAccount clicked")

                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        AppPaper
        {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: QmlInterface.isDarkTheme? "#29292d":"white"
            elevation: 10

            DashboardGraphWidget{}
        }
    }
}
