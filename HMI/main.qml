import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.12

ApplicationWindow {
    width: 800
    height: 600
    visible: true
    title: qsTr("HMI")



    Material.theme: Material.Dark
    Material.accent: Material.DeepOrange
    Material.background: Material.BlueGrey
    Material.foreground:  Material.DeepPurple

    StackView
    {

        id:stack
        initialItem: startScreenView
    }


    Component
    {

        id:mainView
        Column
        {
            Label
            {
                text:"Communication"
            }
            Button
            {
                text: "<--"
                onClicked:
                {
                    stack.push(startScreenView)
                }
            }
        }
    }

    Component
    {
    id:startScreenView
        StartScreen
        {
            stackView:stack
        }
    }

}
