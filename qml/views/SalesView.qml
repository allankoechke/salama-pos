import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4 as Controls2

import "../components"
import "../subviews"

Item {
    id: root

    Layout.fillHeight: true
    Layout.fillWidth: true
    Layout.margins: 50

    // Current screen to display ...
    property int currentScreen: 0

    // Current date selection & subsequent selected text
    property alias currentIndex: cb.currentIndex
    property string currentSelection: "Today"

    // Handle custom date and date range selection
    property var startDate: undefined
    property var endDate: undefined

    onStartDateChanged: {
        if(startDate && currentSelection === "Custom Date") {
            var d = startDate.toISOString().split("T")[0];
            var start = `${d} 00:00:00+03`
            var end = `${d} 23:59:59+03`
            ProductSalesModel.showCustomDateSalesData(start, end)
        }
    }

    function getDateModel() {
        return isAdmin ? ["Today", "Yesterday", "This Week", "This Month", "This Year", "All Time", "Custom Date", "Custom Date Range"] : ["Today", "Yesterday", "Custom Date"]
    }

    function getData() {
        if (currentSelection === "Today")
            ProductSalesModel.showTodaysSales()
        else if (currentSelection === "Yesterday")
            ProductSalesModel.showYesterdaysSales()
        else if (currentSelection === "This Week")
            ProductSalesModel.showThisWeeksSales()
        else if (currentSelection === "This Month")
            ProductSalesModel.showThisMonthsSales()
        else if (currentSelection === "This Year")
            ProductSalesModel.showThisYearsSales()
        else if (currentSelection === "All Time")
            ProductSalesModel.loadSalesData()
        else if (currentSelection === "Custom Date") {
            startDatePopup.open()
        } else if (currentSelection === "Custom Date Range") {
            startDatePopup.open()
        } else
            cb.currentIndex = 0
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.leftMargin: 40
        anchors.rightMargin: 40
        anchors.bottomMargin: 30
        spacing: 10

        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: 70

            Rectangle {
                width: parent.width
                height: 3
                color: "#d3e5ff"

                anchors.bottom: parent.bottom
                anchors.bottomMargin: 3
            }

            RowLayout {
                anchors.fill: parent
                spacing: 1

                AppText {
                    color: QmlInterface.isDarkTheme ? "grey" : "black"
                    size: 17
                    text: qsTr("Showing Sales for : ")

                    Layout.alignment: Qt.AlignVCenter
                }

                // Select date for sales display ...
                Controls2.ComboBox {
                    id: cb
                    height: 45
                    Layout.minimumWidth: 220
                    currentIndex: 0

                    editable: false
                    model: getDateModel()

                    Layout.rightMargin: 10

                    onCurrentIndexChanged: {
                        currentSelection =  getDateModel()[currentIndex]
                        getData()
                    }
                }

                // Open date selector
                CustomButton {
                    Layout.preferredHeight: 45
                    Layout.preferredWidth: 200
                    Layout.margins: 1

                    color: menuColor
                    radius: 5
                    visible: ["Custom Date", "Custom Date Range"].includes(currentSelection)

                    RowLayout {
                        anchors.fill: parent
                        spacing: 1

                        AppIcon {
                            Layout.fillHeight: true
                            Layout.preferredWidth: height
                            horizontalAlignment: AppIcon.AlignHCenter
                            verticalAlignment: AppIcon.AlignVCenter

                            size: 16
                            color: "white"
                            icon: "\uf271"
                        }

                        AppText {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            horizontalAlignment: AppIcon.AlignLeft
                            verticalAlignment: AppIcon.AlignVCenter

                            color: "#f4f4f4"
                            size: 15
                            text: `Select ${currentSelection}`
                        }
                    }

                    onClicked: startDatePopup.open()
                }


                HorizontalSpacer {}

                CustomButton {
                    Layout.preferredHeight: 45
                    Layout.preferredWidth: 200
                    Layout.margins: 1

                    color: menuColor
                    radius: 5
                    visible: isAdmin

                    RowLayout {
                        anchors.fill: parent
                        spacing: 1

                        AppIcon {
                            Layout.fillHeight: true
                            Layout.preferredWidth: height
                            horizontalAlignment: AppIcon.AlignHCenter
                            verticalAlignment: AppIcon.AlignVCenter

                            size: 16
                            color: "white"
                            icon: "\uf7d9"
                        }

                        AppText {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            horizontalAlignment: AppIcon.AlignLeft
                            verticalAlignment: AppIcon.AlignVCenter

                            color: "#f4f4f4"
                            size: 15
                            text: qsTr("Admin Options")
                        }
                    }

                    onClicked: adminSalesWindow.show()
                }

                CustomButton {
                    Layout.preferredHeight: 45
                    Layout.preferredWidth: 200
                    Layout.margins: 1

                    color: menuColor
                    radius: 5

                    RowLayout {
                        anchors.fill: parent
                        spacing: 1

                        AppIcon {
                            Layout.fillHeight: true
                            Layout.preferredWidth: height
                            horizontalAlignment: AppIcon.AlignHCenter
                            verticalAlignment: AppIcon.AlignVCenter

                            size: 16
                            color: "white"
                            icon: "\uf81d"
                        }

                        AppText {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            horizontalAlignment: AppIcon.AlignLeft
                            verticalAlignment: AppIcon.AlignVCenter

                            color: "#f4f4f4"
                            size: 15
                            text: qsTr("Credits & Debts")
                        }
                    }

                    onClicked: creditNdebtpopup.open()
                }
            }
        }

        SalesProductView {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        CalendarPopup {
            id: startDatePopup

            onAccepted: (d) => startDate = d
        }

        CalendarPopup {
            id: endDatePopup
        }
    }
}
