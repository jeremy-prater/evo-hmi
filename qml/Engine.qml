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
        id: engine
        objectName: "Engine"
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
                id: engineBody
                objectName: "Engine Body"
                Layout.fillWidth: true
                Layout.fillHeight: true
                ColumnLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 24
                    anchors.rightMargin: 24
                    anchors.topMargin: 24
                    anchors.bottomMargin: 24
                    spacing: 16
                    Item {
                        id: gauges
                        objectName: "Gauges"
                        Layout.fillWidth: true
                        Layout.preferredHeight: 200
                        RowLayout {
                            anchors.fill: parent
                            spacing: 0
                            Item {
                                Layout.alignment: Qt.AlignVCenter
                                id: gaugeRPM
                                objectName: "Gauge RPM"
                                Layout.preferredWidth: 180
                                Layout.preferredHeight: 180
                                ColumnLayout {
                                    anchors.fill: parent
                                    spacing: 8
                                    Item { Layout.fillHeight: true }
                                    Rectangle {
                                        Layout.alignment: Qt.AlignHCenter
                                        id: ring
                                        objectName: "Ring"
                                        Layout.preferredWidth: 120
                                        Layout.preferredHeight: 120
                                        radius: width / 2
                                        color: Theme.accentDeep
                                        border.color: Theme.silver
                                        border.width: 2
                                        Icon {
                                            anchors.centerIn: parent
                                            name: "gauge-high"
                                            color: Theme.silver
                                            font.pixelSize: 36
                                        }
                                    }
                                    Text {
                                        Layout.alignment: Qt.AlignHCenter
                                        id: rPM
                                        objectName: "RPM"
                                        text: "RPM"
                                        font.pixelSize: Theme.fontMd
                                        font.letterSpacing: 2
                                        color: Theme.textMuted
                                        horizontalAlignment: Text.AlignLeft
                                        Layout.preferredWidth: 27
                                        Layout.preferredHeight: 17
                                    }
                                    Item { Layout.fillHeight: true }
                                }
                            }
                            Item {
                                Layout.alignment: Qt.AlignVCenter
                                id: gaugeBOOST
                                objectName: "Gauge BOOST"
                                Layout.preferredWidth: 180
                                Layout.preferredHeight: 180
                                ColumnLayout {
                                    anchors.fill: parent
                                    spacing: 8
                                    Item { Layout.fillHeight: true }
                                    Rectangle {
                                        Layout.alignment: Qt.AlignHCenter
                                        id: ring2
                                        objectName: "Ring"
                                        Layout.preferredWidth: 120
                                        Layout.preferredHeight: 120
                                        radius: width / 2
                                        color: Theme.accentDeep
                                        border.color: Theme.accent
                                        border.width: 2
                                        Icon {
                                            anchors.centerIn: parent
                                            name: "gauge"
                                            color: Theme.accent
                                            font.pixelSize: 36
                                        }
                                    }
                                    Text {
                                        Layout.alignment: Qt.AlignHCenter
                                        id: bOOST
                                        objectName: "BOOST"
                                        text: "BOOST"
                                        font.pixelSize: Theme.fontMd
                                        font.letterSpacing: 2
                                        color: Theme.textMuted
                                        horizontalAlignment: Text.AlignLeft
                                        Layout.preferredWidth: 42
                                        Layout.preferredHeight: 17
                                    }
                                    Item { Layout.fillHeight: true }
                                }
                            }
                            Item {
                                Layout.alignment: Qt.AlignVCenter
                                id: gaugeAFR
                                objectName: "Gauge AFR"
                                Layout.preferredWidth: 180
                                Layout.preferredHeight: 180
                                ColumnLayout {
                                    anchors.fill: parent
                                    spacing: 8
                                    Item { Layout.fillHeight: true }
                                    Rectangle {
                                        Layout.alignment: Qt.AlignHCenter
                                        id: ring3
                                        objectName: "Ring"
                                        Layout.preferredWidth: 120
                                        Layout.preferredHeight: 120
                                        radius: width / 2
                                        color: Theme.accentDeep
                                        border.color: Theme.silver
                                        border.width: 2
                                        Icon {
                                            anchors.centerIn: parent
                                            name: "wave-square"
                                            color: Theme.silver
                                            font.pixelSize: 36
                                        }
                                    }
                                    Text {
                                        Layout.alignment: Qt.AlignHCenter
                                        id: aFR
                                        objectName: "AFR"
                                        text: "AFR"
                                        font.pixelSize: Theme.fontMd
                                        font.letterSpacing: 2
                                        color: Theme.textMuted
                                        horizontalAlignment: Text.AlignLeft
                                        Layout.preferredWidth: 23
                                        Layout.preferredHeight: 17
                                    }
                                    Item { Layout.fillHeight: true }
                                }
                            }
                        }
                    }
                    Item {
                        id: values
                        objectName: "Values"
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        GridLayout {
                            anchors.fill: parent
                            columns: 4
                            rowSpacing: 12
                            columnSpacing: 12
                            Rectangle {
                                id: valueTIMING
                                objectName: "Value TIMING"
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
                                    anchors.leftMargin: 12
                                    anchors.rightMargin: 12
                                    anchors.topMargin: 8
                                    anchors.bottomMargin: 8
                                    spacing: 4
                                    Item { Layout.fillHeight: true }
                                    Text {
                                        id: tIMING
                                        objectName: "TIMING"
                                        text: "TIMING"
                                        font.pixelSize: Theme.fontXs
                                        font.letterSpacing: 1
                                        color: Theme.textMuted
                                        horizontalAlignment: Text.AlignLeft
                                        Layout.preferredWidth: 34
                                        Layout.preferredHeight: 14
                                    }
                                    Rectangle {
                                        id: valueLine
                                        objectName: "Value Line"
                                        Layout.preferredWidth: 72
                                        Layout.preferredHeight: 10
                                        radius: Theme.radiusSm
                                        color: Theme.accent
                                    }
                                    Item { Layout.fillHeight: true }
                                }
                            }
                            Rectangle {
                                id: valueCOOLANT
                                objectName: "Value COOLANT"
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
                                    anchors.leftMargin: 12
                                    anchors.rightMargin: 12
                                    anchors.topMargin: 8
                                    anchors.bottomMargin: 8
                                    spacing: 4
                                    Item { Layout.fillHeight: true }
                                    Text {
                                        id: cOOLANT
                                        objectName: "COOLANT"
                                        text: "COOLANT"
                                        font.pixelSize: Theme.fontXs
                                        font.letterSpacing: 1
                                        color: Theme.textMuted
                                        horizontalAlignment: Text.AlignLeft
                                        Layout.preferredWidth: 45
                                        Layout.preferredHeight: 14
                                    }
                                    Rectangle {
                                        id: valueLine2
                                        objectName: "Value Line"
                                        Layout.preferredWidth: 72
                                        Layout.preferredHeight: 10
                                        radius: Theme.radiusSm
                                        color: Theme.accent
                                    }
                                    Item { Layout.fillHeight: true }
                                }
                            }
                            Rectangle {
                                id: valueKNOCK
                                objectName: "Value KNOCK"
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.row: 0
                                Layout.column: 2
                                radius: Theme.radiusMd
                                color: Theme.bgTile
                                border.color: Theme.border
                                border.width: Theme.borderWidth
                                ColumnLayout {
                                    anchors.fill: parent
                                    anchors.leftMargin: 12
                                    anchors.rightMargin: 12
                                    anchors.topMargin: 8
                                    anchors.bottomMargin: 8
                                    spacing: 4
                                    Item { Layout.fillHeight: true }
                                    Text {
                                        id: kNOCK
                                        objectName: "KNOCK"
                                        text: "KNOCK"
                                        font.pixelSize: Theme.fontXs
                                        font.letterSpacing: 1
                                        color: Theme.textMuted
                                        horizontalAlignment: Text.AlignLeft
                                        Layout.preferredWidth: 34
                                        Layout.preferredHeight: 14
                                    }
                                    Rectangle {
                                        id: valueLine3
                                        objectName: "Value Line"
                                        Layout.preferredWidth: 72
                                        Layout.preferredHeight: 10
                                        radius: Theme.radiusSm
                                        color: Theme.warning
                                    }
                                    Item { Layout.fillHeight: true }
                                }
                            }
                            Rectangle {
                                id: valueWGDC
                                objectName: "Value WGDC"
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.row: 0
                                Layout.column: 3
                                radius: Theme.radiusMd
                                color: Theme.bgTile
                                border.color: Theme.border
                                border.width: Theme.borderWidth
                                ColumnLayout {
                                    anchors.fill: parent
                                    anchors.leftMargin: 12
                                    anchors.rightMargin: 12
                                    anchors.topMargin: 8
                                    anchors.bottomMargin: 8
                                    spacing: 4
                                    Item { Layout.fillHeight: true }
                                    Text {
                                        id: wGDC
                                        objectName: "WGDC"
                                        text: "WGDC"
                                        font.pixelSize: Theme.fontXs
                                        font.letterSpacing: 1
                                        color: Theme.textMuted
                                        horizontalAlignment: Text.AlignLeft
                                        Layout.preferredWidth: 29
                                        Layout.preferredHeight: 14
                                    }
                                    Rectangle {
                                        id: valueLine4
                                        objectName: "Value Line"
                                        Layout.preferredWidth: 72
                                        Layout.preferredHeight: 10
                                        radius: Theme.radiusSm
                                        color: Theme.accent
                                    }
                                    Item { Layout.fillHeight: true }
                                }
                            }
                            Rectangle {
                                id: valueSPEED
                                objectName: "Value SPEED"
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
                                    anchors.leftMargin: 12
                                    anchors.rightMargin: 12
                                    anchors.topMargin: 8
                                    anchors.bottomMargin: 8
                                    spacing: 4
                                    Item { Layout.fillHeight: true }
                                    Text {
                                        id: sPEED
                                        objectName: "SPEED"
                                        text: "SPEED"
                                        font.pixelSize: Theme.fontXs
                                        font.letterSpacing: 1
                                        color: Theme.textMuted
                                        horizontalAlignment: Text.AlignLeft
                                        Layout.preferredWidth: 31
                                        Layout.preferredHeight: 14
                                    }
                                    Rectangle {
                                        id: valueLine5
                                        objectName: "Value Line"
                                        Layout.preferredWidth: 72
                                        Layout.preferredHeight: 10
                                        radius: Theme.radiusSm
                                        color: Theme.accent
                                    }
                                    Item { Layout.fillHeight: true }
                                }
                            }
                            Rectangle {
                                id: valueTPS
                                objectName: "Value TPS"
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
                                    anchors.leftMargin: 12
                                    anchors.rightMargin: 12
                                    anchors.topMargin: 8
                                    anchors.bottomMargin: 8
                                    spacing: 4
                                    Item { Layout.fillHeight: true }
                                    Text {
                                        id: tPS
                                        objectName: "TPS"
                                        text: "TPS"
                                        font.pixelSize: Theme.fontXs
                                        font.letterSpacing: 1
                                        color: Theme.textMuted
                                        horizontalAlignment: Text.AlignLeft
                                        Layout.preferredWidth: 18
                                        Layout.preferredHeight: 14
                                    }
                                    Rectangle {
                                        id: valueLine6
                                        objectName: "Value Line"
                                        Layout.preferredWidth: 72
                                        Layout.preferredHeight: 10
                                        radius: Theme.radiusSm
                                        color: Theme.accent
                                    }
                                    Item { Layout.fillHeight: true }
                                }
                            }
                            Rectangle {
                                id: valueBATT
                                objectName: "Value BATT"
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.row: 1
                                Layout.column: 2
                                radius: Theme.radiusMd
                                color: Theme.bgTile
                                border.color: Theme.border
                                border.width: Theme.borderWidth
                                ColumnLayout {
                                    anchors.fill: parent
                                    anchors.leftMargin: 12
                                    anchors.rightMargin: 12
                                    anchors.topMargin: 8
                                    anchors.bottomMargin: 8
                                    spacing: 4
                                    Item { Layout.fillHeight: true }
                                    Text {
                                        id: bATT
                                        objectName: "BATT"
                                        text: "BATT"
                                        font.pixelSize: Theme.fontXs
                                        font.letterSpacing: 1
                                        color: Theme.textMuted
                                        horizontalAlignment: Text.AlignLeft
                                        Layout.preferredWidth: 24
                                        Layout.preferredHeight: 14
                                    }
                                    Rectangle {
                                        id: valueLine7
                                        objectName: "Value Line"
                                        Layout.preferredWidth: 72
                                        Layout.preferredHeight: 10
                                        radius: Theme.radiusSm
                                        color: Theme.accent
                                    }
                                    Item { Layout.fillHeight: true }
                                }
                            }
                            Rectangle {
                                id: valueLOAD
                                objectName: "Value LOAD"
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.row: 1
                                Layout.column: 3
                                radius: Theme.radiusMd
                                color: Theme.bgTile
                                border.color: Theme.border
                                border.width: Theme.borderWidth
                                ColumnLayout {
                                    anchors.fill: parent
                                    anchors.leftMargin: 12
                                    anchors.rightMargin: 12
                                    anchors.topMargin: 8
                                    anchors.bottomMargin: 8
                                    spacing: 4
                                    Item { Layout.fillHeight: true }
                                    Text {
                                        id: lOAD
                                        objectName: "LOAD"
                                        text: "LOAD"
                                        font.pixelSize: Theme.fontXs
                                        font.letterSpacing: 1
                                        color: Theme.textMuted
                                        horizontalAlignment: Text.AlignLeft
                                        Layout.preferredWidth: 25
                                        Layout.preferredHeight: 14
                                    }
                                    Rectangle {
                                        id: valueLine8
                                        objectName: "Value Line"
                                        Layout.preferredWidth: 72
                                        Layout.preferredHeight: 10
                                        radius: Theme.radiusSm
                                        color: Theme.accent
                                    }
                                    Item { Layout.fillHeight: true }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
