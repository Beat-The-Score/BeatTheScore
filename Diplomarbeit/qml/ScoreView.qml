import QtQuick 2.2
import QtGraphicalEffects 1.0
import ScoreView 1.0

Rectangle {
    width: 1280
    height: 720
    focus: true

    property bool pauseVisible: mainGame.pauseMenuVisible;

    onPauseVisibleChanged: {
        if(!pauseVisible) {
            mouseArea.forceActiveFocus();
        }
    }

    Rectangle {
        id: content
        visible: true
        anchors.fill: parent
        color: "white"

        ScoreView {
            id: scoreView;
            game: mainGame;
            width: parent.width;
            height: parent.height - 20;
            visible: !mainGame.resultScreenVisible

            // Toggle Pause Menu
            MouseArea {
                id: mouseArea
                width: parent.width
                height: parent.height
                anchors.topMargin: window.height / 10
                focus: !mainGame.resultScreenVisible;
                onClicked: {
                    if(!mainGame.resultScreenVisible) {
                        mainGame.togglePause();
                        helpText.visible = false;
                    }
                }

                Keys.onPressed: {
                    console.log("key")
                    if (event.key === Qt.Key_Space || event.key === Qt.Key_Escape || event.key === Qt.Key_Back) {
                        mainGame.togglePause()
                        helpText.visible = false
                        event.accepted = true
                    }
                }
            }
        }

        Component.onCompleted: {
            blurToDestroy = pauseBlur
        }

        FastBlur {
            id: pauseBlur
            anchors.fill: scoreView
            source: scoreView
            radius: height / 20
            visible: mainGame.pauseMenuVisible
        }

        ResultScreen {
            id: resultScreen
            visible: mainGame.resultScreenVisible
            anchors.fill: parent
            focus: mainGame.resultScreenVisible;
        }

        CustomScrollBar {
            y: scoreView.height
            visual: scoreView;
        }

        HelpText {
            id: helpText
        }

        PercentageText {

        }
    }

    /*
    FancyShader {
        source: content
    }*/
}
