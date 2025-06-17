import QtQuick 2.0
import QtQuick.Controls 1.0

HighlightableItem {
    id: highlightableItem
    property alias text: label.text
    property alias checked: checkBox.checked
    property string checkedSource: "../graphics/checkbox_checked.png"
    property string uncheckedSource: "../graphics/checkbox.png"
    backgroundSource: ""
    highlightOn: focus

    Row {
        anchors.verticalCenter: parent.verticalCenter

        Image {
            property bool checked: false
            id: checkBox
            source: checked ? checkedSource : uncheckedSource
            height: highlightableItem.height / 2
            width: height
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    checkBox.checked = !checkBox.checked
                }
            }
        }

        Label {
            id: label
            text: "Hello"
            font.pixelSize: checkBox.height / 2
            color: standardFontColor
            z: 2
        }
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Return || event.key === Qt.Key_Space) {
            checked = !checked
        }
    }

}
