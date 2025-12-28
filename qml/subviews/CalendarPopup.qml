import QtQuick 2.0
import QtQuick.Controls
import QtQuick.Layouts 1.3
import "../components"

Popup
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

            Item
            {
                id: calendar
                Layout.fillWidth: true
                Layout.fillHeight: true

                property var maximumDate: new Date()
                property var minimumDate: new Date(2000)

                // TODO ...
                // maximumDate: new Date()

                GridLayout {
                    anchors.fill: parent
                    columns: 2

                    DayOfWeekRow {
                        locale: grid.locale

                        Layout.column: 1
                        Layout.fillWidth: true
                    }

                    WeekNumberColumn {
                        month: grid.month
                        year: grid.year
                        locale: grid.locale

                        Layout.fillHeight: true
                    }

                    MonthGrid {
                        id: grid
                        month: Calendar.December
                        year: 2015
                        locale: Qt.locale("en_US")

                        Layout.fillWidth: true
                        Layout.fillHeight: true
                    }
                }
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
