import QtQuick
import QtQuick.Layouts
import "."

Item {
    id: root
    width: 1024
    height: 600
    implicitWidth: width
    implicitHeight: height
    Rectangle {
        anchors.fill: parent
        id: splash
        objectName: "Splash"
        width: 1024
        height: 600
        color: Theme.bg
        border.width: 0
        ColumnLayout {
            anchors.fill: parent
            spacing: 20
            Item { Layout.fillHeight: true }
            Text {
                Layout.alignment: Qt.AlignHCenter
                id: wordmark
                objectName: "Wordmark"
                text: "EVO"
                font.pixelSize: Theme.fontHero
                font.letterSpacing: 10
                font.weight: Font.DemiBold
                color: Theme.chrome
                horizontalAlignment: Text.AlignLeft
                Layout.preferredWidth: 109
                Layout.preferredHeight: 77
            }
            Rectangle {
                Layout.alignment: Qt.AlignHCenter
                id: rule
                objectName: "Rule"
                Layout.preferredWidth: 120
                Layout.preferredHeight: 2
                color: Theme.rallyRed
            }
            Text {
                Layout.alignment: Qt.AlignHCenter
                id: subtitle
                objectName: "Subtitle"
                text: "INFOTAINMENT"
                font.pixelSize: Theme.fontLg
                font.letterSpacing: 6
                color: Theme.textMuted
                horizontalAlignment: Text.AlignLeft
                Layout.preferredWidth: 103
                Layout.preferredHeight: 20
            }
            Item {
                Layout.alignment: Qt.AlignHCenter
                id: spacer
                objectName: "Spacer"
                Layout.preferredWidth: 10
                Layout.preferredHeight: 28
            }
            Rectangle {
                Layout.alignment: Qt.AlignHCenter
                id: loadingTrack
                objectName: "Loading Track"
                Layout.preferredWidth: 280
                Layout.preferredHeight: 12
                radius: Theme.radiusSm
                color: Theme.bgPanel
                border.color: Theme.border
                border.width: Theme.borderWidth
                Rectangle {
                    y: 0
                    x: 0
                    id: loadingProgress
                    objectName: "Loading Progress"
                    width: 100
                    height: 12
                    radius: Theme.radiusSm
                    color: Theme.accent
                }
            }
            Text {
                Layout.alignment: Qt.AlignHCenter
                id: loadingLabel
                objectName: "Loading Label"
                text: "LOADING"
                font.pixelSize: Theme.fontSm
                font.letterSpacing: 4
                color: Theme.accent
                horizontalAlignment: Text.AlignLeft
                Layout.preferredWidth: 46
                Layout.preferredHeight: 15
            }
            Item { Layout.fillHeight: true }
        }
    }
}
