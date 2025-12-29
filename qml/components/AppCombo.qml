import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

Item
{
    Layout.fillWidth: true
    Layout.preferredHeight: 56  // Material Design standard height

    property alias currentIndex: combo.currentIndex
    property alias label: textLabel.text
    property alias model: combo.model

    RowLayout
    {
        anchors.fill: parent
        spacing: 16  // Material Design spacing

        Item
        {
            Layout.fillHeight: true
            Layout.preferredWidth: 150

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

        ComboBox
        {
            id: combo
            Layout.minimumHeight: 56
            Layout.fillWidth: true
            Layout.fillHeight: true

            // Material Design styling
            Material.accent: Material.primary
            Material.foreground: Material.theme === Material.Dark ? Material.foreground : "#212121"
            font.pixelSize: 16
            font.family: montserratFontLoader.name
        }

        Button
        {
            Layout.fillHeight: true
            Layout.preferredWidth: height
            Layout.maximumWidth: height
            Material.background: Material.accent
            Material.foreground: "white"
            Material.elevation: 2

            AppIcon
            {
                color: "white"
                size: 15
                icon: "\uf067"
                anchors.centerIn: parent
            }
        }
    }
}
