import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import "../components"

Popup
{
    id: root
    width: 450
    height: 550

    x: (mainApp.width - width)/2
    y: (mainApp.height - height)/2
    modal: true
    closePolicy: Popup.NoAutoClose

    property bool isNewItemMode
    property int currentIndex: -1

    property alias barCode: barcode.text
    property alias itemName: name.text
    property alias itemUnit: unit.text
    property alias itemBp: bp.text
    property alias itemSp: sp.text
    property alias itemQty: qty.text
    property alias itemCompany: company.text
    property string typeId: ""

    contentItem: Rectangle
    {
        color: QmlInterface.isDarkTheme? "#1e2027":bgColor
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
                    color: QmlInterface.isDarkTheme? "#f4f4f4":"black"
                    height: 3
                    width: parent.width
                    opacity: 0.08

                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 5
                }

                AppText
                {
                    color: QmlInterface.isDarkTheme? "#f4f4f4":"black"
                    size: 17
                    text: isNewItemMode? qsTr("New Item Window"):qsTr("Update Item")

                    anchors.centerIn: parent
                }
            }

            Item
            {
                Layout.fillWidth: true
                Layout.fillHeight: true

                ColumnLayout
                {
                    anchors.fill: parent
                    spacing: 10

                    AppTextInput
                    {
                        id: barcode
                        prefWidth: 150
                        label: qsTr("Item Barcode")
                        hintText: qsTr("Scan/enter barcode")
                        validator: RegularExpressionValidator { regularExpression: /[a-zA-Z0-9]+/}
                        readOnly: true
                    }

                    AppTextInput
                    {
                        id: name
                        prefWidth: 150
                        label: qsTr("Item Name")
                        hintText: qsTr("Enter item name")
                        // validator: RegularExpressionValidator { regularExpression: /^[a-zA-Z0-9]+( [a-zA-Z0-9]+)*$")} // "[a-zA-Z0-9]+[\s]+")}
                        validator: RegularExpressionValidator { regularExpression: /^[a-zA-Z0-9%-]+( [a-zA-Z0-9%-]+)*$/}
                    }

                    AppTextInput
                    {
                        id: unit
                        prefWidth: 150
                        label: qsTr("Unit")
                        hintText: qsTr("Item unit, ie, 1kg, 1g")
                        validator: RegularExpressionValidator { regularExpression: /^[a-zA-Z0-9]+( [a-zA-Z0-9]+)*$/}
                    }

                    AppTextInput
                    {
                        id: bp
                        prefWidth: 150
                        label: qsTr("Buying price")
                        hintText: qsTr("Enter bp in Ksh.")
                        validator: IntValidator {bottom: 1; top: 1000000}
                    }

                    AppTextInput
                    {
                        id: sp
                        prefWidth: 150
                        label: qsTr("Selling price")
                        hintText: qsTr("Enter sp in Ksh.")
                        validator: IntValidator {bottom: 1; top: 1000000}
                    }

                    AppCombo
                    {
                        id: category
                        label: qsTr("Item Category")
                        model: ["Vet products","Pesticides","Mineral suppliments","Hygene products","Dewormer","Poultry products","Injectables","Seeds","Feeds","Herbicide","Accaricides","Fertilizers","Vaccine","Fungicides","injectables"]
                        // currentIndex: isNewItemMode? 0:
                    }

                    AppTextInput
                    {
                        id: qty
                        visible: isNewItemMode
                        prefWidth: 150
                        label: qsTr("Quantity")
                        hintText: qsTr("Enter quantity added")
                        validator: IntValidator {bottom: 1; top: 1000000}
                    }

                    AppTextInput
                    {
                        id: company
                        prefWidth: 150
                        label: qsTr("Item's Company")
                        hintText: qsTr("Manufacturing company")
                        validator: RegularExpressionValidator { regularExpression: /^[a-zA-Z0-9%-]+( [a-zA-Z0-9%-]+)*$/ }
                    }

                    Item
                    {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30

                        RowLayout
                        {
                            anchors.fill: parent
                            spacing: 10

                            Rectangle
                            {
                                Layout.fillHeight: true
                                Layout.preferredWidth: 100

                                color: 'red'
                                radius: 3
                                visible: !isNewItemMode && loggedUser_canDeleteItems && isAdmin

                                AppText
                                {
                                    color: "white"
                                    size: 15
                                    text: qsTr("Delete")

                                    anchors.centerIn: parent
                                }

                                MouseArea
                                {
                                    anchors.fill: parent
                                    onClicked: {
                                        // To delete item
                                        StockItemModel.deleteItem(barCode)
                                    }
                                }
                            }

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
                                Layout.preferredWidth: 150

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
                                        text: isNewItemMode? qsTr("Add Item"):qsTr("Update Item")
                                    }
                                }

                                MouseArea
                                {
                                    anchors.fill: parent
                                    onClicked: {
                                        var _barcode = barCode
                                        var _name = itemName
                                        var _unit = itemUnit
                                        var _bp = itemBp
                                        var _sp = itemSp
                                        var _qty = itemQty
                                        var _company = itemCompany

                                        if(_barcode!=="" && _name !== "" && _unit!=="" && _bp!=="" && _sp!=="" && _qty!=="")
                                        {
                                            if(isNewItemMode)
                                                addNewItemToDb(_barcode,_name,_unit,_bp,_sp,_qty,_company);

                                            else
                                                updateItemInDb(_barcode,_name,_unit,_bp,_sp,_qty,_company);

                                        }

                                        else
                                        {
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

    function addNewItemToDb(_barcode,_name,_unit,_bp,_sp,_qty,_company)
    {
        var tmspt = new Date().getTime();
        StockItemModel.addNewItem(_barcode,_name, _unit, _bp, _sp, parseInt(_qty), _company, tmspt, category);
    }

    function updateItemInDb(_barcode,_name,_unit,_bp,_sp,_qty,_company)
    {
        StockItemModel.updateItem(_barcode, _name, _unit, _bp, _sp, _company, category, barCode, currentIndex);
    }

    Connections
    {
        target: StockItemModel

        function onItemUpdatedChanged(status)
        {
            if(status)
            {
                console.log(" [INFO] Item Updated Successfuly!");
                ToastModel.addToastItem("info", "Item updated successfully!")
                root.close();
            }

            else
            {
                console.log(" [ERROR] Error Updating item");
                ToastModel.addToastItem("error", "Error updating item")
            }
        }

        function onItemDeletedChanged(status)
        {
            if(status)
            {
                console.log(" [INFO] Item Deleted Successfuly!");
                ToastModel.addToastItem("info", "Item deleted successfully!")
                root.close();
            }

            else
            {
                console.log(" [ERROR] Error Deleting item");
                ToastModel.addToastItem("error", "Error deleting item")
            }
        }

        function onItemAddingChanged(status)
        {
            if(status)
            {
                console.log(" [INFO] Item Added Successfuly!");
                ToastModel.addToastItem("info", "Item added successfully!")
                root.close();
            }

            else
            {
                console.log(" [ERROR] Error Adding item");
                ToastModel.addToastItem("error", "Error adding item")
            }
        }

        function onCategoryIdChanged(l_id)
        {

        }

        function onCategoryNamesChanged(l_name)
        {
            // category.model = StockItemModel.getCategryList();
            var x = StockItemModel.categoryNames

            for(var i=0; i<x.length; i++)
            {
                //console.log(">> ", x[i])
            }
        }
    }
}
