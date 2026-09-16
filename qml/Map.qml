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
        id: map
        objectName: "Map"
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
                id: mapBody
                objectName: "Map Body"
                Layout.fillWidth: true
                Layout.fillHeight: true
                ColumnLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 16
                    anchors.rightMargin: 16
                    anchors.topMargin: 16
                    anchors.bottomMargin: 16
                    spacing: 12
                    Rectangle {
                        id: search
                        objectName: "Search"
                        Layout.fillWidth: true
                        Layout.preferredHeight: 44
                        radius: Theme.radiusMd
                        color: Theme.bgPanel
                        border.color: Theme.border
                        border.width: Theme.borderWidth
                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 16
                            anchors.rightMargin: 16
                            spacing: 12
                            Icon {
                                Layout.alignment: Qt.AlignVCenter
                                name: "magnifying-glass"
                                font.pixelSize: 16
                                color: Theme.textMuted
                                Layout.preferredWidth: 20
                            }
                            Text {
                                Layout.alignment: Qt.AlignVCenter
                                id: searchDestination
                                objectName: "Search destination"
                                text: "Search destination"
                                font.pixelSize: Theme.fontMd
                                color: Theme.textMuted
                            }
                        }
                    }
                    Rectangle {
                        id: mapArea
                        objectName: "Map Area"
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        radius: Theme.radiusMd
                        color: Theme.bgPanel
                        border.color: Theme.border
                        border.width: Theme.borderWidth
                        ColumnLayout {
                            anchors.fill: parent
                            spacing: 8
                            Item { Layout.fillHeight: true }
                            Icon {
                                Layout.alignment: Qt.AlignHCenter
                                id: position
                                objectName: "Position"
                                name: "location-dot"
                                font.pixelSize: 28
                                color: Theme.rallyRed
                            }
                            Text {
                                Layout.alignment: Qt.AlignHCenter
                                id: mAP
                                objectName: "MAP"
                                text: "MAP"
                                font.pixelSize: Theme.fontMd
                                color: Theme.textMuted
                                horizontalAlignment: Text.AlignLeft
                                Layout.preferredWidth: 26
                                Layout.preferredHeight: 17
                            }
                            Item { Layout.fillHeight: true }
                        }
                    }
                }
            }
        }
    }
}
