import QtQuick 2.0
import Visuals 1.0

NavigatablePanel {
    id: resultScreen
    anchors.fill: parent;
    smooth: true;
    visible: mainGame.resultScreenVisible
    focus: mainGame.resultScreenVisible

    background: true

    function addSectionsToFocusChain() {
        focusComponents = new Array()
        focusComponents.push(watchReplayButton)
        focusComponents.push(tryAgainButton)
        focusComponents.push(backToMenuButton)
    }


    Column {
        height: (parent.height / 3) * 2
        y: parent.height / 3
        x: parent.width / 8
        width: parent.width

        Text {
            id: scoreText;
            text: qsTr("Score: ") + mainGame.getScorePercentage()
            font.pixelSize: parent.height / 7
            width: resultScreen.width
            height: parent.height / 3
            color: "#FFFFFF"
        }


        CustomButton {
            id: watchReplayButton

            buttonText: "watch replay"
            height: parent.height / 5
            width: parent.width / 5
            focus: mainGame.resultScreenVisible

            onClicked: {
                mainGame.startReplay();
                helpText.text = clickVerb + " to replay!";
                helpText.visible = true;
            }
        }

        CustomButton {
            id: tryAgainButton
            buttonText: "try again"
            height: parent.height / 5
            width: parent.width / 5

            onClicked: {
                mainGame.startAgain();
                helpText.text = clickVerb + " to play!";
                helpText.visible = true;
            }
        }

        CustomButton {
            id: backToMenuButton
            buttonText: "back to menu"
            height: parent.height / 5
            width: parent.width / 5

            onClicked: {
                mainGame.songSelection()
                window.loadView("GameSettings.qml")
            }
        }
    }

    onVisibleChanged: {
        if (visible) {
            scoreText.text = qsTr("Score: ") + mainGame.getScorePercentage()
            initializeFocus()
        }
    }
}
