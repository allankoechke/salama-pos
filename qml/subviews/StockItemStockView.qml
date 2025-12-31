import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4 as Controls2

import "../components"
import "../delegates"
import "../models"

Rectangle {

    id: root
    radius: 2
    border.color: "grey"
    border.width: 1

    Layout.fillWidth: true
    Layout.fillHeight: true

    ColumnLayout
    {
        anchors.fill: parent
        anchors.margins: 0
        spacing: 0

        StockProductStockHeader {}

        Rectangle
        {
            Layout.fillWidth: true
            Layout.preferredHeight: 1
            color: "grey"
        }

        Controls2.ScrollView
        {
            id: scroll
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 1
            clip: true
            Controls2.ScrollBar.horizontal.policy: Controls2.ScrollBar.AlwaysOff
            Controls2.ScrollBar.vertical.policy: Controls2.ScrollBar.AlwaysOn

            ListView
            {
                id: listView
                anchors.fill: parent
                spacing: 0

                model: StockItemModel // StockViewModel {}
                delegate: Component {
                    id: productViewDelegate

                    StockViewDelegate
                    {
                        width: scroll.width
                        _index: model.index
                        itemName: name
                        barCode: barcode
                        _unit: unit
                        qty: quantity
                        lastUpdated: Qt.formatDateTime(date, "dddd dd MMM, yyyy hh:mm ") + qsTr("Hrs")

                        onClicked: {
                            if(loggedUser_canAddStock | loggedUser_canDeleteStock)
                            {
                                listView.currentIndex =  index
                                addRemoveStockPopup.open()
                                addRemoveStockPopup.currentIndex = index
                                addRemoveStockPopup.currentStock = qty;
                                addRemoveStockPopup.barcode = barCode;
                            }

                            else
                            {
                                ToastModel.addToastItem("error", "User has no priviledges to perform action")
                                logger.debug(" [ERROR] User has no priviledges to perform action")
                            }
                        }

                        onHistoryClicked: {
                            stockHistoryWindow.productBarCode = barCode;
                            stockHistoryWindow.item = _unit +" "+itemName
                            stockHistoryWindow.show();
                            stockHistoryWindow.startLoading();
                        }
                    }
                }
            }
        }
    }
}
