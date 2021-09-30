import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.12

ColumnLayout
{
    property StackView stackView

    Text {
        id: name
        text: "Home Page"
        color: "black"
        font.bold: true
        font.pixelSize: 16
    }

}
