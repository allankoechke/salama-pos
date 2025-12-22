import QtQuick 2.0
import QtQuick.Controls 1.4 as QQC1
import QtQuick.Controls 2.4 as QQC2
import QtQuick.Layouts 1.3
import "../components"

QQC2.Popup
{
    id: root
    width: 400
    height: 500
    modal: true
    closePolicy: "NoAutoClose"
    x: (parent.width-width)/2
    y: (parent.height-height)/2

    signal accepted(var date)

    property alias maximumDate: calendar.maximumDate
    property alias minimumDate: calendar.minimumDate

    background: Rectangle
    {
        color: bgColor
        border.width: 1
        border.color: '#444'
        radius: 5

        ColumnLayout
        {
            anchors.fill: parent
            anchors.margins: 10

            QQC1.Calendar
            {
                id: calendar
                Layout.fillWidth: true
                Layout.fillHeight: true

                maximumDate: new Date()
            }

            Item {
                id: it
                Layout.fillWidth: true
                Layout.preferredHeight: 40

                CustomButton {
                    id: cancelBtn
                    color: menuColor
                    height: parent.height
                    width: 100
                    anchors.right: okayBtn.left
                    anchors.rightMargin: 10

                    AppText
                    {
                        anchors.centerIn: parent
                        color: "#fff"
                        text: qsTr("CANCEL")
                        size: 15
                    }

                    onClicked: root.close();
                }

                CustomButton {
                    id: okayBtn
                    color: menuColor
                    height: parent.height
                    width: 100
                    anchors.right: parent.right

                    AppText
                    {
                        anchors.centerIn: parent
                        color: "#fff"
                        text: qsTr("OK")
                        size: 15
                    }

                    onClicked: {
                        root.accepted(calendar.selectedDate)
                        root.close()
                    }
                }

            }
        }
    }
}
