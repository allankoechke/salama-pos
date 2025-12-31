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

    property int currentButtonIndex: 0

    RowLayout
    {
        anchors.fill: parent
        anchors.margins: 40
        spacing: 10

        Rectangle
        {
            color: QmlInterface.isDarkTheme? "#29292d":"white"
            width: 200
            radius: 5

            Layout.fillHeight: true

            ColumnLayout
            {
                anchors.fill: parent
                spacing: 0
                anchors.margins: 3

                SettingsMenuButton
                {
                    isCurrentSelected: currentButtonIndex===0
                    buttonLabel: qsTr("General")
                    buttonIcon: "\uf3f1"

                    onClicked: currentButtonIndex = 0
                }

                SettingsMenuButton
                {
                    isCurrentSelected: currentButtonIndex===1
                    buttonLabel: qsTr("Barcode Scanner")
                    buttonIcon: "\uf488"

                    onClicked: currentButtonIndex = 1
                }

                SettingsMenuButton
                {
                    isCurrentSelected: currentButtonIndex===2
                    buttonLabel: qsTr("Printer")
                    buttonIcon: "\uf02f"

                    onClicked: currentButtonIndex = 2
                }

                SettingsMenuButton
                {
                    isCurrentSelected: currentButtonIndex===3
                    buttonLabel: qsTr("Remote View")
                    buttonIcon: "\uf6ff"

                    onClicked: currentButtonIndex = 3
                }

                VerticalSpacer {}
            }
        }

        Rectangle
        {
            color: QmlInterface.isDarkTheme? "#29292d":"white"
            radius: 5

            Layout.fillHeight: true
            Layout.fillWidth: true

            StackLayout
            {
                anchors.fill: parent
                anchors.margins: 10

                currentIndex: currentButtonIndex

                GeneralSettingsView
                {

                }

                PrinterSettingsView
                {

                }

                BarcodeScannerSettingsView
                {

                }

                RemoteSettingsView
                {

                }
            }
        }
    }

    Connections
    {
        target: QmlInterface

        function onIsDarkThemeChanged()
        {
            logger.info("Theme changed", QmlInterface.isDarkTheme ? "Switched to dark theme" : "Switched to light theme")
        }
    }

}
