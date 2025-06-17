import QtQuick 2.0

Rectangle {

    property var notes
    signal played
    id: chordRectangle

    function chordPlayed(pitch1, pitch2, pitch3) {
        if (notes.indexOf(getNoteName(pitch1)) >= 0 && notes.indexOf(getNoteName(pitch2)) >= 0 && notes.indexOf(getNoteName(pitch3)) >= 0)
        {
            // if child of CustomButton or selectableListViewDelegate
            // chordRectangle.parent.selectMe();
            played();
        }
    }

    function getNoteName(pitch) {
        var notes = new Array("C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B");
        var note_index = pitch % 12;
        return notes[note_index];
    }


    Component.onCompleted: {
        navPan.chordPlayed.connect(chordPlayed)
        //chordRectangle.parent.chordPlayed.connect(chordPlayed)
    }

    function clickHandler() {
        console.log('main clicked')
    }

}
