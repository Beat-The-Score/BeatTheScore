import QtQuick 2.0
import QtQuick.LocalStorage 2.0
import QtGraphicalEffects 1.0

NavigatablePanel {
    property variant backgroundSource

    color: "#80ffffff"
    anchors.fill: backgroundSource
    smooth: true
    background: false

    id: pauseMenu

    function addSectionsToFocusChain() {
        focusComponents = new Array()
        focusComponents.push(continueGameButton)
        focusComponents.push(backButton)
        focusComponents.push(volumeSlider)
    }

    Column {
        anchors.centerIn: parent
        height: parent.height / 3
        width: parent.width / 1.5
        anchors.leftMargin: 40

        Text {
            text: qsTr("Paused")
            font.pixelSize: parent.height / 4
            width: parent.width
        }

        Text {
            text: qsTr(clickVerb + " to continue")
            font.pixelSize: parent.height / 4
            width: parent.width
        }

        CustomButton {
            id: continueGameButton
            fontColor: "black"
            fontBorderColor: "white"
            buttonText: "continue"
            height: parent.height / 5
            width: parent.width / 5

            onClicked: {
                if (pauseMenu.visible) {
                    mainGame.togglePause()
                }
            }
        }

        CustomButton {
            id: backButton
            fontColor: "black"
            fontBorderColor: "white"
            buttonText: "back to menu"
            height: parent.height / 5
            width: parent.width / 5

            onClicked: {
                if (pauseMenu.visible) {
                    blurToDestroy.visible = false
                    blurToDestroy.destroy()
                    mainGame.songSelection()
                    window.loadView('GameSettings.qml')
                }
            }
        }

        CustomSlider {
            id: volumeSlider
            height: parent.height / 5
            width: parent.width / 2
            fontColor: "black"
            fontBorderColor: "white"
            visible: mainGame.isDesktop() && mainGame.pauseMenuVisible

            unit: "%"
            label: "volume: "
            min: 0
            max: 100
            value: volume
            onChanged: {
                mainGame.setVolume(value)

                var db = LocalStorage.openDatabaseSync(db_name);
                db.transaction(
                    function(tx) {
                        writePreferenceToDB(tx, 'Volume', value)
                    }
                )
            }
        }
    }

    onVisibleChanged: {
        if(visible) {
            addSectionsToFocusChain()
            focusComponents[0].focus = true
            focusComponents[0].forceActiveFocus()
        }
    }

    property variant disconnectHandler: mainGame.disconnectHandler;

    onDisconnectHandlerChanged: {
        if(disconnectHandler) {
            mainGame.songSelection()
            window.loadView('GameSettings.qml');
        }
    }
}
