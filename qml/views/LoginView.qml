import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import "../components"

Item {
    id: root
    Layout.fillWidth: true
    Layout.fillHeight: true

    property bool isError: false
    property string errorString: ""

    function clearFields()
    {
        _uname.text = "";
        _pswd.text = "";
        errorString = "";
    }

    // Material Design Card Container
    Pane {
        id: loginCard
        width: 420
        height: 520
        anchors.centerIn: parent
        
        Material.elevation: 8
        Material.background: Material.theme === Material.Dark ? "#1E1E1E" : "#FFFFFF"
        padding: 0

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 32
            spacing: 24

            // Logo and Title Section
            Column {
                Layout.topMargin: 16
                Layout.alignment: Qt.AlignHCenter

                spacing: 8

                // Logo
                Image {
                    id: logoImage
                    source: "qrc:/assets/images/6.png"
                    width: 80
                    height: 80
                    fillMode: Image.PreserveAspectFit
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                // App Name
                AppText {
                    text: qsTr("Salama P.O.S.")
                    size: 28
                    font.bold: true
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                // Tagline
                AppText {
                    text: qsTr("Point of sale technology")
                    size: 14
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }

            // Input Fields Section
            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 16

                LoginInputField {
                    id: _uname
                    icon: "\uf007"
                    hintText: qsTr("Username")
                    isCorrect: false
                }

                LoginInputField {
                    id: _pswd
                    icon: "\uf084"
                    hintText: qsTr("Password")
                    isCorrect: false
                    isPassword: true
                }

                // Error Message
                AppText {
                    text: errorString
                    size: 12
                    color: Material.theme === Material.Dark ? "#CF6679" : "#B00020"
                    useMaterialColors: false
                    visible: errorString !== ""
                    Layout.fillWidth: true
                    Layout.topMargin: 4
                }

                // Sign In Button
                Button {
                    id: signInButton
                    Layout.fillWidth: true
                    Layout.preferredHeight: 48
                    Layout.topMargin: 8
                    
                    text: qsTr("Sign In")
                    Material.background: Material.accent
                    Material.foreground: "white"
                    Material.elevation: 2
                    
                    font.pixelSize: 14
                    font.bold: true
                    
                    onClicked: {
                        if(QmlInterface.databaseLoaded)
                        {
                            var uname = _uname.text
                            var pswd = _pswd.text

                            if(uname !== "" && pswd !== "")
                            {
                                AccountsModel.loginUser(uname, pswd);
                            }
                            else
                            {
                                isError = true;
                                errorString = qsTr("Please enter both username and password")
                                AlarmsModel.addAlarmItem("error", "Empty or short Fields!")
                            }
                        }
                        else
                        {
                            AlarmsModel.addAlarmItem("error", QmlInterface.databaseConnectionErrorString)
                        }
                    }
                }

                // Create Account Link (hidden for now)
                AppText {
                    text: qsTr("Don't have an account? Create One")
                    size: 12
                    color: Material.accent
                    useMaterialColors: false
                    visible: false
                    Layout.alignment: Qt.AlignHCenter
                    Layout.topMargin: 8

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            if(QmlInterface.databaseLoaded)
                                userAccountPopup.open()
                            else
                                AlarmsModel.addAlarmItem("error", QmlInterface.databaseConnectionErrorString)
                        }
                    }
                }
            }
        }

        Connections
        {
            target: AccountsModel

            function onLoggingInUsernameStatus(status)
            {
                if(!status)
                {
                    isError = true
                    errorString = qsTr("Invalid login details")
                    console.log("Username doesnt exist")
                    AlarmsModel.addAlarmItem("error", "Invalid login credentials")
                }
            }

            function onLoggingInPasswordStatus(status)
            {
                // console.log("Status: ", status)

                if(!status)
                {
                    isError = true
                    errorString = qsTr("Invalid login details")
                    console.log(" [ERROR] Wrong Password")
                    AlarmsModel.addAlarmItem("error", "Invalid password")
                }

                else
                {
                    console.log(" [INFO] Correct Password")
                    
                    // Access parent MainAppView through Loader
                    var loader = root.parent
                    if(loader && loader.parent) {
                        var mainView = loader.parent.parent
                        while(mainView && mainView.objectName !== "mainAppViewRoot") {
                            mainView = mainView.parent
                        }
                        
                        if(mainView) {
                            mainView.startApp(true)
                            mainView.navBarIndex = 0
                        }
                    }

                    // Load the days sales
                    ProductSalesModel.showTodaysSales();

                    // Load Creditee Accounts
                    CrediteeModel.loadCrediteeAccounts();
                    QmlInterface.check4Update();

                    AlarmsModel.addAlarmItem("info", "Login successful!")
                }
            }

        }
    }
}
