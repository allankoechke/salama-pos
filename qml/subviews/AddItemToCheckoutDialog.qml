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
    property alias searchText: textField.text
    property alias currentIndex: completions.currentIndex

    flags:  Qt.Dialog
    modality: Qt.ApplicationModal
    color: bgColor
    width: 500
    height: 400

    Component.onCompleted: {
        logger.debug("Loaded the Add Item Dialog!");
    }

    onClosing: {
        close.accepted = false
        CompleterModel.addCompleterItems("")
        textField.text = ""
        close.accepted = true
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

                    text: qsTr("Item Name ")
                    color: QmlInterface.isDarkTheme? "#F5F5F5":"#2e2e2e"
                }

                TextField
                {
                    id: textField
                    Layout.fillWidth: true
                    Layout.preferredHeight: 45
                    verticalAlignment: TextInput.AlignBottom
                    horizontalAlignment: TextInput.AlignLeft
                    Layout.leftMargin: 5

                    Material.theme: QmlInterface.isDarkTheme? Material.Dark:Material.Light
                    Material.foreground: QmlInterface.isDarkTheme? "#f5f5f5":"black"
                    Material.accent: Material.Pink
                    font.pixelSize: 16
                    font.family: montserratFontLoader.name
                    selectByMouse: true
                    // validator: RegExpValidator {regExp: RegExp("[a-zA-Z0-9]+")}
                    validator: RegularExpressionValidator { regularExpression: /^[a-zA-Z0-9%-]+( [a-zA-Z0-9%-]+)*$/} // "[a-zA-Z0-9]+[\s]+")}

                    placeholderText: qsTr("Enter item Name")

                    onTextEdited: {
                        CompleterModel.addCompleterItems(text);
                    }

                }
            }
        }

        Rectangle {
            id: completionsBox
            Layout.fillWidth: true
            Layout.fillHeight: true
            Material.elevation: 6

            color: QmlInterface.isDarkTheme? "#29292d":"white"
            border.width: 1
            border.color: QmlInterface.isDarkTheme? "transparent":"silver"
            radius: 3

            Item{
                anchors.fill: parent
                visible: CompleterModel.completerSize === 0 && searchText !== ""

                ColumnLayout{
                    anchors.fill: parent

                    ListEmptyState
                    {
                        emptyLabel: qsTr("Couldn't find a match for that word!")
                    }
                }
            }

            AppText
            {
                visible: searchText === "" && CompleterModel.completerSize === 0
                color: QmlInterface.isDarkTheme? "#fefefe":"#2e2e2e"
                text: qsTr("Start typing in the words to search for the item.")
                size: 15
                anchors.centerIn: parent
            }

            ScrollView
            {
                width: completionsBox.width-6
                height: completionsBox.height-6
                anchors.centerIn: parent
                anchors.margins: 3

                clip: true
                spacing: 1

                visible: CompleterModel.completerSize > 0

                ListView
                {
                    id: completions
                    width: parent.width - 5
                    height: parent.height
                    anchors.top: parent.top
                    anchors.left: parent.left
                    currentIndex: -1
                    spacing: 1

                    model: CompleterModel
                    delegate: Component
                    {
                        AutocompleteDelegate
                        {
                            width: completions.width
                            label: model.name
                            labelIndex: model.index
                            sellingprice: model.sp
                            isHighlighted: index === currentIndex

                            onClicked: currentIndex = index
                        }
                    }
                }
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: 45

            CheckBox
            {
                text: qsTr("Close dialog after adding item")
                checked: isDialogClosedAfterEachAdd

                // Clear this baggage once i find the bug
                Material.theme: QmlInterface.isDarkTheme? Material.Dark:Material.Light
                Material.foreground: QmlInterface.isDarkTheme? "#f5f5f5":"black"
                Material.accent: Material.Pink
                onCheckStateChanged: isDialogClosedAfterEachAdd=checked
            }

            Rectangle
            {
                anchors.right: close_btn.left
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter

                color: menuColor
                opacity: currentIndex === -1? 0.4:1
                enabled: currentIndex !== -1
                radius: 5
                height: 35
                width: 120

                RowLayout
                {
                    anchors.centerIn: parent
                    spacing: 10

                    AppText
                    {
                        Layout.alignment: Qt.AlignVCenter

                        text: qsTr("Add")
                        color: "white"
                    }

                    AppIcon
                    {
                        color: "white"
                        size: 16
                        icon: "\uf217"

                        Layout.alignment: Qt.AlignVCenter
                    }
                }

                MouseArea
                {
                    anchors.fill: parent
                    onClicked: {
                        StockItemModel.getItemData(CompleterModel.getKey(currentIndex))

                    }
                }
            }

            Rectangle
            {
                id: close_btn
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter

                color: "#FF605C"
                radius: 5
                height: 35
                width: 80
                border.width: 1
                border.color: "silver"


                AppText
                {
                    anchors.centerIn: parent

                    text: qsTr("CLOSE")
                    color: "white"
                }


                MouseArea
                {
                    anchors.fill: parent
                    onClicked: {
                        searchText = "";
                        myWindow.close();
                    }
                }
            }
        }
    }

    Connections
    {
        target: StockItemModel

        function onItemStockWarningChanged()
        {
            logger.debug(">> Stock is less to complete this add!")
            ToastModel.addToastItem("error", "Stock is less to add to Checkout")
        }

        function onItemDataChanged(status, jsonObj)
        {
            if(status)
            {
                logger.debug(">> Item Added to Checkout Model!")
                CheckoutModel.addSellItem(jsonObj["sellBarcode"], jsonObj["sellItemName"], jsonObj["sellItemUnit"], jsonObj["buyingPrice"], jsonObj["sellingPrice"], 1);

                searchText = "";
                currentIndex = -1;
                CompleterModel.addCompleterItems("");

                if(isDialogClosedAfterEachAdd)
                    myWindow.close()
            }

            else
            {
                logger.debug(">> Failed to Add Item to Checkout Model!")
                ToastModel.addToastItem("error", "Failed to Add Item to Checkout Model")
            }
        }


    }
}
