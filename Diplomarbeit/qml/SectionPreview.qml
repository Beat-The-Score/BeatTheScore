import QtQuick 2.0

Rectangle {

    id: sectionPreview

    property alias selectedIndex: sectionListView.currentIndex
    property alias text: sectionListView.text
    signal sectionSelected

    property bool listVisible: sectionListView.count > 0

    SelectableListView {
        id: sectionListView
        model: mainGame.getAvaiableSections
        focus: parent.focus
        width: window.width
        height: sectionHeight
        itemDelegate: SelectableListViewDelegate {

            listView: sectionListView

            function getWidth() {
                var songLength = mainGame.getLengthOfSelectedSong()
                var sectionLength = model.modelData.getLength()
                var listViewWidth = sectionListView.width
                var sectionWidth = listViewWidth * (sectionLength / songLength)

                return sectionWidth
            }

            width: getWidth()
            itemText: model.modelData.getName()
        }
    }

    function loadSections() {
        sectionListView.model = mainGame.getAvailableSections()
    }

    function select(index) {
        sectionListView.select(index);
    }
}

