import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4 as Controls2

import "../components"

Controls2.Popup
{
    id: root
    width: col.width + 60
    height: col.height + 60

    x: (mainApp.width - width)/2
    y: (mainApp.height - height)/2

    modal: true
    closePolicy: Controls2.Popup.NoAutoClose

    property bool isError: false
    property string errorText: ""
    property alias passwordText: password.text
    property alias confirmPasswordText: confirmpassword.text
    property alias currentPasswordText: currentpassword.text

    QtObject {
        id: internal
        property bool isAwaitingUpdatePasswordRequest: false
    }

    onClosed: resetFields();

    background: Rectangle {
        color: QmlInterface.isDarkTheme? "#1e2027":bgColor
        anchors.fill: parent
    }

    contentItem: Item {
        anchors.fill: parent

        Column
        {
            id: col
            width: 500
            anchors.centerIn: parent

            Item
            {
                width: parent.width
                height: 40

                Rectangle
                {
                    color: QmlInterface.isDarkTheme ? "#999fa6" : "black"
                    height: 3
                    width: parent.width
                    opacity: 0.08

                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 5
                }

                AppText
                {
                    color: QmlInterface.isDarkTheme? "#999fa6":"#2e2e2e"
                    size: 17
                    text: qsTr("Password Reset")

                    anchors.centerIn: parent
                }
            }

            Item
            {
                width: parent.width
                height: fieldscol.height

                Column
                {
                    id: fieldscol
                    width: parent.width
                    spacing: 10

                    Item { height: 8; width: 1}

                    AppTextInput
                    {
                        id: currentpassword
                        width: parent.width
                        height: 40
                        prefWidth: 200
                        label: qsTr("Current Password")
                        hintText: qsTr("Original Password")
                        echoMode: TextInput.Password
                    }

                    AppTextInput
                    {
                        id: password
                        width: parent.width
                        height: 40
                        prefWidth: 200
                        label: qsTr("New Password")
                        hintText: qsTr("Pick a new password")
                        echoMode: TextInput.Password
                    }

                    AppTextInput
                    {
                        id: confirmpassword
                        width: parent.width
                        height: 40
                        prefWidth: 200
                        label: qsTr("Confirm Password")
                        hintText: qsTr("Re-enter new password")
                        echoMode: TextInput.Password
                    }

                    AppText
                    {
                        id: err
                        color: "red"
                        text: !isError ? "" : errorText
                        width: parent.width
                        height: visible ? 1 : 25
                        visible: isError
                        horizontalAlignment: AppText.AlignHCenter
                        verticalAlignment: AppText.AlignVCenter
                    }

                    Item
                    {
                        width: parent.width
                        height: 48

                        RowLayout
                        {
                            anchors.fill: parent
                            spacing: 10

                            HorizontalSpacer {}

                            AppButton
                            {
                                // radius: 3
                                Layout.fillHeight: true
                                Layout.preferredWidth: 100
                                text: qsTr("Cancel")
                                // bgRect.color: "transparent"
                                // bgRect.border {
                                //     width: 1
                                //     color: backgroundColor
                                // }
                                enabled: !internal.isAwaitingUpdatePasswordRequest

                                onClicked: root.close()
                            }

                            AppButton
                            {
                                // radius: 3
                                Layout.fillHeight: true
                                Layout.preferredWidth: 100
                                text: qsTr("Update")
                                // textIcon.icon: "\uf067"
                                busy: internal.isAwaitingUpdatePasswordRequest

                                onClicked: resetPassword();
                            }
                        }
                    }
                }
            }
        }
    }

    function resetPassword() {
        internal.isAwaitingUpdatePasswordRequest = true;

        if(currentPasswordText.trim().length<4)
        {
            internal.isAwaitingUpdatePasswordRequest = false;
            isError = true;
            errorText = qsTr("Current password cannot be empty")
            console.log("Current password cannot be empty")
            return;
        }

        if(passwordText.trim().length<4 || confirmPasswordText !== passwordText)
        {
            internal.isAwaitingUpdatePasswordRequest = false;
            isError = true;
            errorText = qsTr("New Password do not match, or is short")
            console.log("New Password do not match, or is short")
            ToastModel.addToastItem("error", "New Password do not match, or is short!")
            return;
        }


        isError = false
        AccountsModel.updatePassword(loggedUser_username, currentPasswordText, passwordText);

    }

    function resetFields()
    {
        isError = false;
        passwordText=""
        confirmPasswordText=""
        currentPasswordText=""
    }

    Connections {
        target: AccountsModel

        function onUserPasswordChanged(ok) {
            internal.isAwaitingUpdatePasswordRequest = false;
            if(ok) {
                ToastModel.addToastItem("success", "Password updated, logging you out!")

                navBarIndex = 8;
                loginView.clearFields();
                navBar.visible = false;
                notificationBar.visible = false;
                root.close();
            }
        }

        function onUserPasswordChangeError(reason) {
            internal.isAwaitingUpdatePasswordRequest = false;
            ToastModel.addToastItem("error", reason)
        }
    }
}
