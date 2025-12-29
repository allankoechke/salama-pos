import QtQuick 2.3
import QtQuick.Layouts 1.3
import "../components"

Item {
    id: root
    Layout.fillWidth: true
    Layout.fillHeight: true

    property bool isAnimationRunning: popupTimer.running

    RotationAnimation {
        target: ico
        from: 0
        to: 360
        duration: 1000
        loops: RotationAnimation.Infinite
        running: isAnimationRunning
    }

    Column {
        id: _col
        spacing: 20
        anchors.centerIn: parent

        Item {
            id: logo
            width: root.width * 0.5
            height: width

            // y: (parent.height - height) / 3
            anchors.horizontalCenter: parent.horizontalCenter

            Image {
                source: "qrc:/assets/images/3.png"
                height: parent.height * 0.75
                fillMode: Image.PreserveAspectFit

                anchors.centerIn: parent
            }
        }

        AppText {
            color: "#ff8900"
            text: qsTr("Salama P.O.S.")
            font.pixelSize: 30
            anchors.horizontalCenter: parent.horizontalCenter
            // anchors.top: logo.bottom
        }

        Item {
            id: p
            height: 40
            width: parent.width

            // anchors.bottom: parent.bottom
            // anchors.bottomMargin: 20

            RowLayout {
                anchors.centerIn: parent
                spacing: 10

                AppIcon {
                    id: ico
                    color: "#00a4c7"
                    size: 30
                    icon: "\uf110"

                    Layout.alignment: Qt.AlignVCenter
                }

                AppText {
                    color: "#00a4c7"
                    text: qsTr("Loading ...")
                    font.pixelSize: 20

                    Layout.alignment: Qt.AlignVCenter
                }
            }
        }

        AppText {
            color: QmlInterface.isDarkTheme ? "#999fa6" : "#2e2e2e"
            text: qsTr("(c) 2018-") + (new Date()).getFullYear() + " Codeart Studios"
            font.pixelSize: 13

            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
