import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    Layout.fillWidth: true
    Layout.fillHeight: true

    property alias value: inputBox.text
    property alias input: inputBox

    Rectangle
    {
        anchors.fill: parent
        border.width: 1
        border.color: QmlInterface.isDarkTheme? 'grey':'#eee'
        radius: 5; clip: true
        color: 'transparent'

        RowLayout
        {
            anchors.fill: parent
            anchors.margins: 5

            TextField
            {
                id: inputBox
                placeholderText: qsTr("Quantity")
                font.pixelSize: 16
                selectByMouse: true
                font.family: montserratFontLoader.name
                color: QmlInterface.isDarkTheme? "#f4f4f4":"black"
                validator: RegularExpressionValidator { regularExpression: /([0-9]+)/ }

                Layout.fillWidth: true
                Layout.alignment: Qt.AlignVCenter
                Layout.margins: 2
                horizontalAlignment: "AlignLeft"
                verticalAlignment: "AlignVCenter"

                background: Item{ clip: true }
            }
        }
    }
}
