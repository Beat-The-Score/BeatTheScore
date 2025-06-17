import QtQuick 2.0

Item {
    //anchors.bottom: parent.bottom
    //anchors.right: parent.right

    width: window.width / 8
    height: window.height / 10
    x: parent.width - width
    y: parent.height - height - 20

    visible: !mainGame.resultScreenVisible

    Timer {
        id: scoreTimer
        interval: 500; running: true; repeat: true
        onTriggered: percentagScoreText.text = mainGame.getScorePercentage() + "%"
    }

    Text {
        id: percentagScoreText
        anchors.centerIn: parent
        font.pixelSize: parent.height * (3/4)
        style: Text.Outline
        text: mainGame.getScorePercentage() + "%"
    }

    onVisibleChanged: {
        if(visible) {
            scoreTimer.start();
        } else {
            scoreTimer.stop();
        }
    }
}
