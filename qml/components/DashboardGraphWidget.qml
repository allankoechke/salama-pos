import QtQuick 2.0
import QtCharts 2.3
// import QwtCharts 1.0

Item {
    id: root
    anchors.fill: parent
    anchors.margins: 1

    ChartView {
        id: chartV
        title: "Weekly Sales Summary"
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        antialiasing: true
        titleColor: QmlInterface.isDarkTheme? "#f4f4f4":"#535353"
        backgroundColor: QmlInterface.isDarkTheme? "#29292d":"white"
        legend.alignment: Qt.AlignBottom
        legend.color: QmlInterface.isDarkTheme? "#f4f4f4":"#535353"

        BarSeries {
            id: myBarSeries
            axisY: ValueAxis {
                min: 0; max: QmlInterface.plotYmax;
                labelsColor: QmlInterface.isDarkTheme? "#f4f4f4":"#535353"
                labelFormat: "%.0f"
                labelsFont:Qt.font({pixelSize: 10})
            }
            axisX: BarCategoryAxis {
                categories: QmlInterface.plotXAxis;
                labelsColor: QmlInterface.isDarkTheme? "#f4f4f4":"#535353"
                labelsFont:Qt.font({pixelSize: 11})
            }

            BarSet
            {
                id: cashBar
                label: "Cash";
                labelColor: QmlInterface.isDarkTheme? "#f4f4f4":"#535353";
                values: [];
                color: "#00bfff"
            }

            BarSet
            {
                label: "M-Pesa";
                labelColor: QmlInterface.isDarkTheme? "#f4f4f4":"#535353";
                values: [];
                color: "#a0522d"
            }

            BarSet
            {
                label: "Cheque";
                labelColor: QmlInterface.isDarkTheme? "#f4f4f4":"#535353";
                values: [];
                color: "#dc143c"
            }

            BarSet
            {
                label: "Credit";
                labelColor: QmlInterface.isDarkTheme? "#f4f4f4":"#535353";
                values: [];
                color: "#d2691e"
            }

            BarSet
            {
                label: "Debt Paid";
                labelColor: QmlInterface.isDarkTheme? "#f4f4f4":"#535353";
                values: [];
                color: "#00ff7f"
            }
        }

    }

    Connections
    {
        target: QmlInterface

        function onDataReadyForPlotting()
        {
            // logger.debug("New plot values");

            let csh = [], mpsa=[], cr=[], chq=[], crp=[];

            for(var i=0; i<7; i++)
            {
                csh[i] = QmlInterface.cashYAxis[i]
                mpsa[i]= QmlInterface.mpesaYAxis[i]
                cr[i] = QmlInterface.creditYAxis[i]
                chq[i] = QmlInterface.chequeYAxis[i]
                crp[i] = QmlInterface.creditPaidYAxis[i]
            }

            myBarSeries.clear();
            myBarSeries.append("Cash", csh)
            myBarSeries.append("M-Pesa", mpsa)
            myBarSeries.append("Cheque", chq)
            myBarSeries.append("Credit", cr)
            myBarSeries.append("Debt Paid", crp)
        }
    }
}
