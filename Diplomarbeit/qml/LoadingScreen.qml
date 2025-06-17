import QtQuick 2.0
import LicenseManager 1.0

Rectangle {
    id: loadingScreen
    width: 1200
    height: 800
    property variant pitchProgress: mainGame.loadingProgress;
    property bool pitchingDone: false;
    color: "transparent"

    onPitchProgressChanged: {
        if(pitchProgress >= 1.0 && !pitchingDone) {
            window.loadView("LicenseMenu.qml")
            pitchingDone = true;
        }
    }

    Keys.onPressed: {

    }

    Rectangle {
        id: progressBar
        x: 0
        y: parent.height * 4 / 5
        width: parent.width * (pitchProgress)
        height: parent.height / 20
        color: "#FFFFFF"
    }

    Text {
        anchors.bottom: progressBar.top
        anchors.left: parent.left
        anchors.margins: parent.height / 100

        font.pixelSize: parent.height / 15
        color: "#FFFFFF"
        text: qsTr("Loading instruments...")
    }

    Text {
        anchors.verticalCenter: progressBar.verticalCenter
        anchors.right: progressBar.right

        font.pixelSize: progressBar.height
        text: String(Math.round(pitchProgress * 100)) + "%"
    }
}
