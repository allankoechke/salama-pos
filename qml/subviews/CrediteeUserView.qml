import QtQuick 2.2
import QtQuick.Controls 2.4 as Controls2
import QtQuick.Layouts 1.3

import "../components"
import "../delegates"

Item
{
    id: root

    Layout.fillWidth: true
    Layout.fillHeight: true

    property string fname: ""
    property string lname: ""
    property string mobile: ""
    property string idNo: ""
    property int debt: 0

    signal backClicked()

    function loadPaymentHistory()
    {
        crediteePaymentModel.clear();
        CrediteeModel.getPaymentHistory(idNo)
    }

    ListModel
    {
        id: crediteePaymentModel
    }

    ColumnLayout
    {
        anchors.fill: parent
        spacing: 5

        Item{
            Layout.fillWidth: true
            Layout.preferredHeight: 40

            RowLayout
            {
                anchors.fill: parent
                spacing: 10

                Rectangle
                {
                    color: menuColor
                    radius: 4
                    width: 100

                    Layout.fillHeight: true

                    RowLayout
                    {
                        anchors.centerIn: parent
                        spacing: 10

                        AppIcon
                        {
                            color: "#f4f4f4"
                            size: 20
                            icon: "\uf060"

                            Layout.alignment: Qt.AlignVCenter
                        }

                        AppText
                        {
                            color: "#f4f4f4"
                            text: qsTr("Back")

                            Layout.alignment: Qt.AlignVCenter
                        }
                    }

                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked: root.backClicked();
                    }
                }

                HorizontalSpacer{}

                Rectangle
                {
                    color: menuColor
                    radius: 4
                    width: 150

                    Layout.fillHeight: true

                    RowLayout
                    {
                        anchors.centerIn: parent
                        spacing: 10

                        AppIcon
                        {
                            color: "#f4f4f4"
                            size: 20
                            icon: "\uf543"

                            Layout.alignment: Qt.AlignVCenter
                        }

                        AppText
                        {
                            color: "#f4f4f4"
                            text: qsTr("Repay Debt")


                            Layout.alignment: Qt.AlignVCenter
                        }
                    }

                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked: {
                            creditRepayDialog.open()
                            creditRepayDialog.debt = debt;
                            creditRepayDialog.crediteeId = idNo;
                        }
                    }
                }

                Rectangle
                {
                    color: menuColor
                    radius: 4
                    width: 180

                    Layout.fillHeight: true

                    RowLayout
                    {
                        anchors.centerIn: parent
                        spacing: 10

                        AppIcon
                        {
                            color: "#f4f4f4"
                            size: 20
                            icon: "\uf067"

                            Layout.alignment: Qt.AlignVCenter
                        }

                        AppText
                        {
                            color: "#f4f4f4"
                            text: qsTr("Update Creditee")

                            Layout.alignment: Qt.AlignVCenter
                        }
                    }

                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked:
                        {
                            newCrediteePopup.open();
                            newCrediteePopup.isNewCrediteeMode = false;
                            newCrediteePopup.orig_id = idNo;

                            newCrediteePopup.idNo = idNo;
                            newCrediteePopup.mobile = mobile;
                            newCrediteePopup.fname = fname;
                            newCrediteePopup.lname = lname;
                        }
                    }
                }
            }
        }

        AppText
        {
            color: QmlInterface.isDarkTheme? "#f4f4f4":"#2e2e2e"
            text: qsTr("Amount Due: Ksh. ") + debt.toString()
            height: 35; size: 20
            font.bold: true

            Layout.alignment: Qt.AlignHCenter
            verticalAlignment: AppText.AlignVCenter
        }

        AppText
        {
            size: 25
            color: QmlInterface.isDarkTheme? "#f4f4f4":"#2e2e2e"
            text: qsTr("Re-payment History")
            height: 40

            Layout.alignment: Qt.AlignHCenter
            verticalAlignment: AppText.AlignVCenter
        }

        Rectangle
        {
            border.width: 1
            border.color: "grey"
            color: QmlInterface.isDarkTheme? "#29292d":"white"

            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout
            {
                anchors.fill: parent
                anchors.margins: 1
                spacing: 0

                CrediteePaymentsTableHeader
                {}

                Controls2.ScrollView
                {
                    id: scroll
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    clip: true
                    Controls2.ScrollBar.vertical.policy: Controls2.ScrollBar.AlwaysOn

                    ListView
                    {
                        anchors.fill: parent
                        width: scroll.width
                        height: scroll.height

                        model: crediteePaymentModel
                        delegate: Component
                        {
                            id: _delegate

                            CrediteePaymentsTableDelegate
                            {
                                width: scroll.width // - 15
                                no: model.index
                                paid: _paid
                                due: _due
                                date: _date
                            }
                        }
                    }
                }
            }
        }
    }

    Connections
    {
        target: CrediteeModel

        function onPaymentReceived(date, paid, due)
        {
            // logger.debug("History Added: ", date, " ", paid, " ", due)

            crediteePaymentModel.append(JSON.parse('{
            "_paid":' + paid + ',
            "_due":' + due +',
            "_date":"' + date + '" }'));
        }
    }

    Connections
    {
        target: CrediteeModel

        function onDebRepaymentChanged(state)
        {
            if(state)
            {
                // Set new value for the remianing debt value
                debt = debt-creditRepayDialog.debtPaid

                // Reset the amount paid field
                creditRepayDialog.debtPaid = "";

                // Reload the payment history data
                loadPaymentHistory();

                // Close the repay dialog
                creditRepayDialog.close();

                // Reload the Dashboard Graph with data from the DB
                QmlInterface.getDashboardTableData();
            }
        }
    }
}

