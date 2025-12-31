import QtQuick 2.0

Item {
    id: logicRoot

    property string url: "http://127.0.0.1:5000/convert"

    function generatePriceList()
    {
        logger.debug("Starting sending request ")
        var d = StockItemModel.generatePriceList();
        var html = _generatePriceList(d);

        ToastModel.addToastItem('info', 'Price List generation beginning ...')

        var req = new XMLHttpRequest();
        req.open("POST", url);
        req.setRequestHeader("Content-Type", "application/json;charset=UTF-8");

        req.onreadystatechange = function()
        {
            if (req.readyState === XMLHttpRequest.DONE)
            {
                if(req.responseText === "")
                {
                    return;
                }

                logger.debug("Request successful!")

                ToastModel.addToastItem('info', 'Price List generated successfully')

                logger.debug(req.responseText)

                var obj = JSON.parse(req.responseText)

                QmlInterface.openLocation(obj['path'])
            }
        }
        req.onerror = function()
        {
            logger.debug("Request failed!")
            ToastModel.addToastItem('error', 'Price List generation failed, try again later.')
        }

        req.send(JSON.stringify({ "html": html }));
    }

    function _generatePriceList(data)
    {
        var dt = new Date();

        var html = '<!DOCTYPE html>

        <head>
            <style>
                *{
                    font-size: 14px;
                }

                .cText{
                    text-align: center;
                    font-size: medium;
                }

                .tff{
                    font-size: 16px;
                }

                .row{
                    display: flex;
                    flex-direction: row;
                }

                .spaceBtn{
                    align-items: center;
                    justify-content: space-between;
                }

                .table {
                    display: flex;
                    width: 100%;
                }

                table {
                    font-family: arial, sans-serif;
                    border-collapse: collapse;
                    width: 100%;
                }

                td, th {
                    border: 1px solid #dddddd;
                    text-align: left;
                    padding: 8px;
                }

                tr:nth-child(even) {
                    background-color: #eeeeee;
                }
            </style>
        </head>

        <body style=" margin-left: 20px; margin-right: 20px;">

            <p class="cText" style="font-weight: bold; font-size: 18px;">CHEBUI AGROVET</p>

            <p class="cText tff">For Better Animal Health And Productivity</p>
            <p class="cText tff">P.O. BOX 32, Kabiyet.</p>

            <hr class="dashed-line"/>

            <div class="row spaceBtn">
                <p style="font-size: 15px;">OFFICIAL PRICELIST '
        html += Qt.formatDate(dt, "yyyy")
        html +='</p>
                <p cstyle="font-size: 15px;t">Generated '
        html += Qt.formatDateTime(dt, "yyyy-MM-dd HH:mm:ss")
        html +='</p>
            </div>

            <div class="table">
                <table style="width:100%;">
                    <tr>
                        <th>No.</th>
                        <th>Item Name</th>
                        <th>Product Unit</th>
                        <th>Buying Price (Ksh.)</th>
                        <th>Selling Price (Ksh.)</th>
                    </tr>
                    '
        html += data
        html += '
                  </table>
            </div>
            <hr class="dashed-line"/>

            <div class="row spaceBtn">
                <p style="font-size: 15px;">&copy;Chebui Agrovet</p>
                <p cstyle="font-size: 15px;t">Salama POS &copy;'
        html += Qt.formatDate(dt, "yyyy")
        html += '(chat@devqt.me)</p>
            </div>

        </body>
        </html>'

        return html;
    }
}
