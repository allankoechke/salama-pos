import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15

import "../components"

Rectangle {
    id: root

    width: 344  // Material Design Snackbar standard width
    height: Math.max(48, messageText.implicitHeight + 16)  // Minimum 48, expand for long text
    radius: 4
    
    // Material Design colors based on category
    property color snackbarColor: {
        if (category === "error") {
            return Material.theme === Material.Dark ? "#CF6679" : "#B00020"
        } else if (category === "warning") {
            return Material.theme === Material.Dark ? "#FFB74D" : "#F57C00"
        } else { // info
            return Material.theme === Material.Dark ? "#81C784" : "#388E3C"
        }
    }
    
    color: snackbarColor

    property string alarmId
    property alias content: messageText.text
    property string category

    Timer {
        id: alarmTimer
        repeat: false
        interval: category === "error" ? 5000 : category === "info" ? 3500 : 3500
        running: true

        onTriggered: AlarmsModel.removeAlarmItem(alarmId)
    }

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 16
        anchors.rightMargin: 8
        spacing: 12

        // Icon
        AppIcon {
            Layout.preferredWidth: 24
            Layout.preferredHeight: 24
            Layout.alignment: Qt.AlignVCenter

            color: "white"
            icon: category === "error" ? "\uf071" : category === "info" ? "\uf05a" : "\uf06a"
            size: 20
        }

        // Message text
        AppText {
            id: messageText
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignVCenter
            
            text: root.content
            color: "white"
            useMaterialColors: false
            wrapMode: Text.WordWrap
            elide: Text.ElideRight
            size: 14
        }

        // Close button
        Button {
            Layout.preferredWidth: 24
            Layout.preferredHeight: 24
            Layout.alignment: Qt.AlignVCenter
            
            flat: true
            Material.foreground: "white"
            
            AppIcon {
                anchors.centerIn: parent
                color: "white"
                size: 16
                icon: "\uf00d"
            }
            
            onClicked: {
                if (alarmTimer.running === true)
                    alarmTimer.running = false
                AlarmsModel.removeAlarmItem(alarmId)
            }
        }
    }
}
