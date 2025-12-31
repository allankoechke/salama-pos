import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import "../components"

Popup
{
    id: root

    width: 400
    height: 250
    x: (parent.width-width)/2
    y: (parent.height-height)/2
    closePolicy: "NoAutoClose"

    property int lowerLimit: 1
    property int higherLimit: 1
    property string salesid
    property string barcode

    onClosed: {
        lowerLimit=1;
        higherLimit=1;
        salesid=""
        barcode=""
        input.text=""
    }

    signal done()

    background: Rectangle
    {
        color: bgColor
        border.width: 1
        border.color: '#444'
        radius: 5

        ColumnLayout
        {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 10

            AppText
            {
                Layout.fillWidth: true
                Layout.preferredHeight: 45
                horizontalAlignment: AppText.AlignLeft
                verticalAlignment: AppText.AlignVCenter

                size: 15
                text: qsTr("Maximum quantity to undo is: ") + higherLimit.toString()
                color: QmlInterface.isDarkTheme? "#F5F5F5":"#2e2e2e"
            }

            Item
            {
                Layout.fillWidth: true
                Layout.preferredHeight: 45

                RowLayout
                {
                    anchors.fill: parent
                    spacing: 10

                    Item{
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        AppText
                        {
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            horizontalAlignment: AppText.AlignLeft

                            text: qsTr("Enter Quantity to undo")
                            color: QmlInterface.isDarkTheme? "#F5F5F5":"#2e2e2e"
                        }
                    }

                    Item{
                        Layout.fillHeight: true
                        Layout.fillWidth: true

                        Rectangle
                        {
                            color: "transparent"
                            radius: 5
                            anchors.fill: parent
                            anchors.margins: 5
                            border.width: 1
                            border.color: '#444'

                            TextInput
                            {
                                id: input
                                height: parent.height * 0.6
                                width: parent.width-20
                                color: QmlInterface.isDarkTheme? "#F5F5F5":"#2e2e2e"
                                font.pixelSize: 15

                                property bool isValid: parseInt(text)<=higherLimit && parseInt(text)>=lowerLimit

                                validator: IntValidator{ bottom: 0; top: 99 }
                                anchors.centerIn: parent

                                AppText
                                {
                                    visible: parent.text===""
                                    color: QmlInterface.isDarkTheme? "grey":"grey"
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.left: parent.left
                                    text: qsTr("Quantity")
                                }
                            }
                        }
                    }
                }
            }

            Item{
                Layout.fillWidth: true
                Layout.preferredHeight: 45

                CustomButton
                {
                    id: cancelBtn
                    color: menuColor
                    height: parent.height
                    width: 100
                    anchors.right: okBtn.left
                    anchors.rightMargin: 10
                    anchors.verticalCenter: parent.verticalCenter

                    onClicked: {
                        input.text=""
                        input.isValid=false
                        root.close();
                    }

                    AppText
                    {
                        text: qsTr("CANCEL")
                        color: "#fff"
                        size: 15
                        anchors.centerIn: parent
                    }
                }

                CustomButton
                {
                    id: okBtn
                    color: menuColor
                    height: parent.height
                    width: 100
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    anchors.verticalCenter: parent.verticalCenter

                    onClicked: {
                        logger.debug("Lower: ", lowerLimit, " - higher: ", higherLimit, " - input: ", input.text)

                        if(input.isValid)
                        {
                            var state = SalesModel.undoSale(barcode,salesid,parseInt(input.text))
                            root.done();
                            root.close();
                        }

                        else
                        {
                            ToastModel.addToastItem("error", "Quantity entered is invalid!")
                        }
                    }

                    AppText
                    {
                        text: qsTr("OK")
                        color: "#fff"
                        size: 15
                        anchors.centerIn: parent
                    }
                }
            }
        }
    }
}
