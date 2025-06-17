import QtQuick 2.0

NavigatablePanel {
    id: container

    anchors.fill: parent

    property int titleSize: height / 20;
    property int textSize: height / 30;
    property int spacing: height / 100;

    Component.onCompleted: {
        focusComponents.push(backButton)
        focusComponents[0].focus = true
        focusComponents[0].forceActiveFocus()
    }

    onBack: {
        window.loadView("MainMenu.qml")
    }

    Flickable {
        anchors.fill: parent
        anchors.leftMargin: spacing
        anchors.rightMargin: spacing

        flickableDirection: Flickable.VerticalFlick
        clip: true

        contentHeight: help.height;

        Column {
            id: help
            width: parent.width

            spacing: container.spacing;

            Text {
                width: parent.width
                font.pixelSize: titleSize
                wrapMode: Text.WordWrap
                color: "#fff"
                text: "Help"
            }

            Text {
                width: parent.width
                font.pixelSize: textSize
                wrapMode: Text.WordWrap
                color: "#fff"
                text: "Beat the Score is a music game where you try to play a song as accurately as possible."
            }

            Text {
                width: parent.width
                font.pixelSize: titleSize
                wrapMode: Text.WordWrap
                color: "#fff"
                text: "Start a new game"
            }

            Text {
                width: parent.width
                font.pixelSize: textSize
                wrapMode: Text.WordWrap
                color: "#fff"
                text: "After clicking the start button in the menu you can choose the song, track and instrument you want to play."
            }

            Row {
                width: parent.width
                spacing: container.spacing;

                Image {
                    width: parent.width / 2
                    height: width * sourceSize.height / sourceSize.width
                    source: "../graphics/help/settings.png"
                }
                Text {
                    width: parent.width / 2
                    font.pixelSize: textSize
                    wrapMode: Text.WordWrap
                    color: "#fff"
                    text:
                        "In the top row you can select the song you wanna play. You can put your songs in the folder Scores in your home directory and they will show up here.\n" +
                        "Most songs contain multiple tracks. Here you can choose which one you want to play.\n" +
                        "If you wanna start from a certain section of the song, you can select it in the list below.\n" +
                        "Don't forget to choose your instrument from the available inputs! Available MIDI instruments and phone connectors will show up here.\n" +
                        "Under the instrument selection you can choose how the notes should be displayed. You can choose between a pianoroll which will display the notes as rectangles, conventional music notation and a tablature for guitarists.\n" +
                        "For trainings purposes you can change the tempo of the song and activate the Trainings mode where the game will pause at every note until you get it right."
                }
            }

            Text {
                width: parent.width
                font.pixelSize: titleSize
                wrapMode: Text.WordWrap
                color: "#fff"
                text: "The game screen"
            }

            Row {
                width: parent.width
                spacing: container.spacing;

                Image {
                    width: parent.width / 2
                    height: width * sourceSize.height / sourceSize.width
                    source: "../graphics/help/score.png"
                }
                Text {
                    width: parent.width / 2
                    font.pixelSize: textSize
                    wrapMode: Text.WordWrap
                    color: "#fff"
                    text:
                        "What all display methods have in common is the progress bar at the bottom, the percentage value on the right and the red line in the center.\n" +
                        "The notes gonna move from the right of the screen to the left. Try to hit them when they touch the red line.\n" +
                        "Depending on how good your timing is you can get up to 100 points for each note. If you play the wrong key you don't get any points."
                }
            }

            Text {
                width: parent.width
                font.pixelSize: titleSize
                wrapMode: Text.WordWrap
                color: "#fff"
                text: "The piano-roll"
            }

            Row {
                width: parent.width
                spacing: container.spacing;

                Image {
                    width: parent.width / 2
                    height: width * sourceSize.height / sourceSize.width
                    source: "../graphics/help/piano.png"
                }
                Text {
                    width: parent.width / 2
                    font.pixelSize: textSize
                    wrapMode: Text.WordWrap
                    color: "#fff"
                    text:
                        "In this view the notes are displayed as rectangles. The keyboard on the left shows which key should be pressed.\n" +
                        "By playing notes on the keyboard you can fill the rectangles that are already on the screen.\n" +
                        "Arrows show whether you pressed/released too early or to late. An arrow to the left means too early, an arrow to the right means too late."
                }
            }

            Text {
                width: parent.width
                font.pixelSize: titleSize
                wrapMode: Text.WordWrap
                color: "#fff"
                text: "The tablature"
            }

            Row {
                width: parent.width
                spacing: container.spacing;

                Image {
                    width: parent.width / 2
                    height: width * sourceSize.height / sourceSize.width
                    source: "../graphics/help/tablature.png"
                }
                Text {
                    width: parent.width / 2
                    font.pixelSize: textSize
                    wrapMode: Text.WordWrap
                    color: "#fff"
                    text:
                        "The view for guitarists contains a line for each string of the guitar and numbers indicate which fret should be played.\n" +
                        "Arrows show whether you played too early or to late. An arrow to the left means too early, an arrow to the right means too late."
                }
            }
        }

    }

    CustomButton {
        id: backButton
        anchors.bottom: parent.bottom
        anchors.right: parent.right

        width: parent.height / 5
        height: parent.height / 8

        buttonText: qsTr("back")
        onClicked: {
            window.loadView("MainMenu.qml")
        }
    }
}
