import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts

import "../delegates"
import "../subviews"
import "../components"

Window {
    id: rootWindow

    property alias x: rootWindow.x
    property alias y: rootWindow.y

    flags:  Qt.Dialog //  | Qt.WindowCloseButtonHint
    modality: Qt.ApplicationModal
    color: bgColor
    width: 800
    height: 600
    minimumWidth: 800
    minimumHeight: 500
    title: qsTr("Admin actions on sales data")

    property alias calendarPopup: calendarPopup
    property var selectedDate: new Date()
    property var selectedObject
    property bool isInvalidSelected: selectedObject==="" || selectedObject===undefined
    property var adminTableModel: []

    onClosing: {
        close.accepted = false
        resetFields();
        close.accepted = true
    }

    function resetFields()
    {
        selectedDate=new Date();
        selectedObject=undefined;
    }

    ColumnLayout
    {
        anchors.fill: parent
        anchors.margins: 30
        spacing: 1

        Item{
            Layout.fillWidth: true
            Layout.preferredHeight: 50
            Layout.rightMargin: 20
            Layout.leftMargin: 20

            RowLayout{
                anchors.fill: parent
                spacing: 20

                AppText
                {
                    color: QmlInterface.isDarkTheme? "#fefefe":"#2e2e2e"
                    text: qsTr("Select the Admin action")
                    horizontalAlignment: AppText.AlignLeft

                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignVCenter|Qt.AlignLeft
                }

                ComboBox
                {
                    id: cb
                    height: 45
                    currentIndex: 0
                    editable: false
                    model: ["Undo Sales"]

                    Layout.fillWidth: true
                    Layout.minimumWidth: 200
                    Layout.alignment: Qt.AlignVCenter|Qt.AlignLeft
                }
            }
        }

        Item{
            Layout.fillWidth: true
            Layout.preferredHeight: 50
            Layout.rightMargin: 20
            Layout.leftMargin: 20

            RowLayout
            {
                anchors.fill: parent
                spacing: 20

                Item{
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    AppText
                    {
                        color: QmlInterface.isDarkTheme? "#fefefe":"#2e2e2e"
                        text: qsTr("Select the sales transaction date")
                        horizontalAlignment: AppText.AlignLeft
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                    }
                }

                Item{
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    RowLayout
                    {
                        anchors.fill: parent
                        spacing: 5

                        AppText
                        {
                            color: QmlInterface.isDarkTheme? "#fefefe":"#2e2e2e"
                            text: Qt.formatDate(selectedDate, "dd MMMM yyyy")
                            horizontalAlignment: AppText.AlignLeft

                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignVCenter|Qt.AlignLeft
                        }

                        CustomButton
                        {
                            color: menuColor
                            radius: 5
                            Layout.preferredWidth: 130
                            Layout.preferredHeight: 45

                            AppText
                            {
                                text: qsTr("Change date")
                                color: '#FFF'
                                size: 15
                                anchors.centerIn: parent
                            }

                            onClicked: calendarPopup.open();
                        }
                    }
                }
            }
        }

        Item{
            Layout.fillWidth: true
            Layout.preferredHeight: 50
            Layout.rightMargin: 20
            Layout.leftMargin: 20

            RowLayout
            {
                anchors.fill: parent
                spacing: 20

                Item{
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    AppText
                    {
                        color: QmlInterface.isDarkTheme? "#fefefe":"#2e2e2e"
                        text: qsTr("Select the item")
                        horizontalAlignment: AppText.AlignLeft
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                    }
                }

                Item{
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    RowLayout
                    {
                        anchors.fill: parent
                        spacing: 5

                        AppText
                        {
                            color: QmlInterface.isDarkTheme? "#fefefe":"#2e2e2e"
                            text: isInvalidSelected? qsTr("<None selected>"):selectedObject.name
                            horizontalAlignment: AppText.AlignLeft

                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignVCenter|Qt.AlignLeft
                        }

                        CustomButton
                        {
                            color: menuColor
                            radius: 5
                            Layout.preferredWidth: 130
                            Layout.preferredHeight: 45

                            AppText
                            {
                                text: qsTr("SELECT ITEM")
                                color: '#FFF'
                                size: 15
                                anchors.centerIn: parent
                            }

                            onClicked: searchPopup.open();
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
                id: clearBtn
                height: parent.height
                anchors.right: okBtn.left
                anchors.rightMargin: 10
                width: 100
                radius: 5
                color: menuColor


                onClicked: {
                    selectedObject=undefined
                    selectedDate = new Date()
                    adminTableModel = []
                }

                AppText
                {
                    text: qsTr("CLEAR")
                    color: "#fff"
                    size: 15
                    anchors.centerIn: parent
                }
            }

            CustomButton
            {
                id: okBtn
                height: parent.height
                width: 100
                radius: 5
                anchors.right: parent.right
                color: menuColor

                onClicked: {
                    isInvalidSelected? ToastModel.addToastItem("error", "No valid item selected"):getItems();
                }

                AppText
                {
                    text: qsTr("FIND")
                    color: "#fff"
                    size: 15
                    anchors.centerIn: parent
                }
            }

        }

        Rectangle
        {
            radius: 2
            border.color: "grey"
            border.width: 1
            color: 'transparent'

            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true

            Rectangle
            {
                id: header
                height: 50
                width: parent.width-2
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 1
                color: menuColor

                RowLayout
                {
                    anchors.fill: parent
                    anchors.rightMargin: 10
                    anchors.leftMargin: 10
                    spacing: 1

                    Item
                    {
                        Layout.fillHeight: true
                        Layout.preferredWidth: 50
                    }


                    VSeparator{}

                    Item
                    {
                        Layout.fillHeight: true
                        Layout.fillWidth: true

                        AppText
                        {
                            color: "#fff"
                            text: qsTr("Sales ID")
                            font.bold: true
                            horizontalAlignment: AppText.AlignLeft

                            width: parent.width-20
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            elide: AppText.ElideRight
                        }
                    }

                    VSeparator{}

                    Item
                    {
                        Layout.fillHeight: true
                        Layout.preferredWidth: 150

                        AppText
                        {
                            color: "#fff"
                            text: qsTr("Selling Price")
                            font.bold: true
                            horizontalAlignment: AppText.AlignLeft

                            width: parent.width-20
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            elide: AppText.ElideRight
                        }
                    }

                    VSeparator{}

                    Item
                    {
                        Layout.fillHeight: true
                        Layout.preferredWidth: 100

                        AppText
                        {
                            color: "#fff"
                            text: qsTr("Quantity")
                            font.bold: true
                            horizontalAlignment: AppText.AlignLeft

                            width: parent.width-20
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            elide: AppText.ElideRight
                        }
                    }

                    VSeparator{}

                    Item{
                        Layout.fillHeight: true
                        Layout.preferredWidth: 150

                        AppText
                        {
                            color: "#fff"
                            text: qsTr("ACTION")
                            font.bold: true
                            horizontalAlignment: AppText.AlignLeft

                            width: parent.width-20
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            elide: AppText.ElideRight
                        }
                    }
                }
            }

            Item{
                anchors.fill: parent
                anchors.topMargin: 52
                visible: adminTableModel.length===0

                AppText
                {
                    color: QmlInterface.isDarkTheme? "#fefefe":"#2e2e2e"
                    text: qsTr("Nothing here!")
                    anchors.centerIn: parent
                }
            }

            ListView
            {
                id: lv
                anchors.fill: parent
                anchors.topMargin: 52
                model: adminTableModel
                visible: adminTableModel.length>0
                clip: true
                delegate: Item{
                    id: adminDelegate
                    height: 45
                    width: lv.width

                    property real it_sp
                    property int it_qty
                    property string it_sales_id
                    property int it_index

                    signal accepted(string salesid, string bcode, int q_ty)

                    it_sp: model.modelData.sp
                    it_sales_id: model.modelData.salesId
                    it_qty: model.modelData.qty
                    it_index: index+1

                    onAccepted: {
                        undoSalesPopup.open();
                        undoSalesPopup.salesid = salesid
                        undoSalesPopup.higherLimit = q_ty;
                        undoSalesPopup.barcode = bcode;
                    }

                    Rectangle
                    {
                        height: parent.height
                        width: parent.width-2
                        anchors.horizontalCenter: parent.horizontalCenter // adminDelegate.it_index
                        color: /*isSelected? Qt.lighter(menuColor, 1.3):*/QmlInterface.isDarkTheme? ((adminDelegate.it_index%2)===0? Qt.lighter("#29292d", 1.3):Qt.lighter("#29292d", 1.5)):((adminDelegate.it_index%2)===0? "#eeeeee":"white")

                        RowLayout
                        {
                            anchors.fill: parent
                            anchors.rightMargin: 10
                            anchors.leftMargin: 10
                            spacing: 3

                            Item
                            {
                                Layout.fillHeight: true
                                Layout.preferredWidth: 50

                                AppText
                                {
                                    color: QmlInterface.isDarkTheme? "#fefefe":"#2e2e2e"
                                    text: adminDelegate.it_index.toString()+'.'
                                    horizontalAlignment: AppText.AlignLeft

                                    width: parent.width-10
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.verticalCenter: parent.verticalCenter
                                    elide: AppText.ElideRight
                                }
                            }

                            Item
                            {
                                Layout.fillHeight: true
                                Layout.fillWidth: true

                                AppText
                                {
                                    color: QmlInterface.isDarkTheme? "#fefefe":"#2e2e2e"
                                    text: adminDelegate.it_sales_id
                                    horizontalAlignment: AppText.AlignLeft

                                    width: parent.width-20
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.verticalCenter: parent.verticalCenter
                                    elide: AppText.ElideRight
                                }
                            }

                            Item
                            {
                                Layout.fillHeight: true
                                Layout.preferredWidth: 150

                                AppText
                                {
                                    color: QmlInterface.isDarkTheme? "#fefefe":"#2e2e2e"
                                    text: adminDelegate.it_sp
                                    horizontalAlignment: AppText.AlignLeft

                                    width: parent.width-20
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.verticalCenter: parent.verticalCenter
                                    elide: AppText.ElideRight
                                }
                            }

                            Item
                            {
                                Layout.fillHeight: true
                                Layout.preferredWidth: 100

                                AppText
                                {
                                    color: QmlInterface.isDarkTheme? "#fefefe":"#2e2e2e"
                                    text: adminDelegate.it_qty
                                    horizontalAlignment: AppText.AlignHCenter

                                    width: parent.width-20
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.verticalCenter: parent.verticalCenter
                                    elide: AppText.ElideRight
                                }
                            }

                            Item{
                                Layout.fillHeight: true
                                Layout.preferredWidth: 150

                                CustomButton
                                {
                                    anchors.fill: parent
                                    radius: 5
                                    color: menuColor

                                    onClicked: adminDelegate.accepted(adminDelegate.it_sales_id, selectedObject["key"], adminDelegate.it_qty)

                                    Row
                                    {
                                        anchors.centerIn: parent
                                        spacing: 10

                                        AppText
                                        {
                                            color: '#fff'
                                            size: 15
                                            text: qsTr("MANAGE")
                                            anchors.verticalCenter: parent.verticalCenter
                                        }

                                        AppIcon
                                        {
                                            color: "#fff"
                                            size: 15
                                            icon: '\uf061'
                                            anchors.verticalCenter: parent.verticalCenter
                                        }
                                    }
                                }
                            }
                        }
                    }

                }
            }
        }
    }

    CalendarPopup
    {
        id: calendarPopup

        onAccepted: {
            selectedDate=date
        }
    }

    SearchAndSelectItemPopup
    {
        id: searchPopup

        onAccepted: {
            selectedObject = obj;
        }
    }

    UndoSalesPopup
    {
        id: undoSalesPopup

        onDone: {
            StockItemModel.initializeStockFromDb();
            getItems();
            // Propagate change
            salesView.getData();

            if(adminTableModel.length==0)
            {
                resetFields();
            }
        }
    }

    Connections
    {
        target: SalesModel

        function onEmitError(s,l)
        {
            ToastModel.addToastItem(s,l);
        }
    }

    function getItems()
    {
        var jsonObj = SalesModel.getItemSalesDetails(selectedObject["key"], Qt.formatDate(selectedDate, "yyyy-MM-dd"))

        console.log("JSON Obj: \n\t", JSON.stringify(jsonObj))

        adminTableModel = jsonObj.data
    }
}
