import QtQuick
import "."

Text {
    id: root
    property string name: "circle"
    property string style: "solid"

    text: Fa.glyph(name)
    color: Theme.accent
    font.family: Fa.family(style)
    font.styleName: Fa.styleName(style)
    font.weight: Fa.weight(style)
    font.pixelSize: Theme.fontTitle
    horizontalAlignment: Text.AlignHCenter
    verticalAlignment: Text.AlignVCenter
}
