pragma Singleton
import QtQuick

QtObject {
    // 2003 Evo VIII Cool Silver, night rally dash.
    // Carbon cockpit, brushed aluminum, Ralliart red, turbo-cyan telemetry.

    readonly property color bg: "#080A0D"
    readonly property color bgRaised: "#0E1218"
    readonly property color bgPanel: "#141A22"
    readonly property color bgTile: "#1A222C"
    readonly property color bgTileHover: "#222B36"

    readonly property color silver: "#C7D0D9"
    readonly property color silverMuted: "#8E99A6"
    readonly property color chrome: "#EEF2F6"

    readonly property color text: "#E8EDF2"
    readonly property color textMuted: "#8E99A6"
    readonly property color textDim: "#5A6470"

    readonly property color accent: "#6EE7D8"
    readonly property color accentDeep: "#163F3B"
    readonly property color rallyRed: "#E10600"
    readonly property color warning: "#F5A524"
    readonly property color ok: "#3DDC97"

    readonly property color border: "#2C3540"
    readonly property color borderBright: "#4E5A68"
    readonly property color hairline: "#1F262E"

    readonly property int borderWidth: 1
    readonly property int radiusSm: 4
    readonly property int radiusMd: 8
    readonly property int radiusLg: 14

    readonly property int spaceXs: 4
    readonly property int spaceSm: 8
    readonly property int spaceMd: 16
    readonly property int spaceLg: 24

    readonly property int fontXs: 11
    readonly property int fontSm: 12
    readonly property int fontMd: 14
    readonly property int fontLg: 16
    readonly property int fontXl: 18
    readonly property int fontTitle: 28
    readonly property int fontHero: 64

    readonly property int statusBarHeight: 48
}
