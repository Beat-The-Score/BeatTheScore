import QtQuick 2.0
import QtGraphicalEffects 1.0

NavigatablePanel {
    property string message
    property variant backgroundSource

    color: "#80ffffff"
    anchors.fill: parent
    smooth: true
    background: false

    FastBlur {
        id: messageBoxBlur;
        visible: parent.visible
        anchors.fill: parent
        source: backgroundSource
        radius: height / 20
    }

    NumberAnimation {
        id: messageBoxAnimator
        target: messageContent
        property: "opacity";
        running: false
        to: 1.0;
        duration: 200

        onStopped: {
            messageMouseArea.focus = true
            messageMouseArea.forceActiveFocus()
        }
    }

    Item {
        id: messageContainer
        anchors.fill: parent
        visible: parent.visible

        Rectangle {
            id: messageContent
            anchors.centerIn: parent
            visible: parent.visible
            height: messageText.height
            color: "#666666"
            width: parent.width / 2
            opacity: 0.01

            Column {
                id: messageText
                visible: parent.visible
                width: parent.width

                Text {
                    width: parent.width

                    color: standardFontColor
                    text: message
                    font.pixelSize: standardFontPixelSize * 4
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.Wrap
                }

                Text {
                    width: parent.width

                    color: standardFontColor
                    text: qsTr(clickVerb + " to continue")
                    font.pixelSize: standardFontPixelSize * 3
                }
            }
        }
    }

    DropShadow {
        id: messageShadow;
        anchors.fill: source
        cached: true;
        radius: 32.0;
        samples: 16;
        color: "#80000000";
        smooth: true;
        source: messageContainer;
    }

    MouseArea {
        id: messageMouseArea
        anchors.fill: parent

        onClicked: {
            parent.visible = false
        }

        Keys.onPressed: {
            if(event.key === Qt.Key_Space || event.key === Qt.Key_Escape) {
                parent.visible = false
                messageMouseArea.focus = false
            }
        }

    }

    onVisibleChanged: {
        if(visible) {
            messageBoxAnimator.start();
        } else {
            messageContent.opacity = 0.01;
        }
    }
}
