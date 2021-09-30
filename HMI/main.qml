import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.15


ApplicationWindow {
    id:root
    width: 800
    height: 600
    visible: true
    title: qsTr("HMI")
    opacity: 1
    flags: Qt.Window | Qt.FramelessWindowHint
    visibility: visibilityFlag === false ? "Windowed" : "FullScreen"


    Material.theme: Material.Dark
    Material.accent: Material.DeepOrange
    Material.background: Material.BlueGrey
    Material.foreground:  Material.DeepPurple

    property string time
    property string date
    property bool visibilityFlag: false
    property bool pendingState: false

    BusyIndicator {
      id: busyIndicator
      width: parent.width/8
      height: width
      running: pendingState
      visible: running
      anchors.centerIn: parent
      z: parent.z + 1
    }

    Component.onCompleted:
    {
        pendingState = true
    }

    StackView
    {
        id:stack
        initialItem: mainView
    }


    Timer {
        interval: 500; running: true; repeat: true
        onTriggered:
        {
            time =  Qt.formatTime(new Date(), "hh:mm:ss")
            date = Qt.formatDate(new Date(), Qt.ISODate)
            pendingState = false
        }
    }

    header: ToolBar{
         id:menutoolbar
         MouseArea
         {
             property var clickPos
             clickPos: "1,1"
             anchors.fill: parent

             onPressed: {
                 clickPos  = Qt.point(mouse.x,mouse.y)
             }

             onPositionChanged: {
                 var delta = Qt.point(mouse.x-clickPos.x, mouse.y-clickPos.y)
                 root.x += delta.x;
                 root.y += delta.y;
             }
             onDoubleClicked:
             {
                 visibilityFlag = !visibilityFlag
             }
         }
         contentHeight: toolButton.implicitHeight

         ToolButton {
             id: toolButton

             Text {
                 id: name
                 text: stack.depth > 1 ? "\u2630" : "\u2630"
                 anchors.horizontalCenter: parent.horizontalCenter
                 anchors.verticalCenter: parent.verticalCenter
                 font.bold: true
                 font.pointSize: 16
                 color: "#2c81e0"
             }
             font.pixelSize: Qt.application.font.pixelSize * 1.6
             onClicked: {
                 if (stack.depth > 0) {
                     drawer.open()
                 } else {
                     stack.pop()
                 }
             }
         }

         RowLayout
         {
             anchors.fill: parent
             spacing: 20
             anchors.leftMargin: 140
             anchors.rightMargin: 20
             Button
             {
                 id:logoutbtn
                 anchors.right: parent.right
                 anchors.verticalCenter: parent.verticalCenter
                 width: 30
                 height: 30
                 anchors.leftMargin: 180
                 Layout.minimumWidth: 30
                 Layout.minimumHeight: 40

                 background: Rectangle
                 {
                     color:"transparent"
                 }
                 Image
                 {
                     width: 30
                     height: 30
                     id: logoutimage
                     anchors.verticalCenter: parent.verticalCenter
                     source: "qrc:/Images/logout.svg"
                     sourceSize.width: 30
                     sourceSize.height: 30
                     fillMode: Image.PreserveAspectFit
                 }


                 onClicked:
                 {
                     Qt.quit()
                 }
             }


             Row
             {
                 id:daterow
                 anchors.verticalCenter: parent.verticalCenter
                 Image {
                     width: 30
                     height: 30
                     id: dateimage
                     source: "qrc:/Images/calendar.svg"
                     sourceSize.width: width
                     sourceSize.height: height
                     fillMode: Image.PreserveAspectFit
                     anchors.verticalCenter: parent.verticalCenter
                 }
                 Label
                 {
                     anchors.verticalCenter: parent.verticalCenter
                     color: "black"
                     text:date
                     font.pointSize: 14
                 }
             }
             Row
             {
                 id:timerow
                 anchors.verticalCenter: parent.verticalCenter
                 Image {
                     id:timeimage
                     width: 30
                     height: 30
                     source: "qrc:/Images/time.svg"
                     sourceSize.width: width
                     sourceSize.height: height
                     fillMode: Image.PreserveAspectFit

                 }
                 Label
                 {
                     id:timelabel
                     anchors.verticalCenter: parent.verticalCenter
                     color: "black"
                     text:time
                     font.pointSize: 14
                 }
             }
        }
    }


    Drawer {
            id: drawer
            width: root.width * 0.3
            height: root.height

            Column {
                anchors.fill: parent
                anchors.topMargin: 10

                ColumnLayout {
                    anchors.horizontalCenter: parent.horizontalCenter
                    Rectangle
                    {
                        width: 60
                        height: 60
                        color: "transparent"
                        Image {
                            id: name2
                            source: "qrc:/Images/settings.svg"
                            sourceSize.width: 60
                            sourceSize.height: 60
                            fillMode: Image.PreserveAspectFit

                        }
                    }

                }

                ItemDelegate {

                    id:delegateCamSettings
                    text: qsTr("Kontrol")
                    width: parent.width
                    onClicked: {
                        stack.push(controlScreenView)
                        drawer.close()
                    }
                }

                ItemDelegate {
                    text: qsTr("Haberleşme")
                    width: parent.width
                    onClicked: {
                        stack.push(communicationScreenView)
                        drawer.close()
                    }
                }

                ItemDelegate {
                    anchors.bottom: drawer.bottom
                    text: qsTr("Anasayfa")

                    width: parent.width
                    onClicked: {
                        drawer.close()
                        stack.push(mainView)
                    }
                }
            }
        }


    Component
    {
        id:mainView

        MainView
        {
            anchors.horizontalCenter: parent.horizontalCenter
            stackView: stack
        }
    }

    Component
    {
        id:controlScreenView
        ControlScreen
        {
            stackView: stack
        }
    }

    Component
    {
        id:communicationScreenView
        CommunicationScreen
        {
            stackView: stack
        }
    }

}
