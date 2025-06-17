import QtQuick 2.0

BorderImage {
     id: button

     property string buttonText: ""
     property alias fontColor : text.color
     property color fontBorderColor : "black"
     property alias fontSize: text.font.pixelSize
     property alias bold: text.font.bold
     property int borderWidth: 2
     property string borderColor: "blue"
     property real hoverOpacity : .4
     property string imageSource
     property string mouseHighlightSource

     signal clicked

     Rectangle {
         id: shade
         anchors.fill: button;
         color: "black";
         opacity: mouseArea.containsMouse ? hoverOpacity : 0
         visible: false
     }

     Text {
         id: text
         z: 1
         anchors.centerIn: parent; anchors.verticalCenterOffset: -1
         font.pixelSize: parent.focus ? standardButtonPixelSize * 1.5 : standardButtonPixelSize
         text: buttonText
         color: standardFontColor
         style: Text.Raised;
         styleColor: fontBorderColor
         font.family: headerFont
         font.bold: mouseArea.containsMouse || parent.focus ? true : false
     }

     Image {
         id: buttonImage
         anchors.centerIn: parent
         source: mouseArea.containsMouse || parent.focus ? mouseHighlightSource : imageSource
         width: button.width / 3
         height: button.height / 3
         opacity: 0.8

     }


     MouseArea {
         id: mouseArea
         anchors.fill: parent
         hoverEnabled: true
         onClicked: {
             button.clicked();
         }
     }


     function selectMe() {
         button.clicked();
     }


     Image {
         id: buttonHighlight
         source: "../graphics/highlight.png"
         // visible: parent.focus
         visible: false
         anchors.fill: parent
     }

     // Keyboard Handling
     Keys.onPressed: {
         if ((event.key === Qt.Key_Return || event.key === Qt.Key_Space || event.key === Qt.Key_Enter) && button.focus) {
             event.accepted = true;
             button.clicked();
         }
     }
 }
