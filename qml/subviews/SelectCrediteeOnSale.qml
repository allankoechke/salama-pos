import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4 as Controls2

import "../components"
import "../delegates"

Controls2.Popup
{
    id: root
    width: 450
    height: 350

    x: (mainApp.width - width)/2
    y: (mainApp.height - height)/2
    modal: true
    closePolicy: Controls2.Popup.NoAutoClose

    /*onClosed: {
        combo.currentIndex = 0;
        currentStock = 0;
        addStock.value = 1;
        reduceStock.value = 0;
    }*/

    property var selectedCreditee: ""

    signal accepted()

    contentItem: Rectangle
    {
        color: bgColor
        anchors.fill: parent


        ColumnLayout
        {
            anchors.fill: parent
            anchors.margins: 30

            Item
            {
                Layout.fillWidth: true
                Layout.preferredHeight: 40

                Rectangle
                {
                    color: "black"
                    height: 3
                    width: parent.width
                    opacity: 0.08

                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 5
                }

                AppText
                {
                    color: QmlInterface.isDarkTheme? "#eee" : "black"
                    size: 17
                    text: qsTr("Select Creditee")

                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                }

                AppButton
                {
                    // radius: 3
                    height: 36
                    width: 100
                    // bgRect.color: "transparent"
                    // bgRect.border {
                    //     color: backgroundColor
                    //     width: 1
                    // }

                    text: qsTr("+ New")
                    onClicked: newCrediteePopup.open()

                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                }
            }

            Rectangle
            {
                Layout.fillWidth: true
                Layout.fillHeight: true

                color: QmlInterface.isDarkTheme? "#29292d":bgColor

                ColumnLayout
                {
                    anchors.fill: parent
                    spacing: 0

                    Rectangle
                    {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 35

                        color: menuColor

                        RowLayout
                        {
                            anchors.fill: parent
                            spacing: 1


                            AppText
                            {
                                size: 17
                                color: "white"
                                text: qsTr("No.")
                                font.bold: true
                                horizontalAlignment: AppText.AlignHCenter

                                Layout.alignment: Qt.AlignVCenter|Qt.AlignHCenter
                                Layout.preferredWidth: 50
                            }

                            VSeparator{}

                            AppText
                            {
                                size: 17
                                color: "white"
                                text: qsTr("Name")
                                font.bold: true
                                horizontalAlignment: AppText.AlignHCenter

                                Layout.alignment: Qt.AlignVCenter|Qt.AlignHCenter
                                Layout.fillWidth: true
                            }

                            VSeparator{}

                            AppText
                            {
                                size: 17
                                color: "white"
                                text: qsTr("ID No.")
                                font.bold: true
                                horizontalAlignment: AppText.AlignHCenter

                                Layout.alignment: Qt.AlignVCenter|Qt.AlignHCenter
                                Layout.preferredWidth: 120
                            }
                        }
                    }

                    Controls2.ScrollView
                    {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        ListView
                        {
                            id: lv_crediteelist
                            width: parent.width
                            height: parent.height
                            anchors.left: parent.left

                            spacing: 0
                            currentIndex: -1
                            model: CrediteeModel
                            delegate: CrediteeOnSaleDelegate
                            {
                                width: lv_crediteelist.width
                                _index: (index + 1).toString()+"."
                                name: c_fname + " " + c_lname
                                id_no: c_idNo
                                isSelected: _index === (lv_crediteelist.currentIndex + 1).toString()+"."

                                onSelected: {
                                    lv_crediteelist.currentIndex = index
                                    selectedCreditee = id_no
                                }
                            }
                        }
                    }

                }
            }


            Item
            {
                Layout.fillWidth: true
                Layout.preferredHeight: 48

                RowLayout
                {
                    anchors.fill: parent
                    spacing: 10

                    HorizontalSpacer {}

                    AppButton
                    {
                        // radius: 3
                        // bgRect.color: "transparent"
                        // bgRect.border {
                        //     color: backgroundColor
                        //     width: 1
                        // }

                        text: qsTr("Cancel")
                        onClicked: root.close()
                        Layout.fillHeight: true
                        Layout.preferredWidth: 100
                    }

                    AppButton
                    {
                        text: qsTr("Select")
                        onClicked: {
                            root.accepted()
                            root.close()
                        }

                        Layout.fillHeight: true
                        Layout.preferredWidth: 120
                    }
                }
            }
        }
    }

    Connections {
        target: newCrediteePopup

        function onCrediteeAdded() {
            // TODO
            // Fetch the creditee and update the table
        }
    }
}
