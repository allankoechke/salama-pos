import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15

import "../delegates"
import "../models"

Item {
    id: root
    
    // Material Design Snackbar positioning
    // Snackbars appear at the bottom-right of the screen
    anchors.bottom: parent ? parent.bottom : undefined
    anchors.right: parent ? parent.right : undefined
    anchors.bottomMargin: 16
    anchors.rightMargin: 16
    
    width: 344  // Material Design standard width
    height: toastsColumn.height
    visible: mainAppView.navBarIndex !== 7 && mainAppView.navBarIndex !== 8 && ToastModel.size > 0
    
    property alias toastsListView: toastsColumn
    
    // Compatibility function for old .open() calls
    function open() {
        visible = true
    }
    
    function close() {
        visible = false
    }

    Column {
        id: toastsColumn
        width: parent.width
        spacing: 8
        
        Repeater {
            model: ToastModel
            
            ToastDelegate {
                toastId: toast_id
                category: toast_type
                content: toast_text
                width: toastsColumn.width
            }
        }
    }
}

