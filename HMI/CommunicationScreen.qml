import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.12

Page {
    id:commPage
    property StackView stackView

    ColumnLayout
    {
        Text
        {
            id: name
            text: "Communication Screen"
            color: "black"
            font.bold: true
            font.pixelSize: 16
        }

        Button
        {
            text: "Ping"
            onClicked:
            {
                __datamanage.sendSocketMessageJson("PC","ping","")
            }
        }

        Button
        {
            text: "changePage"
            onClicked:
            {
                __datamanage.sendSocketMessageJson("PC","changePage","2")
            }
        }

        Button
        {
            text: "changeColor"
            onClicked:
            {
                __datamanage.sendSocketMessageJson("PC","changeColor","grey")
            }
        }

    }

}
