import QtQuick 2.0

// Scroll bar
Rectangle {
    y: parent.height - 20
    height: 20
    width: parent.width
    color: "#000000"
    property variant visual;

    Rectangle {
        id: scrollHandle
        width: 40
        height: 20
        x: visual.scrollPosition
        color: "#C0C0C0"

        MouseArea {
            anchors.fill: parent
            property variant previousPosition

            onPressedChanged: {
                if(mainGame.isReplayMode()) {
                    if(pressed) {
                        visual.setScrollHandleGrabbed(true);
                        previousPosition = Qt.point(mouseX, mouseY);
                    } else {
                        visual.setScrollHandleGrabbed(false);
                    }
                }
            }

            onPositionChanged: {
                // Only allow scrolling in replay mode
                if (pressedButtons == Qt.LeftButton && mainGame.isReplayMode()) {
                    var dx = mouseX - previousPosition.x;
                    if(visual.scrollPosition + dx >= 0 &&
                            visual.scrollPosition + dx <= parent.parent.width - 40) {
                        visual.scrollPosition = visual.scrollPosition + dx;
                    }
                }
            }
        }
    }
}
