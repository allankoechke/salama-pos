import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

import "../delegates"
import "../components"

Window {
    id: myWindow

    property alias x: myWindow.x
    property alias y: myWindow.y
    property alias salesCash: salescash.cost
    property alias salesMpesa: salesmpesa.cost
    property alias salesCheque: salescheque.cost
    property alias salesCredit: salescredit.cost
    property alias debtPaid: debtpaid.cost
    property alias tlessDebt: tlessdebt.cost

    flags:  Qt.Dialog
    modality: Qt.ApplicationModal
    color: bgColor
    width: 500
    height: 470

    onClosing: cb.currentIndex = 0;

    Component.onCompleted: {
        cb.currentIndex = 0;
        console.log("Finished loading sales summary")
        QmlInterface.logToFile("INFO", "QML => Finished loading sales summary")
    }

    function call_getSalesSummary()
    {
        QmlInterface.logToFile("INFO", "QML => SalesSummaryDialog.qml: ComboBox::currentIndex="+cb.currentIndex)
        QmlInterface.getSalesSummary(cb.currentIndex)

    }


    ColumnLayout
    {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 10

        Item{
            Layout.fillWidth: true
            Layout.preferredHeight: 45

            RowLayout{
                anchors.fill: parent
                spacing: 5

                AppText
                {
                    Layout.alignment: Qt.AlignVCenter
                    horizontalAlignment: AppText.AlignLeft

                    text: qsTr("Sales Summary for: ")
                    color: QmlInterface.isDarkTheme? "#F5F5F5":"#2e2e2e"
                }

                ComboBox
                {
                    id: cb
                    height: 45
                    Layout.minimumWidth: 220
                    currentIndex: 0

                    editable: false
                    // enabled: isAdmin
                    model: isAdmin? ["Today", "Yesterday", "This Week", "This Month", "This Year", "All Time", "Custom"]:["Today", "Yesterday"]

                    Layout.rightMargin: 10

                    onCurrentIndexChanged: {
                        // ProductSalesModel.getSalesSummary(currentIndex)
                        QmlInterface.getSalesSummary(currentIndex)
                    }
                }
            }
        }

        Item{
            Layout.fillHeight: true
            Layout.fillWidth: true

            RowLayout
            {
                anchors.fill: parent
                spacing: 5

                Item{
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    ColumnLayout
                    {
                        anchors.fill: parent
                        spacing: 10
                        anchors.margins: 10

                        SaleSummaryCostWidget
                        {
                            id: salescash
                            icon: "\uf3d1"
                            text: qsTr("Sales Cash:")
                        }

                        SaleSummaryCostWidget
                        {
                            id: salesmpesa
                            icon: "\uf3ce"
                            text: qsTr("Sales M-Pesa:")
                        }

                        SaleSummaryCostWidget
                        {
                            id: salescredit
                            icon: "\uf09d"
                            text: qsTr("Sales Credit:")
                        }

                        SaleSummaryCostWidget
                        {
                            id: salescheque
                            icon: "\uf53d"
                            text: qsTr("Sales Cheque:")
                        }

                        SaleSummaryCostWidget
                        {
                            id: debtpaid
                            icon: "\uf4c4"
                            text: qsTr("Debt Re-Paid:")
                        }

                        Rectangle
                        {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 1
                            color: "silver"
                            opacity: 0.4
                        }

                        SaleSummaryCostWidget
                        {
                            id: tlessdebt
                            icon: "\uf52c"
                            text: qsTr("Totals less Debt:")
                        }
                    }
                }

                Rectangle
                {
                    Layout.fillHeight: true
                    Layout.preferredWidth: 1
                    color: "silver"
                }

                Item {
                    Layout.fillHeight: true
                    Layout.preferredWidth: 70

                    ColumnLayout
                    {
                        anchors.fill: parent
                        spacing: 5

                        Rectangle
                        {
                            id: close_btn
                            Layout.preferredWidth: 60
                            Layout.preferredHeight: 60

                            color: Qt.darker(menuColor, 1.6)
                            radius: height/2

                            ColumnLayout
                            {
                                anchors.centerIn: parent
                                spacing: 5

                                AppIcon
                                {
                                    size: 18
                                    color: "white"
                                    icon: "\uf02f"

                                    Layout.alignment: Qt.AlignHCenter
                                }

                                AppText
                                {
                                    text: qsTr("PRINT")
                                    color: "white"
                                    font.bold: true
                                    size: 13

                                    Layout.alignment: Qt.AlignHCenter
                                }
                            }


                            MouseArea
                            {
                                anchors.fill: parent
                                onClicked: {
                                    // searchText = "";
                                    // myWindow.close();
                                }
                            }
                        }

                        Rectangle
                        {
                            Layout.preferredWidth: 60
                            Layout.preferredHeight: 60

                            color: Qt.lighter("#FF605C", 1.2)
                            radius: height/2

                            ColumnLayout
                            {
                                anchors.centerIn: parent
                                spacing: 5

                                AppIcon
                                {
                                    size: 18
                                    color: "white"
                                    icon: "\uf1c1"

                                    Layout.alignment: Qt.AlignHCenter
                                }

                                AppText
                                {
                                    text: qsTr("PDF")
                                    color: "white"
                                    font.bold: true
                                    size: 13

                                    Layout.alignment: Qt.AlignHCenter
                                }
                            }



                            MouseArea
                            {
                                anchors.fill: parent
                                onClicked: {
                                    // searchText = "";
                                    myWindow.close();
                                }
                            }
                        }


                        Rectangle
                        {
                            Layout.preferredWidth: 60
                            Layout.preferredHeight: 60

                            color: "orange"
                            radius: height/2

                            ColumnLayout
                            {
                                anchors.centerIn: parent
                                spacing: 5

                                AppIcon
                                {
                                    size: 18
                                    color: "white"
                                    icon: "\uf021"

                                    Layout.alignment: Qt.AlignHCenter
                                }

                                AppText
                                {
                                    text: qsTr("SYNC")
                                    color: "white"
                                    font.bold: true
                                    size: 13

                                    Layout.alignment: Qt.AlignHCenter
                                }
                            }


                            MouseArea
                            {
                                anchors.fill: parent
                                onClicked: {
                                    // searchText = "";
                                    // myWindow.close();
                                }
                            }
                        }

                        Item {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        }

                        Rectangle
                        {
                            Layout.preferredWidth: 60
                            Layout.preferredHeight: 60

                            color: "#FF605C"
                            radius: height/2

                            ColumnLayout
                            {
                                anchors.centerIn: parent
                                spacing: 5

                                AppIcon
                                {
                                    size: 18
                                    color: "white"
                                    icon: "\uf00d"

                                    Layout.alignment: Qt.AlignHCenter
                                }

                                AppText
                                {
                                    text: qsTr("CLOSE")
                                    color: "white"
                                    font.bold: true
                                    size: 13

                                    Layout.alignment: Qt.AlignHCenter
                                }
                            }


                            MouseArea
                            {
                                anchors.fill: parent
                                onClicked: {
                                    myWindow.close();
                                }
                            }
                        }

                    }
                }
            }
        }

    }

    Connections
    {
        target: QmlInterface

        function onSalesSummaryCost(_cash,_mpesa,_cheque,_credit,_paid,_totals)
        {
            salesCash = qsTr("Ksh. ") + _cash.toString()
            salesMpesa = qsTr("Ksh. ") + _mpesa.toString()
            salesCheque = qsTr("Ksh. ") + _cheque.toString()
            salesCredit = qsTr("Ksh. ") + _credit.toString()
            debtPaid = qsTr("Ksh. ") + _paid.toString()
            tlessDebt = qsTr("Ksh. ") + _totals.toString()
        }
    }
}
