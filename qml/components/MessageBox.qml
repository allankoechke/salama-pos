import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15

Dialog {
    id: root
    modal: true
    closePolicy: Dialog.NoAutoClose
    
    // Material Design dialog sizing
    width: Math.min(560, mainApp.width * 0.9)
    height: implicitHeight
    
    // Center on screen
    x: (mainApp.width - width) / 2
    y: (mainApp.height - height) / 2
    
    property alias text: messageText.text
    property string dialogType: "info"  // "info", "warning", "error", "question"
    
    // Material Design styling
    Material.theme: Material.theme
    // Material.accent: Material.accent
    
    // Title bar (optional, can be hidden if not needed)
    header: Item {
        visible: false
        height: 0
    }
    
    contentItem: ColumnLayout {
        spacing: 0
        
        // Message content area
        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: messageText.implicitHeight + 48
            Layout.leftMargin: 24
            Layout.rightMargin: 24
            Layout.topMargin: 24
            
            AppText {
                id: messageText
                width: parent.width
                anchors.verticalCenter: parent.verticalCenter
                wrapMode: Text.WordWrap
                size: 16
                color: Material.theme === Material.Dark ? Material.foreground : "#212121"
                useMaterialColors: false
            }
        }
        
        // Divider
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 1
            color: Material.theme === Material.Dark ? "#3A3A3A" : "#E0E0E0"
        }
        
        // Action buttons
        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 52
            spacing: 0
            
            // Cancel button (if dialogType is "question")
            Button {
                visible: dialogType === "question"
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredWidth: visible ? undefined : 0
                
                text: qsTr("CANCEL")
                flat: true
                Material.foreground: Material.accent
                font.pixelSize: 14
                font.bold: true
                
                onClicked: {
                    root.declined()
                    root.close()
                }
            }
            
            // Divider between buttons
            Rectangle {
                visible: dialogType === "question"
                Layout.fillHeight: true
                Layout.preferredWidth: 1
                color: Material.theme === Material.Dark ? "#3A3A3A" : "#E0E0E0"
            }
            
            // OK/Accept button
            Button {
                Layout.fillWidth: true
                Layout.fillHeight: true
                
                text: qsTr("OK")
                flat: true
                Material.foreground: Material.accent
                font.pixelSize: 14
                font.bold: true
                
                onClicked: {
                    root.accepted()
                    root.close()
                }
            }
        }
    }
    
    // Material Design background
    background: Rectangle {
        color: Material.theme === Material.Dark ? "#1E1E1E" : "#FFFFFF"
        radius: 4
    }
}
