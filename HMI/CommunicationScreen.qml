import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.12

Column {

    property StackView stackView

    Button
    {
        onClicked:
        {
            stackView.push(mainView)
        }
    }
}
