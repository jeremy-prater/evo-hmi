import QtQuick
import QtQuick.Layouts
import "."

Item {
    id: root
    implicitWidth: 1024
    implicitHeight: Theme.statusBarHeight
    width: parent ? parent.width : implicitWidth
    height: implicitHeight
    signal homeClicked()
    Rectangle {
        anchors.fill: parent
        id: hMIStatusBar
        objectName: "HMI / StatusBar"
        width: 1024
        height: Theme.statusBarHeight
        color: Theme.bgRaised
        border.color: Theme.hairline
        border.width: Theme.borderWidth
        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 24
            anchors.rightMargin: 24
            spacing: 0
            Item {
                Layout.alignment: Qt.AlignVCenter
                id: indicators
                objectName: "Indicators"
                Layout.preferredWidth: 120
                Layout.preferredHeight: 24
                RowLayout {
                    anchors.fill: parent
                    spacing: 12
                    Icon {
                        Layout.alignment: Qt.AlignVCenter
                        name: "house"
                        font.pixelSize: 18
                        color: Theme.chrome
                    }
                    Rectangle {
                        Layout.alignment: Qt.AlignVCenter
                        id: indicator1
                        objectName: "Indicator 1"
                        Layout.preferredWidth: 12
                        Layout.preferredHeight: 12
                        radius: Theme.radiusSm
                        color: Theme.rallyRed
                        border.width: 0
                    }
                    Rectangle {
                        Layout.alignment: Qt.AlignVCenter
                        id: indicator2
                        objectName: "Indicator 2"
                        Layout.preferredWidth: 12
                        Layout.preferredHeight: 12
                        radius: Theme.radiusSm
                        color: Theme.accent
                        border.width: 0
                    }
                    Rectangle {
                        Layout.alignment: Qt.AlignVCenter
                        id: indicator3
                        objectName: "Indicator 3"
                        Layout.preferredWidth: 12
                        Layout.preferredHeight: 12
                        radius: Theme.radiusSm
                        color: Theme.silverMuted
                        border.width: 0
                    }
                }
            }
            Item { Layout.fillWidth: true; Layout.fillHeight: true }
            Text {
                Layout.alignment: Qt.AlignVCenter
                id: time
                objectName: "Time"
                text: "12:34"
                font.pixelSize: Theme.fontXl
                font.weight: Font.DemiBold
                color: Theme.chrome
                horizontalAlignment: Text.AlignLeft
                Layout.preferredWidth: 45
                Layout.preferredHeight: 24
            }
        }
    }
    MouseArea {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: 96
        onClicked: root.homeClicked()
    }
}
