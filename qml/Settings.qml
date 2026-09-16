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
        id: settings
        objectName: "Settings"
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
                id: settingsBody
                objectName: "Settings Body"
                Layout.fillWidth: true
                Layout.fillHeight: true
                ColumnLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 24
                    anchors.rightMargin: 24
                    anchors.topMargin: 24
                    anchors.bottomMargin: 24
                    spacing: 12

                    Rectangle {
                        id: rowBLUETOOTH
                        objectName: "Row BLUETOOTH"
                        Layout.fillWidth: true
                        Layout.preferredHeight: 72
                        radius: Theme.radiusMd
                        color: Theme.bgTile
                        border.color: Theme.border
                        border.width: Theme.borderWidth
                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 20
                            anchors.rightMargin: 20
                            spacing: 12
                            Icon {
                                Layout.alignment: Qt.AlignVCenter
                                name: "bluetooth"
                                style: "brands"
                                font.pixelSize: 20
                                color: Theme.accent
                                Layout.preferredWidth: 28
                            }
                            Text {
                                Layout.alignment: Qt.AlignVCenter
                                id: bLUETOOTH
                                objectName: "BLUETOOTH"
                                text: "BLUETOOTH"
                                font.pixelSize: Theme.fontLg
                                font.letterSpacing: 1
                                color: Theme.text
                            }
                            Item { Layout.fillWidth: true; Layout.fillHeight: true }
                            Icon {
                                Layout.alignment: Qt.AlignVCenter
                                name: "chevron-right"
                                font.pixelSize: 16
                                color: Theme.silverMuted
                            }
                        }
                    }

                    Rectangle {
                        id: rowECU
                        objectName: "Row ECU"
                        Layout.fillWidth: true
                        Layout.preferredHeight: 72
                        radius: Theme.radiusMd
                        color: Theme.bgTile
                        border.color: Theme.border
                        border.width: Theme.borderWidth
                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 20
                            anchors.rightMargin: 20
                            spacing: 12
                            Icon {
                                Layout.alignment: Qt.AlignVCenter
                                name: "microchip"
                                font.pixelSize: 20
                                color: Theme.accent
                                Layout.preferredWidth: 28
                            }
                            Text {
                                Layout.alignment: Qt.AlignVCenter
                                id: eCU
                                objectName: "ECU"
                                text: "ECU"
                                font.pixelSize: Theme.fontLg
                                font.letterSpacing: 1
                                color: Theme.text
                            }
                            Item { Layout.fillWidth: true; Layout.fillHeight: true }
                            Icon {
                                Layout.alignment: Qt.AlignVCenter
                                name: "chevron-right"
                                font.pixelSize: 16
                                color: Theme.silverMuted
                            }
                        }
                    }

                    Rectangle {
                        id: rowGPSMAP
                        objectName: "Row GPS / MAP"
                        Layout.fillWidth: true
                        Layout.preferredHeight: 72
                        radius: Theme.radiusMd
                        color: Theme.bgTile
                        border.color: Theme.border
                        border.width: Theme.borderWidth
                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 20
                            anchors.rightMargin: 20
                            spacing: 12
                            Icon {
                                Layout.alignment: Qt.AlignVCenter
                                name: "satellite-dish"
                                font.pixelSize: 20
                                color: Theme.accent
                                Layout.preferredWidth: 28
                            }
                            Text {
                                Layout.alignment: Qt.AlignVCenter
                                id: gPSMAP
                                objectName: "GPS / MAP"
                                text: "GPS / MAP"
                                font.pixelSize: Theme.fontLg
                                font.letterSpacing: 1
                                color: Theme.text
                            }
                            Item { Layout.fillWidth: true; Layout.fillHeight: true }
                            Icon {
                                Layout.alignment: Qt.AlignVCenter
                                name: "chevron-right"
                                font.pixelSize: 16
                                color: Theme.silverMuted
                            }
                        }
                    }

                    Rectangle {
                        id: rowDISPLAY
                        objectName: "Row DISPLAY"
                        Layout.fillWidth: true
                        Layout.preferredHeight: 72
                        radius: Theme.radiusMd
                        color: Theme.bgTile
                        border.color: Theme.border
                        border.width: Theme.borderWidth
                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 20
                            anchors.rightMargin: 20
                            spacing: 12
                            Icon {
                                Layout.alignment: Qt.AlignVCenter
                                name: "display"
                                font.pixelSize: 20
                                color: Theme.accent
                                Layout.preferredWidth: 28
                            }
                            Text {
                                Layout.alignment: Qt.AlignVCenter
                                id: dISPLAY
                                objectName: "DISPLAY"
                                text: "DISPLAY"
                                font.pixelSize: Theme.fontLg
                                font.letterSpacing: 1
                                color: Theme.text
                            }
                            Item { Layout.fillWidth: true; Layout.fillHeight: true }
                            Icon {
                                Layout.alignment: Qt.AlignVCenter
                                name: "chevron-right"
                                font.pixelSize: 16
                                color: Theme.silverMuted
                            }
                        }
                    }
                }
            }
        }
    }
}
