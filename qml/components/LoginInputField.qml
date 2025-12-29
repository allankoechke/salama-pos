import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import "../components"

Item {
    Layout.fillWidth: true
    Layout.preferredHeight: 56  // Material Design standard height

    property alias hintText: _textField.placeholderText
    property alias text: _textField.text
    property alias icon: ico.icon
    property alias textInput: _textField
    property bool isCorrect: false
    property bool isPassword: false

    RowLayout {
        anchors.fill: parent
        spacing: 16

        AppIcon {
            id: ico
            color: Material.accent
            size: 20
            Layout.alignment: Qt.AlignVCenter
            Layout.preferredWidth: 20
        }

        TextField {
            id: _textField
            Layout.fillWidth: true
            Layout.preferredHeight: 56
            
            // Material Design styling
            Material.accent: Material.primary
            Material.foreground: Material.theme === Material.Dark ? Material.foreground : "#212121"
            
            font.pixelSize: 16
            font.family: montserratFontLoader.name
            echoMode: isPassword ? TextInput.Password : TextInput.Normal
            verticalAlignment: TextInput.AlignVCenter
        }

        AppIcon {
            color: Material.accent
            size: 20
            icon: isCorrect ? "\uf00c" : ""
            visible: isCorrect
            Layout.alignment: Qt.AlignVCenter
        }
    }
}
