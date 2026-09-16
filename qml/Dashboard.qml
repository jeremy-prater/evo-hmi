import QtQuick
import QtQuick.Layouts
import "."

Item {
    id: root
    width: 1024
    height: 600
    implicitWidth: width
    implicitHeight: height
    signal activated(string name)
    signal homeClicked()
    Rectangle {
        anchors.fill: parent
        id: dashboard
        objectName: "Dashboard"
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
                id: dashboardBody
                objectName: "Dashboard Body"
                Layout.fillWidth: true
                Layout.fillHeight: true
                ColumnLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 24
                    anchors.rightMargin: 24
                    anchors.topMargin: 24
                    anchors.bottomMargin: 24
                    spacing: 0
                    Item {
                        id: tiles
                        objectName: "Tiles"
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        GridLayout {
                            anchors.fill: parent
                            columns: 2
                            rowSpacing: 16
                            columnSpacing: 16
                            Rectangle {
                                id: tileENGINE
                                objectName: "Tile ENGINE"
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.row: 0
                                Layout.column: 0
                                radius: Theme.radiusMd
                                color: Theme.bgTile
                                border.color: Theme.border
                                border.width: Theme.borderWidth
                                ColumnLayout {
                                    anchors.fill: parent
                                    spacing: 16
                                    Item { Layout.fillHeight: true }
                                    Icon {
                                        Layout.alignment: Qt.AlignHCenter
                                        id: icon
                                        objectName: "Icon"
                                        name: "gauge-high"
                                        Layout.preferredWidth: 56
                                        Layout.preferredHeight: 56
                                        font.pixelSize: 40
                                        color: Theme.accent
                                    }
                                    Text {
                                        Layout.alignment: Qt.AlignHCenter
                                        id: label
                                        objectName: "Label"
                                        text: "ENGINE"
                                        font.pixelSize: Theme.fontXl
                                        font.letterSpacing: 2
                                        color: Theme.text
                                        horizontalAlignment: Text.AlignLeft
                                        Layout.preferredWidth: 59
                                        Layout.preferredHeight: 22
                                    }
                                    Item { Layout.fillHeight: true }
                                }
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: root.activated("ENGINE")
                                }
                            }
                            Rectangle {
                                id: tileMAP
                                objectName: "Tile MAP"
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.row: 0
                                Layout.column: 1
                                radius: Theme.radiusMd
                                color: Theme.bgTile
                                border.color: Theme.border
                                border.width: Theme.borderWidth
                                ColumnLayout {
                                    anchors.fill: parent
                                    spacing: 16
                                    Item { Layout.fillHeight: true }
                                    Icon {
                                        Layout.alignment: Qt.AlignHCenter
                                        id: icon2
                                        objectName: "Icon"
                                        name: "location-dot"
                                        Layout.preferredWidth: 56
                                        Layout.preferredHeight: 56
                                        font.pixelSize: 40
                                        color: Theme.accent
                                    }
                                    Text {
                                        Layout.alignment: Qt.AlignHCenter
                                        id: label2
                                        objectName: "Label"
                                        text: "MAP"
                                        font.pixelSize: Theme.fontXl
                                        font.letterSpacing: 2
                                        color: Theme.text
                                        horizontalAlignment: Text.AlignLeft
                                        Layout.preferredWidth: 34
                                        Layout.preferredHeight: 22
                                    }
                                    Item { Layout.fillHeight: true }
                                }
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: root.activated("MAP")
                                }
                            }
                            Rectangle {
                                id: tileMEDIA
                                objectName: "Tile MEDIA"
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.row: 1
                                Layout.column: 0
                                radius: Theme.radiusMd
                                color: Theme.bgTile
                                border.color: Theme.border
                                border.width: Theme.borderWidth
                                ColumnLayout {
                                    anchors.fill: parent
                                    spacing: 16
                                    Item { Layout.fillHeight: true }
                                    Icon {
                                        Layout.alignment: Qt.AlignHCenter
                                        id: icon3
                                        objectName: "Icon"
                                        name: "music"
                                        Layout.preferredWidth: 56
                                        Layout.preferredHeight: 56
                                        font.pixelSize: 40
                                        color: Theme.accent
                                    }
                                    Text {
                                        Layout.alignment: Qt.AlignHCenter
                                        id: label3
                                        objectName: "Label"
                                        text: "MEDIA"
                                        font.pixelSize: Theme.fontXl
                                        font.letterSpacing: 2
                                        color: Theme.text
                                        horizontalAlignment: Text.AlignLeft
                                        Layout.preferredWidth: 49
                                        Layout.preferredHeight: 22
                                    }
                                    Item { Layout.fillHeight: true }
                                }
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: root.activated("MEDIA")
                                }
                            }
                            Rectangle {
                                id: tileSETTINGS
                                objectName: "Tile SETTINGS"
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.row: 1
                                Layout.column: 1
                                radius: Theme.radiusMd
                                color: Theme.bgTile
                                border.color: Theme.border
                                border.width: Theme.borderWidth
                                ColumnLayout {
                                    anchors.fill: parent
                                    spacing: 16
                                    Item { Layout.fillHeight: true }
                                    Icon {
                                        Layout.alignment: Qt.AlignHCenter
                                        id: icon4
                                        objectName: "Icon"
                                        name: "gear"
                                        Layout.preferredWidth: 56
                                        Layout.preferredHeight: 56
                                        font.pixelSize: 40
                                        color: Theme.accent
                                    }
                                    Text {
                                        Layout.alignment: Qt.AlignHCenter
                                        id: label4
                                        objectName: "Label"
                                        text: "SETTINGS"
                                        font.pixelSize: Theme.fontXl
                                        font.letterSpacing: 2
                                        color: Theme.text
                                        horizontalAlignment: Text.AlignLeft
                                        Layout.preferredWidth: 76
                                        Layout.preferredHeight: 22
                                    }
                                    Item { Layout.fillHeight: true }
                                }
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: root.activated("SETTINGS")
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
