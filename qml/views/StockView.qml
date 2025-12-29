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

    property var currentScreen: 0


    ColumnLayout
    {
        anchors.fill: parent
        anchors.leftMargin: 40
        anchors.rightMargin: 40
        anchors.bottomMargin: 30
        spacing: 10

        Item
        {
            Layout.fillWidth: true
            Layout.preferredHeight: 70

            Rectangle
            {
                width: parent.width
                height: 3
                color: "#d3e5ff"

                anchors.bottom: parent.bottom
                anchors.bottomMargin: 3
            }

            RowLayout
            {
                anchors.fill: parent
                spacing: 1

                StockViewSelectBtn
                {

                    buttonIcon: ""
                    buttonLabel: qsTr("Products")
                    isCurrentlySelected: currentScreen === 0

                    onClicked: currentScreen=0;
                }

                StockViewSelectBtn
                {
                    buttonIcon: ""
                    buttonLabel: qsTr("Stock")
                    isCurrentlySelected: currentScreen === 1

                    onClicked: currentScreen=1;
                }

                StockViewSelectBtn
                {
                    visible: false
                    buttonIcon: ""
                    buttonLabel: qsTr("Products")
                    isCurrentlySelected: currentScreen === 2

                    onClicked: currentScreen=2;
                }

                HorizontalSpacer {}

                CustomButton
                {
                    visible: currentScreen===0
                    enabled: loggedUser_canAddItems
                    Layout.preferredHeight: 45
                    Layout.preferredWidth: row1.width+20
                    Layout.alignment: Qt.AlignVCenter
                    Layout.rightMargin: 20

                    color: menuColor
                    radius: 3
                    opacity: enabled? 1:0.2

                    Row
                    {
                        id: row1
                        anchors.centerIn: parent

                        spacing: 7

                        AppIcon
                        {
                            color: "white"
                            size: 15
                            icon: "\uf067"
                        }

                        AppText
                        {
                            color: "white"
                            size: 15
                            text: qsTr("Generate Price List")
                        }
                    }

                    onReleased: {
                        logic.generatePriceList();
                    }
                }

                CustomButton
                {
                    visible: currentScreen===0
                    enabled: loggedUser_canAddItems
                    Layout.preferredHeight: 45
                    Layout.preferredWidth: 150
                    Layout.alignment: Qt.AlignVCenter
                    Layout.rightMargin: 20

                    color: menuColor
                    radius: 3
                    opacity: enabled? 1:0.2

                    RowLayout
                    {
                        anchors.centerIn: parent

                        spacing: 7

                        AppIcon
                        {
                            color: "white"
                            size: 15
                            icon: "\uf067"
                        }

                        AppText
                        {
                            color: "white"
                            size: 15
                            text: qsTr("Add Item")
                        }
                    }

                    onReleased: {

                        if(loggedUser_canAddItems)
                        {
                            newItemPopup.open()
                            newItemPopup.barCode = new Date().getTime().toString();
                            newItemPopup.itemName = "";
                            newItemPopup.itemUnit = "";
                            newItemPopup.itemBp = "";
                            newItemPopup.itemSp = "";
                            newItemPopup.itemQty = "";
                            newItemPopup.itemCompany = "";

                            newItemPopup.isNewItemMode = true;
                        }

                        else
                        {
                            ToastModel.addToastItem("error", "User has no rights to add items")
                            console.log(" [ERROR] User has no rights to add item")
                        }
                    }
                }

                CustomButton
                {
                    visible: false
                    Layout.fillHeight: true
                    Layout.preferredWidth: 150

                    enabled: loggedUser_canAddStock
                    color: menuColor
                    radius: 3
                    opacity: enabled? 1:0.2

                    RowLayout
                    {
                        anchors.centerIn: parent

                        spacing: 10

                        AppIcon
                        {
                            color: "white"
                            size: 15
                            icon: "\uf067"
                        }

                        AppText
                        {
                            color: "white"
                            size: 15
                            text: qsTr("Add Stock")
                        }
                    }
                }
            }
        }

        Item
        {
            Layout.fillHeight: true
            Layout.fillWidth: true

            StackLayout
            {
                anchors.fill: parent
                currentIndex: currentScreen

                StockProductView
                { }

                StockItemStockView
                { }


            }
        }
    }
}
