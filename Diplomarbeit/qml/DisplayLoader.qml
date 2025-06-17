import QtQuick 2.0
import QtGraphicalEffects 1.0

Rectangle {

    id: window
    width: 1200
    height: 800

    property int sectionHeight : window.height / 4
    property int standardItemWidth: window.height / 3
    property int itemBorderWidth: 1
    property int selectionSpeed: 2000
    property real standardFontPixelSize: window.height / 58
    property real standardHeaderPixelSize: standardFontPixelSize * 1.5
    property real standardButtonPixelSize: standardFontPixelSize * 3
    property real marginInItem: 20
    property string standardFontColor: "white"
    property string db_name: "BTS_UserData"
    property string menuPageBackgroundSource: "../graphics/settings_background.png"
    property string headerFont: "Freestyle Script"

    // User Preferences
    property bool keepPreferences;
    property bool showAudioInputs;
    property int audioLatency;
    property int volume : 100;

    property string clickVerb: mainGame.isDesktop() ? "click" : "tap"

    property var nextView

    property FastBlur blurToDestroy;

    function loadView(viewToLoad) {
        if (viewToLoad !== loader.source) {
            fadeIn.stop()
            fadeOut.stop();
            fadeOut.start();
            nextView = viewToLoad;
        }
    }

    Rectangle {
        id: blurRoot
        anchors.fill: parent

        Image {
            id: background
            source: menuPageBackgroundSource
            anchors.fill: parent
        }

        Loader {
            id: loader
            anchors.fill: parent
            source: "LoadingScreen.qml"

            asynchronous: true

            visible: status === Loader.Ready

            onLoaded: {
                fadeOut.stop()
                fadeIn.restart()
            }

            NumberAnimation on opacity {
                id: fadeOut
                to: 0.0
                duration: 100
                onStopped: {
                    if (nextView) {
                        loader.source = nextView;
                    }
                }
            }

            NumberAnimation on opacity {
                id: fadeIn
                to: 1.0
                duration: 100
            }
        }
    }

    function handleCommand(notes) {
        loader.item.chordPlayed(notes[0], notes[1], notes[2])
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_F11) {
            mainGame.changeFullScreen()
        }
    }

/*
    ShaderEffectSource {
        sourceItem: loader
        id: shaderSource
        mipmap: true
    }

    ShaderEffect {
        id: shader
        anchors.fill: parent
        property variant source: shaderSource
        property size delta: Qt.size(1.0 / width, 1.0 / height)
        visible: mainGame.isDesktop() ? true : false;
        fragmentShader: !mainGame.isDesktop() ? "" : "
uniform sampler2D source;
uniform highp vec2 delta;
varying highp vec2 qt_TexCoord0;

void main() {
    gl_FragColor = texture2D(source, qt_TexCoord0);
    gl_FragColor.a = 1.0;
}
"
    }*/

    PauseMenu { // can't be in the loader because it changes the loader.source
        id: pauseMenu
        visible: mainGame.pauseMenuVisible
        anchors.fill: parent
        backgroundSource: blurRoot
    }

    MessageBox {
        id: messageBox
        backgroundSource: blurRoot
        visible: false
    }

    function showMessage(message) {
        messageBox.message = message
        messageBox.visible = true
    }
}
