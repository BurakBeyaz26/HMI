import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.12

Column {

    property StackView stackView
    Text
    {
        leftPadding: 25
        text: "HMI"
        style: Text.Raised
        font.pointSize: 10
        anchors.horizontalCenter: parent.horizontalCenter
    }
    Button
    {
        onClicked:
        {
            stackView.push(mainView)
        }
    }
}
