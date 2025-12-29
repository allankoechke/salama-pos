import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

Button {
    id: root
    enabled: !busy

    property string backgroundColor: menuColor
    property string foregroundColor: "#fff"
    property alias textIcon: ico
    property bool busy: false
    property bool useMaterialDesign: true

    // Use Material Design button style
    Material.background: useMaterialDesign ? Material.accent : backgroundColor
    Material.foreground: foregroundColor
    Material.elevation: enabled ? (pressed ? 2 : (hovered ? 4 : 2)) : 0

    contentItem: Item {
        BusyIndicator {
            visible: busy
            running: busy
            height: 32
            width: 32
            anchors.centerIn: parent
            Material.accent: foregroundColor
        }

        Row {
            spacing: 8
            anchors.centerIn: parent
            visible: !busy

            AppIcon
            {
                id: ico
                visible: icon !== ""
                color: foregroundColor
                size: 15
                anchors.verticalCenter: parent.verticalCenter
            }

            AppText
            {
                color: foregroundColor
                size: 15
                text: root.text
                anchors.verticalCenter: parent.verticalCenter
                useMaterialColors: false
            }
        }
    }
}
