import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4 as Controls2

import "../components"

Controls2.Popup
{
    id: root
    x: (mainApp.width - width)/2
    y: (mainApp.height - height)/2

    modal: true
    width: 400
    height: 250

    closePolicy: Controls2.Popup.NoAutoClose

    property alias from: qtySpinbox.from
    property alias to: qtySpinbox.to
    property int value: parseInt(qtySpinbox.value)
    property string barcode

    signal accepted()

    contentItem: Rectangle
    {
        color: bgColor
        anchors.fill: parent


        ColumnLayout
        {
            anchors.fill: parent
            anchors.margins: 30

            Item
            {
                Layout.fillWidth: true
                Layout.preferredHeight: 40

                Rectangle
                {
                    color: QmlInterface.isDarkTheme? "white":"black"
                    height: 3
                    width: parent.width
                    opacity: 0.1

                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 5
                }

                AppText
                {
                    color: QmlInterface.isDarkTheme? "#f4f4f4":"black"
                    size: 17
                    text: qsTr("Set Quantity Product Quantity")

                    anchors.centerIn: parent
                }
            }

            Item
            {
                Layout.fillWidth: true
                Layout.preferredHeight: 50

                RowLayout
                {
                    anchors.fill: parent
                    spacing: 10

                    AppText
                    {
                        size: 20
                        color: QmlInterface.isDarkTheme? "#f4f4f4":"black"
                        text: qsTr("New Quantity")

                        Layout.preferredWidth: 200
                        horizontalAlignment: AppText.AlignLeft
                        Layout.alignment: Qt.AlignVCenter
                    }

                    StockQtyInputBox
                    {
                        id: qtySpinbox

                        property real from: 0
                        property real to: 0

                        onValueChanged: {
                            logger.debug('Value changed: ', value, ', Int: ', parseInt(qtySpinbox.value))
                        }
                    }
                }
            }


            Item
            {
                Layout.fillWidth: true
                Layout.preferredHeight: 35

                RowLayout
                {
                    anchors.fill: parent
                    spacing: 10

                    HorizontalSpacer {}

                    Rectangle
                    {
                        Layout.fillHeight: true
                        Layout.preferredWidth: 100

                        color: menuColor
                        radius: 3


                        AppText
                        {
                            color: "white"
                            size: 15
                            text: qsTr("Cancel")

                            anchors.centerIn: parent
                        }

                        MouseArea
                        {
                            anchors.fill: parent
                            onClicked: root.close()
                        }
                    }

                    Rectangle
                    {
                        Layout.fillHeight: true
                        Layout.preferredWidth: 120

                        color: menuColor
                        radius: 3

                        RowLayout
                        {
                            anchors.centerIn: parent

                            spacing: 10

                            AppIcon
                            {
                                color: "white"
                                size: 15
                                icon: "\uf067"
                            }

                            AppText
                            {
                                color: "white"
                                size: 15
                                text: qsTr("Update")
                            }
                        }

                        MouseArea
                        {
                            anchors.fill: parent
                            onClicked: {
                                logger.debug(">> Updating Stock: ", barcode)

                                logger.debug("Value", parseInt(qtySpinbox.value), "From: ", qtySpinbox.from, ", To: ", qtySpinbox.to)

                                if( parseInt(qtySpinbox.value) < from || qtySpinbox.value==='' )
                                    ToastModel.addToastItem("error", "Quantity CANT be ZERO!")

                                else if( parseInt(qtySpinbox.value) > to )
                                    ToastModel.addToastItem("error", "Quantity entered exceeds quantity available!")

                                else
                                    CheckoutModel.changeSellStock(parseInt(qtySpinbox.value), barcode);
                            }
                        }
                    }
                }
            }
        }
    }

    Connections
    {
        target: CheckoutModel

        function onSaleQtyChanged(state)
        {
            if(state)
            {
                logger.debug("Sale Qty Update successful!");
                ToastModel.addToastItem("info", "Quantity added successfully")
                root.close();
            }

            else
            {
                logger.debug("Sale Qty failed!");
                ToastModel.addToastItem("error", "Quantity add failed")
            }
        }
    }
}
