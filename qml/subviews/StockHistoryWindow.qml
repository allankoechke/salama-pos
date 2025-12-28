import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

import "../delegates"
import "../subviews"
import "../components"

Window {
    id: rootWindow

    property alias x: rootWindow.x
    property alias y: rootWindow.y

    property string item
    property string productBarCode
    property string currentState: 'loading'

    property var historyModel: []

    flags:  Qt.Dialog | Qt.WindowCloseButtonHint
    modality: Qt.ApplicationModal
    color: bgColor
    width: 800
    height: 600
    minimumWidth: 800
    minimumHeight: 500
    title: qsTr("Stock History Dialog")

    onClosing: {
        close.accepted = false
        close.accepted = true
        historyModel = []
    }

    function startLoading()
    {
        // Set the flag to loading and start loading from the database
        currentState = 'loading';

        var items = StockItemModel.loadHistory(productBarCode);
        historyModel = items["data"]
        currentState = 'loaded';
    }


    ColumnLayout
    {
        anchors.fill: parent
        anchors.margins: 30
        spacing: 1

        AppText
        {
            color: QmlInterface.isDarkTheme? "#fefefe":"#2e2e2e"
            text: qsTr("Stock History for: ") + rootWindow.item
            font.bold: true
            horizontalAlignment: AppText.AlignHCenter

            Layout.alignment: Qt.AlignVCenter|Qt.AlignHCenter
            Layout.fillWidth: true
            Layout.bottomMargin: 20
        }

        Rectangle
        {
            radius: 2
            border.color: "grey"
            border.width: 1
            color: 'transparent'

            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout
            {
                anchors.fill: parent
                spacing: 0

                StockHistoryHeader{}

                ListEmptyState
                {
                    visible: historyModel.length === 0
                }

                ListView
                {
                    id: listView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.topMargin: 1
                    visible: historyModel.length !== 0

                    model: historyModel
                    clip: true
                    delegate: Item{
                        height: 45
                        width: listView.width

                        property alias index_no: h_delegate.index_no
                        property alias initialQty: h_delegate.initialQty
                        property alias diffQty: h_delegate.diffQty
                        property alias newQty: h_delegate.newQty
                        property alias isAdding: h_delegate.isAdding
                        property alias dayUpdated: h_delegate.dayUpdated

                        index_no: index
                        initialQty: model.modelData._initialQty
                        diffQty: model.modelData._diffQty
                        newQty: model.modelData._newQty
                        isAdding: model.modelData._isAdding
                        dayUpdated: model.modelData._dayUpdated

                        StockHistoryDelegate
                        {
                            id: h_delegate
                            width: parent.width-2
                            height: parent.height
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                    }
                }
            }
        }
    }
}
