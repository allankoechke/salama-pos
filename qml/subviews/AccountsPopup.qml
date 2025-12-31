import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4 as Controls2

import "../components"

Controls2.Popup
{
    id: root
    width: 450
    height: 520

    x: (mainApp.width - width)/2
    y: (mainApp.height - height)/2
    modal: true
    closePolicy: Controls2.Popup.NoAutoClose

    property int currentIndex: -1
    property string currentUser: ""

    property alias canAddUsers: addUsers.isChecked
    property alias canRemoveUsers: deleteUsers.isChecked
    property alias canAddItems: addProducts.isChecked
    property alias canRemoveItems: deleteProducts.isChecked
    property alias canAddStock: addStock.isChecked
    property alias canDeleteStock: deleteStock.isChecked
    property alias canUndoSales: deleteSales.isChecked
    property alias canBackupDb: backup.isChecked

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
                    color: "black"
                    height: 3
                    width: parent.width
                    opacity: 0.08

                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 5
                }

                AppText
                {
                    color: QmlInterface.isDarkTheme? "#999fa6":"black"
                    size: 17
                    text: qsTr("Edit User Accounts")

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

                    AppToggle
                    {
                        id: addUsers
                        prefWidth: 150
                        label: qsTr("Can Add New User Accounts")
                    }

                    AppToggle
                    {
                        id: deleteUsers
                        prefWidth: 150
                        label: qsTr("Can Remove User Accounts")
                    }

                    AppToggle
                    {
                        id: addProducts
                        prefWidth: 150
                        label: qsTr("Can Add New Products")
                    }

                    AppToggle
                    {
                        id: deleteProducts
                        prefWidth: 150
                        label: qsTr("Can Remove/Delete Products")
                    }

                    AppToggle
                    {
                        id: addStock
                        prefWidth: 150
                        label: qsTr("Can Add Stock")
                    }

                    AppToggle
                    {
                        id: deleteStock
                        prefWidth: 150
                        label: qsTr("Can Remove/Delete Stock")

                    }

                    AppToggle
                    {
                        id: deleteSales
                        prefWidth: 150
                        label: qsTr("Can Undo/Delete Sales")
                    }

                    AppToggle
                    {
                        id: backup
                        prefWidth: 150
                        label: qsTr("Can Backup the Database")
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

                                color: "#b8352c"
                                radius: 3
                                visible: loggedUser_username !== currentUser
                                enabled: loggedUser_canDeleteAccounts || isAdmin
                                opacity: enabled? 1:0.2

                                AppText
                                {
                                    color: "white"
                                    size: 15
                                    text: qsTr("Delete User")

                                    anchors.centerIn: parent
                                }

                                MouseArea
                                {
                                    anchors.fill: parent
                                    onClicked: AccountsModel.removeUserAccount(currentUser, currentIndex);
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
                                        icon: "\uf560"
                                    }

                                    AppText
                                    {
                                        color: "white"
                                        size: 15
                                        text: qsTr("Update Roles")
                                    }
                                }

                                MouseArea
                                {
                                    anchors.fill: parent
                                    onClicked: {
                                        // logger.debug("[QMl-Info] ", addUsers.isChecked, " : ", deleteUsers.isChecked, " : ", addProducts.isChecked, " : ", deleteProducts.isChecked, " : ", addStock.isChecked, " : ", deleteStock.isChecked, " : ", deleteSales.isChecked, " : ", backup.isChecked)
                                        AccountsModel.updateUserAccount(currentUser, canAddUsers, canRemoveUsers, canAddItems, canRemoveItems, canAddStock, canDeleteStock, canUndoSales, canBackupDb);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Connections
    {
        target: AccountsModel

        function onUserRemovedChanged(status)
        {
            if(status)
            {
                logger.debug(" [INFO] User deleted successfully!");
                ToastModel.addToastItem("info", "User deleted successfully!")
                root.close();
            }

            else
            {
                logger.debug(" [ERROR] Error Deleting User");
                ToastModel.addToastItem("error", "Error deleting user")
            }
        }

        function onUserPriviledgesChanged(status)
        {
            if(status)
            {
                logger.debug(" [INFO] User priviledges updated Successfuly!");
                ToastModel.addToastItem("info", "User privileges updated")
                root.close();
            }

            else
            {
                logger.debug(" [ERROR] Error Updating User Priviledges");
                ToastModel.addToastItem("error", "User priviledges update failed")
            }
        }
    }
}
