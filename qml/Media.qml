import QtQuick
import QtQuick.Layouts
import "."

Item {
    id: root
    width: 1024
    height: 600
    implicitWidth: width
    implicitHeight: height
    signal homeClicked()
    Rectangle {
        anchors.fill: parent
        id: media
        objectName: "Media"
        width: 1024
        height: 600
        color: Theme.bg
        border.width: 0
        ColumnLayout {
            anchors.fill: parent
            spacing: 0
            StatusBar {
                id: statusBar
                objectName: "StatusBar"
                Layout.fillWidth: true
                Layout.preferredHeight: Theme.statusBarHeight
                onHomeClicked: root.homeClicked()
            }
            Item {
                id: mediaBody
                objectName: "Media Body"
                Layout.fillWidth: true
                Layout.fillHeight: true
                ColumnLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 40
                    anchors.rightMargin: 40
                    anchors.topMargin: 40
                    anchors.bottomMargin: 40
                    spacing: 20
                    Item { Layout.fillHeight: true }
                    Text {
                        Layout.alignment: Qt.AlignHCenter
                        id: tRACKTITLE
                        objectName: "TRACK TITLE"
                        text: "TRACK TITLE"
                        font.pixelSize: Theme.fontTitle
                        font.letterSpacing: 3
                        color: Theme.chrome
                        horizontalAlignment: Text.AlignLeft
                        Layout.preferredWidth: 150
                        Layout.preferredHeight: 34
                    }
                    Text {
                        Layout.alignment: Qt.AlignHCenter
                        id: artist
                        objectName: "Artist"
                        text: "Artist"
                        font.pixelSize: Theme.fontLg
                        color: Theme.textMuted
                        horizontalAlignment: Text.AlignLeft
                        Layout.preferredWidth: 36
                        Layout.preferredHeight: 20
                    }
                    Rectangle {
                        Layout.alignment: Qt.AlignHCenter
                        id: progressTrack
                        objectName: "Progress Track"
                        Layout.preferredWidth: 640
                        Layout.preferredHeight: 8
                        radius: Theme.radiusSm
                        color: Theme.bgPanel
                        border.width: 0
                        Rectangle {
                            y: 0
                            x: 0
                            id: progress
                            objectName: "Progress"
                            width: 220
                            height: 8
                            radius: Theme.radiusSm
                            color: Theme.accent
                        }
                    }
                    Item {
                        Layout.alignment: Qt.AlignHCenter
                        id: controls
                        objectName: "Controls"
                        Layout.preferredWidth: 280
                        Layout.preferredHeight: 64
                        RowLayout {
                            anchors.fill: parent
                            spacing: 0
                            Rectangle {
                                Layout.alignment: Qt.AlignVCenter
                                id: prev
                                objectName: "Prev"
                                Layout.preferredWidth: 40
                                Layout.preferredHeight: 40
                                radius: width / 2
                                color: "transparent"
                                border.color: Theme.silverMuted
                                border.width: Theme.borderWidth
                                Icon {
                                    anchors.centerIn: parent
                                    name: "backward-step"
                                    color: Theme.chrome
                                    font.pixelSize: 16
                                }
                            }
                            Item { Layout.fillWidth: true; Layout.fillHeight: true }
                            Rectangle {
                                Layout.alignment: Qt.AlignVCenter
                                id: play
                                objectName: "Play"
                                Layout.preferredWidth: 64
                                Layout.preferredHeight: 64
                                radius: width / 2
                                color: Theme.rallyRed
                                border.width: 0
                                Icon {
                                    anchors.centerIn: parent
                                    name: "play"
                                    color: Theme.chrome
                                    font.pixelSize: 24
                                }
                            }
                            Rectangle {
                                Layout.alignment: Qt.AlignVCenter
                                id: next
                                objectName: "Next"
                                Layout.preferredWidth: 40
                                Layout.preferredHeight: 40
                                radius: width / 2
                                color: "transparent"
                                border.color: Theme.silverMuted
                                border.width: Theme.borderWidth
                                Icon {
                                    anchors.centerIn: parent
                                    name: "forward-step"
                                    color: Theme.chrome
                                    font.pixelSize: 16
                                }
                            }
                        }
                    }
                    Item { Layout.fillHeight: true }
                }
            }
        }
    }
}
