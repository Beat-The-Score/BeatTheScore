import QtQuick 2.0

HighlightableItem {

    backgroundSource: "../graphics/search.png"
    higlightSource: "../graphics/search_highlight.png"

    property alias text : searchField.text
    signal filterTextChanged

    id: searchItem

    highlightOn: searchField.focus

    color: "transparent"

      Keys.onPressed: {
          if (event.key === Qt.Key_Return) {
              //shiftFocus(-1)
          }
      }


      TextInput {
          id: searchField
          anchors.bottom: parent.bottom
          color: standardFontColor
          height: parent.height / 2
          font.pixelSize: standardHeaderPixelSize
          font.bold: true
          z: 1

          onTextChanged: {
              var l=0
              while(l < text.length && " \n\t".indexOf(text[l]) > -1) {
                  l++
              }
              text = text.substring(l, text.length)
              scoreListView.applyFilter(text)
              filterTextChanged()
          }
      }

      MouseArea {
          anchors.fill: parent
          onClicked: {
            searchItem.forceActiveFocus()
          }
      }


    onFocusChanged: {
        if (focus) {
            //focusIndex = focusComponents.indexOf(searchItem)
            searchField.forceActiveFocus()
        }
    }
}
