pragma Singleton
import QtQuick

QtObject {
    readonly property string solidFamily: solidLoader.name
    readonly property string regularFamily: regularLoader.name
    readonly property string brandsFamily: brandsLoader.name
    readonly property bool ready: solidLoader.status === FontLoader.Ready

    property FontLoader solidLoader: FontLoader {
        source: Qt.resolvedUrl("fonts/fa-solid-900.otf")
    }
    property FontLoader regularLoader: FontLoader {
        source: Qt.resolvedUrl("fonts/fa-regular-400.otf")
    }
    property FontLoader brandsLoader: FontLoader {
        source: Qt.resolvedUrl("fonts/fa-brands-400.otf")
    }

    function family(style) {
        if (style === "brands")
            return "Font Awesome 7 Brands"
        return "Font Awesome 7 Free"
    }

    function styleName(style) {
        if (style === "solid")
            return "Solid"
        return "Regular"
    }

    function weight(style) {
        if (style === "solid")
            return Font.Black
        return Font.Normal
    }

    function glyph(name) {
        switch (name) {
        case "backward-step": return "\uf048"
        case "bluetooth": return "\uf293"
        case "bolt": return "\uf0e7"
        case "car": return "\uf1b9"
        case "chevron-right": return "\uf054"
        case "clock": return "\uf017"
        case "display": return "\ue163"
        case "fan": return "\uf863"
        case "forward-step": return "\uf051"
        case "gas-pump": return "\uf52f"
        case "gauge": return "\uf624"
        case "gauge-high": return "\uf625"
        case "gear": return "\uf013"
        case "house": return "\uf015"
        case "location-dot": return "\uf3c5"
        case "magnifying-glass": return "\uf002"
        case "map": return "\uf279"
        case "microchip": return "\uf2db"
        case "music": return "\uf001"
        case "oil-can": return "\uf613"
        case "pause": return "\uf04c"
        case "play": return "\uf04b"
        case "satellite-dish": return "\uf7c0"
        case "sliders": return "\uf1de"
        case "temperature-half": return "\uf2c9"
        case "volume-high": return "\uf028"
        case "wave-square": return "\uf83e"
        default: return "\uf111"
        }
    }
}
