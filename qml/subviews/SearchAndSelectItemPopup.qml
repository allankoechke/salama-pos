import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

import "../delegates"
import "../components"

Popup
{
    id: root

    x: (parent.width-width)/2
    y: (parent.height-height)/2

    property alias searchText: textField.text
    property alias currentIndex: completions.currentIndex

    signal accepted(var obj);

    modal: true
    width: 500
    height: 400

    onClosed: {
        close.accepted = false
        CompleterModel.addCompleterItems("")
        textField.text = ""
        close.accepted = true
    }

    background: Rectangle
    {
        color: bgColor
        border.width: 1
        border.color: '#444'
        radius: 5

        ColumnLayout
        {
            anchors.fill: parent
            anchors.margins: 10
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
                        validator: RegularExpressionValidator {regularExpression: /^[a-zA-Z0-9%-]+( [a-zA-Z0-9%-]+)*$/ } // "[a-zA-Z0-9]+[\s]+")}

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

                CustomButton
                {
                    id: closeBtn
                    anchors.right: okayBtn.left
                    anchors.rightMargin: 10
                    anchors.verticalCenter: parent.verticalCenter

                    color: "#FF605C"
                    radius: 5; height: 45; width: 80


                    AppText
                    {
                        anchors.centerIn: parent

                        text: qsTr("CANCEL")
                        color: "white"
                        size: 15
                    }

                    onClicked: {
                        searchText = "";
                        root.close();
                    }
                }


                CustomButton
                {
                    id: okayBtn
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter

                    color: menuColor
                    opacity: currentIndex === -1? 0.4:1
                    enabled: currentIndex !== -1
                    radius: 5; height: 45; width: 100

                    AppText
                    {
                        text: qsTr("SELECT")
                        color: "white"
                        size: 15

                        anchors.centerIn: parent
                    }

                    onClicked: {
                        var d = CompleterModel.getData(currentIndex)
                        root.accepted(d);
                        searchText = "";
                        root.close();
                    }
                }
            }
        }
    }
}
