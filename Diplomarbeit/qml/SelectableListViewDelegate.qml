import QtQuick 2.0

Item {
    property string itemText: (typeof modelData === typeof "" ? modelData : "")
    property string itemId: itemText

    property alias dataItemSource : dataItemLoader.sourceComponent
    property alias entryImage : entryImage
    property alias entrySpecificImage : entrySpecificImage
    property bool highlighted: false
    property bool selected: false
    property var listView


    // Access data of current item from outside delegate
    property variant dataOfItem : model

    function highlightOn() {
        //entryImage.visible = false
        //selectionImage.visible = true
        highlighted = true
    }

    function highlightOff() {
        //entryImage.visible = true
        //selectionImage.visible = false
        highlighted = false
    }

    id: selectableListViewDelegateItem

    height: selectableListViewDelegateItem.parent.height

    Item {
        id: standardDataItem
        anchors.fill: parent
        z: 1

        Text {
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: marginInItem

            z: 1
            text: itemText
            color: standardFontColor
            font.pixelSize: standardHeaderPixelSize
            font.bold: false
            //elide: Text.ElideRight
            wrapMode: Text.WordWrap
            width: parent.width  - 2 * marginInItem
        }

        visible: dataItemLoader.source !== undefined
    }

    Loader {
        id: dataItemLoader
        anchors.fill: parent
        z: 2
    }

    Image {
        id: entryImage
        anchors.fill: parent
        source: "../graphics/entry.png"
    }

    Image {
        id: selectionImage
        anchors.fill: parent
        source: "../graphics/selection.png"
        visible: selected
    }

    Image {
        id: highlightImage
        anchors.fill: parent
        source: "../graphics/item_highlight.png"
        visible: highlighted && selectableListViewDelegateItem.parent.parent.focus
    }

    Image {
        id: entrySpecificImage
        anchors.fill: parent
        opacity: 0.5
    }

    MouseArea {
        anchors.fill: parent
        z: 1
        onClicked: {
            listView.select(index)
            //listView.focus = true
        }
    }

}

