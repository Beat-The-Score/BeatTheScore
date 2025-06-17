import QtQuick 2.0

HighlightableItem {
    property color hightlightColor: "lightblue"
    property int itemHeight
    property real itemWidth
    property alias model: listView.model
    property alias currentIndex: listView.currentIndex
    property var currentItem: listView.currentItem
    property int selectedIndex: 0
    property var selectedItem: null
    property alias count: listView.count
    property alias itemDelegate: listView.delegate
    property alias text: title.text
    property var lastItem: listView.currentItem // to turn off highlight again
    property int preSelection

    signal headerDoubleClicked

    id: listViewComponent
    highlightOn: listView.focus

    Row {
        anchors.fill: container

        Rectangle {
            id: titleRectangle
            height: parent.height
            width: standardFontPixelSize * 3
            color: "transparent"

            Text {
                id: title
                text: qsTr("")
                color: standardFontColor
                font.pixelSize: standardFontPixelSize * 2
                rotation: -90
                anchors.centerIn: parent
            }

            MouseArea {
                anchors.fill: parent
                onDoubleClicked: {
                    headerDoubleClicked()
                }
            }
        }

        ListView {
            clip: true

            id: listView
            model: model
            height: parent.height
            width: parent.width - titleRectangle.width
            orientation: ListView.Horizontal
            highlightMoveVelocity: selectionSpeed
            currentIndex: 0

            onCurrentItemChanged: {
                handleItemChange()
            }

            Keys.onPressed: {
                if (event.key === Qt.Key_Space ||
                        event.key === Qt.Key_Enter ||
                        event.key === Qt.Key_Return) {
                    select(listView.currentIndex)
                }
            }

            delegate: SelectableListViewDelegate {
                width: itemWidth
                listView: listViewComponent
            }
        }
    }

    onFocusChanged: {
        listView.focus = true
    }

    function handleItemChange() {
        if (lastItem) {
            lastItem.highlightOff()
        }

        if (listView.currentItem) {
            listView.currentItem.highlightOn()
        }
    }

    function select(selectedIndex) {
        console.log(parent + "." + "select(" + selectedIndex + ")");
        currentIndex = selectedIndex
        if (selectedItem !== null) {
            selectedItem.selected = false
        }
        selectedItem = currentItem
        if (selectedItem !== null) {
            selectedItem.selected = true
            selectedIndex = currentIndex
        }
    }

    onModelChanged: {
        currentIndex = preSelection
        handleItemChange()
    }
}
