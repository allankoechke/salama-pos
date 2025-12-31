import QtQuick 2.0
import me.devqt.Controls 1.0

Item {

    id: backupRoot

    Process
    {
        id: process

        authorize: ""
        command: 'pg_dump'
        args: "--dbname=postgresql://postgres:postgresdb@127.0.0.1:5432/salama"

        onReadyRead: {
            logger.debug("Data: ", data)
        }

        onReadyReadError: {
            logger.debug("Error: ", error)
        }
    }

    function beginLocalBackup()
    {
        // Dump the postgres database to a local file
        // Hash the file and store the checksum key for security

        var dt = Qt.formatDate(new Date(), "yyyy-MM-dd")

        process.execute('-f/home/lalanke/'+dt+'.sql');
    }

    function beginOnlineBackup()
    {
        // Transfer local backups to an online store
    }
}
