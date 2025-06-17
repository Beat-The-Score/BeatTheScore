import QtQuick 2.0
import QtQuick.LocalStorage 2.0

Rectangle {
    id: navPan
    property int focusIndex: 0
    property alias background: backgroundImage.visible

    property var focusComponents: new Array();

    signal confirm
    signal back

    signal chordPlayed(int pitch1, int pitch2, int pitch3)

    function moveInActiveList(amount) {
        var leftBoundsOk = (focusComponents[focusIndex].currentIndex + amount) >= 0
        var rightBoundsOk = (focusComponents[focusIndex].currentIndex + amount) < focusComponents.length - 1

        if (leftBoundsOk && rightBoundsOk)
        {
            focusComponents[focusIndex].currentIndex += amount
        }
    }

    function initializeFocus() {

        addSectionsToFocusChain()
        focusComponents[0].focus = true
        focusComponents[0].forceActiveFocus()
    }

    function writeDefaultPreferences(tx) {
        writeNewRowToDB(tx, 'Display Option', 0)
        writeNewRowToDB(tx, 'Device', 0)
        writeNewRowToDB(tx, 'Training Mode', 0)
    }

    function writeDefaultGlobalOptions(tx) {
        writeNewRowToDB(tx, 'KeepPreferences', 1)
    }

    function createTableIfNotExistant() {
        var db = LocalStorage.openDatabaseSync(db_name);
        var tableName = 'Preferences'

        db.transaction(
            function(tx) {
                 // count existing tables named ...
                var rs = tx.executeSql('SELECT count(*) FROM sqlite_master WHERE type=\'table\' AND name=\'' + tableName + '\'')
                if (rs.rows.item(0).value === 0 || rs.rows.item(0).value === undefined) {
                    tx.executeSql('CREATE TABLE IF NOT EXISTS Preferences(key TEXT, value TEXT)')
                }

                rs = tx.executeSql('SELECT * FROM Preferences');
                if (rs.rows.length === 0) {
                    // Write default preferencees
                    writeDefaultPreferences(tx)
                    writeDefaultGlobalOptions(tx)
                }
            }
        )
    }

    function writeNewRowToDB(tx, key, value) {
        tx.executeSql('INSERT INTO Preferences VALUES(?, ?)', [key, value])
    }

    function writePreferenceToDB(tx, key, value) {
        tx.executeSql('DELETE FROM Preferences WHERE key = ?', [key]) // will do nothing if key doesn't exist
        tx.executeSql('INSERT INTO Preferences VALUES(?, ?)', [key, value])
    }

    function askDB(tx, key, defaultValue) {
        /*
        // Show all added greetings
        var rs = tx.executeSql('SELECT * FROM Preferences');
        console.log("Preference rows: " + rs.rows.length)
        var r = ""
        for(var i = 0; i < rs.rows.length; i++) {
            r += rs.rows.item(i).key + ", " + rs.rows.item(i).value + "\n"
        }

        console.log(r)
        */

        var rs = tx.executeSql('SELECT value FROM Preferences WHERE key = ?', [key])
        if (rs.rows.length > 0) {
            return rs.rows.item(0).value
        } else {
            return defaultValue
        }
    }


    function shiftFocus(amount) {
        focusIndex = (focusIndex + amount + focusComponents.length) % focusComponents.length
        focusComponents[focusIndex].focus = true
    }

    // Keyboard Handling
    Keys.onPressed: {
        if (event.key === Qt.Key_Tab || event.key === Qt.Key_Down || event.key === Qt.Key_Right) {
            shiftFocus(1)
        }
        else if (event.key === Qt.Key_Backtab || event.key === Qt.Key_Up || event.key === Qt.Key_Left) {
            shiftFocus(-1)
        }
        else if (event.key === Qt.Key_Backspace || event.key === Qt.Key_Escape || event.key == Qt.Key_Back) {
            back()
            event.accepted = true
        }
    }


    // Instrument Handling
    Chord {
        id: upChord
        notes: new Array("D", "F", "A")
        onPlayed: shiftFocus(-1)
    }

    Chord {
        id: downChord
        notes: new Array("D", "F#", "A")
        onPlayed: {
            shiftFocus(1)
        }
    }

    Chord {
        id: rightChord
        notes: new Array("E", "G#", "B")
        onPlayed: {
            moveInActiveList(1)
        }
    }

    Chord {
        id: leftChord
        notes: new Array("E", "G", "B")
        onPlayed: {
            moveInActiveList(-1)
        }
    }

    Chord {
        id: confirmChord
        notes: new Array("C", "E", "G")
        onPlayed: {
            confirm()
        }
    }

    Chord {
        id: backChord
        notes: new Array("C", "D#", "G")
        onPlayed: {
            back()
        }
    }

    Chord {
        id: selectChord
        notes: new Array("F", "A", "C")
        onPlayed: {
            console.log("dings jetzt")
            focusComponents[focusIndex].select(focusComponents[focusIndex].currentIndex)
        }
    }

    color: "transparent"

    Image {
        id: backgroundImage
        anchors.fill: parent
        source: menuPageBackgroundSource
        visible: false
    }
}
