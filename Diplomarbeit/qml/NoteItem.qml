import QtQuick 2.0

Item {
    property int pitch: 0 // Midi
    property int time: 0
    property int duration: 0
    property int relativeDuration: 0;
    property bool sharp: false
    property int targetTime: 0;
    property int targetDuration: 0;
    property int targetPitch: 0;
    property int string: 0;
    property int fret: 0;
    property var pitchShift;
    property bool slide: false;
    property bool hammer: false;
}
