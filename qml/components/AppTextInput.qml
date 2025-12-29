import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

Item
{
    Layout.fillWidth: true
    Layout.preferredHeight: implicitHeight
    implicitHeight: 56  // Material Design standard height

    property alias label: textLabel.text
    property alias readOnly: textInput.readOnly
    property real prefWidth: 0
    property alias text: textInput.text
    property alias validator: textInput.validator
    property alias hintText: textInput.placeholderText
    property alias echoMode: textInput.echoMode
    property alias labelSize: textLabel.size
    property alias inputSize: textInput.font.pixelSize

    RowLayout
    {
        anchors.fill: parent
        spacing: 16  // Material Design spacing

        Item
        {
            Layout.fillHeight: true
            Layout.preferredWidth: prefWidth===0? textLabel.width:prefWidth

            AppText
            {
                id: textLabel
                color: Material.theme === Material.Dark ? Material.foreground : "#212121"
                size: 15
                useMaterialColors: false

                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
            }
        }

        TextField
        {
            id: textInput
            Layout.fillWidth: true
            Layout.fillHeight: true
            verticalAlignment: TextField.AlignVCenter
            horizontalAlignment: TextField.AlignLeft

            // Material Design styling
            Material.accent: Material.primary
            Material.foreground: Material.theme === Material.Dark ? Material.foreground : "#212121"
            selectByMouse: true
            font.pixelSize: 16
            font.family: montserratFontLoader.name
        }
    }
}
