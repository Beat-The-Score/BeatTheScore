import QtQuick 2.0

BorderImage {
     id: button

     property string headerText: "Header"
     property string headerColor: ""


     Rectangle {
         id: shade
         anchors.fill: button;
         color: headerColor
     }


     Text {
         id: text
         anchors.centerIn: parent; anchors.verticalCenterOffset: -1
         font.pixelSize: parent.width > parent.height ? parent.height * .5 : parent.width * .5
         font.letterSpacing: 7
         font.bold: true
         text: headerText
     }

 }
