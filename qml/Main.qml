import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "."

ApplicationWindow {
    id: win
    width: 1024
    height: 600
    minimumWidth: 1024
    minimumHeight: 600
    maximumWidth: 1024
    maximumHeight: 600
    visible: true
    title: qsTr("EVO")
    color: Theme.bg

    readonly property int screenSplash: 0
    readonly property int screenDashboard: 1
    readonly property int screenEngine: 2
    readonly property int screenMap: 3
    readonly property int screenMedia: 4
    readonly property int screenSettings: 5

    function goHome() { stack.currentIndex = screenDashboard }

    Component.onCompleted: {
        if (platformName === "eglfs")
            showFullScreen()
    }

    StackLayout {
        id: stack
        anchors.fill: parent
        currentIndex: win.screenSplash

        Splash {
            MouseArea {
                anchors.fill: parent
                onClicked: stack.currentIndex = win.screenDashboard
            }
            Timer {
                interval: 1500
                running: stack.currentIndex === win.screenSplash
                onTriggered: stack.currentIndex = win.screenDashboard
            }
        }

        Dashboard {
            onActivated: (name) => {
                switch (name) {
                case "ENGINE": stack.currentIndex = win.screenEngine; break
                case "MAP": stack.currentIndex = win.screenMap; break
                case "MEDIA": stack.currentIndex = win.screenMedia; break
                case "SETTINGS": stack.currentIndex = win.screenSettings; break
                }
            }
            onHomeClicked: win.goHome()
        }

        Engine { onHomeClicked: win.goHome() }
        Map { onHomeClicked: win.goHome() }
        Media { onHomeClicked: win.goHome() }
        Settings { onHomeClicked: win.goHome() }
    }
}
