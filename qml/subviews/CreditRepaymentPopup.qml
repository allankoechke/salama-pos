import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import "../components"

Popup
{
    id: root
    width: 450
    height: 270

    x: (mainApp.width - width)/2
    y: (mainApp.height - height)/2
    modal: true
    closePolicy: Popup.NoAutoClose

    property int currentIndex: -1
    property alias debt: amountDueLabel.text
    property alias debtPaid: repayInput.text
    property alias isError: err.visible
    property string crediteeId: ""

    signal accepted()

    onClosed: isError=false;

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
                    color: "black"
                    height: 3
                    width: parent.width
                    opacity: 0.08

                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 5
                }

                AppText
                {
                    color: QmlInterface.isDarkTheme? "#f4f4f4":"#535353"
                    size: 17
                    text: qsTr("Credit Payment Dialog")

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
                        color: QmlInterface.isDarkTheme? "#f4f4f4":"#535353"
                        text: qsTr("Amount Due")

                        Layout.preferredWidth: 200
                        horizontalAlignment: AppText.AlignLeft
                        Layout.alignment: Qt.AlignVCenter
                    }

                    AppText
                    {
                        id: amountDueLabel
                        size: 20
                        color: QmlInterface.isDarkTheme? "#f4f4f4":"#535353"
                        font.bold: true

                        Layout.fillWidth: true
                        horizontalAlignment: AppText.AlignHCenter
                        Layout.alignment: Qt.AlignVCenter
                    }
                }
            }

            AppTextInput
            {
                id: repayInput
                prefWidth: 200
                labelSize: 20
                // inputSize: 20
                label: qsTr("Amount to Repay")
                hintText: qsTr("Amount in Ksh.")
                validator: RegularExpressionValidator { regularExpression: /[0-9]+/ }
            }

            AppText
            {
                id: err
                size: 20
                color: "red"
                visible: false
                text: qsTr("Amount entered exceed Amount owed")

                Layout.preferredWidth: 200
                horizontalAlignment: AppText.AlignLeft
                Layout.alignment: Qt.AlignVCenter
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
                                if(parseInt(debtPaid) > parseInt(debt) || parseInt(debtPaid)===0)
                                    isError=true

                                else
                                {
                                    isError=false
                                    CrediteeModel.repayDebt(crediteeId, debt, debtPaid);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
