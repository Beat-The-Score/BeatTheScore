import QtQuick 2.0

Rectangle {

    property alias highlightOn: listHighlight.visible
    property alias backgroundSource: listViewBackground.source
    property alias higlightSource: listHighlight.source
    property alias container: container

    color: "transparent"

    Rectangle {
        id: container

        anchors.fill: parent
        anchors.topMargin: standardFontPixelSize * .5
        anchors.bottomMargin: standardFontPixelSize * .5

        color: "transparent"

        Image {
            id: listViewBackground
            anchors.fill: parent
            source: "../graphics/background.png"
        }

        Image {
            id: listHighlight
            source: "../graphics/highlight.png"
            z: 1
            anchors.fill: parent
        }
    }
}
