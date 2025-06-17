import QtQuick 2.0
import LicenseManager 1.0

NavigatablePanel {
    width: 1280
    height: 800

    function addSectionsToFocusChain() {
        focusComponents.push(emailAddress)
        focusComponents.push(serialCode)
        focusComponents.push(activateButton)
        focusComponents.push(exitButton)
    }

    Component.onCompleted: {
        if(licenseManager.isLicenseFileValid()) {
            loader.source = "MainMenu.qml"
        } else {
            emailHint.visible = true;
            serialHint.visible = true;
            emailAddress.visible = true;
            serialCode.visible = true;
            exitButton.visible = true;
            activateButton.visible = true;

            addSectionsToFocusChain()
            focusComponents[0].focus = true
            focusComponents[0].forceActiveFocus()
        }
    }

    property int licenseValid : licenseManager.licenseValidity;

    onLicenseValidChanged: {
        if(licenseValid === 4) {
            window.loadView("MainMenu.qml")
        } else {
            showMessage("The entered activation code is not valid. " +
                        "Either your code expired or you entered an invalid code." +
                        "Please try to generate a new activation code from the websites member area and try again.")
        }
    }

    function licenseValidityCheck() {
        licenseManager.isActivationCodeValid(emailAddress.text, serialCode.text);
    }

    Rectangle {
        anchors.fill: parent

        Image {
            id: background
            source: "../graphics/settings_background.png"
            anchors.fill: parent
        }

        Column {

            anchors.centerIn: parent

            Text {
                id: emailHint
                text: qsTr("Email address:")
                font.pixelSize: standardButtonPixelSize;
                color: "#ffffff"
                visible: false
            }

            Rectangle {
                width: window.width / 2
                height: 50
                color: "#ffffff"
                y: emailHint.y + (standardButtonPixelSize * 1.5)
                x: (parent.width / 2) - (width/2)

                TextEdit {
                    id: emailAddress
                    anchors.fill: parent
                    color: "#000000"
                    text: qsTr("")
                    font.pixelSize: standardButtonPixelSize
                    wrapMode: TextEdit.NoWrap
                    visible: false
                }
            }

            Text {
                id: serialHint
                text: qsTr("Activation code:")
                font.pixelSize: standardButtonPixelSize;
                color: "#ffffff"
                visible: false
            }

            Rectangle {
                width: window.width / 2
                height: 50
                color: "#ffffff"
                y: serialHint.y + (standardButtonPixelSize * 1.5)
                x: (parent.width / 2) - (width/2)


                TextEdit {
                    id: serialCode
                    anchors.fill: parent
                    color: "#000000"
                    text: qsTr("")
                    font.pixelSize: standardButtonPixelSize
                    wrapMode: TextEdit.NoWrap
                    visible: false

                    Keys.onEnterPressed: {
                        licenseValidityCheck()
                    }
                }
            }

            Rectangle {
                color: "transparent"
                width: parent.width
                height: parent.height / 8
            }

            CustomButton {
                id: activateButton
                buttonText: "Activate"
                width: parent.width
                height: parent.height / 8
                onClicked: licenseValidityCheck()
                visible: false
            }

            Rectangle {
                color: "transparent"
                width: parent.width
                height: parent.height / 8
            }

            CustomButton {
                id: exitButton
                buttonText: "Exit"
                width: parent.width
                height: parent.height / 8
                onClicked:  Qt.quit()
                visible: false
            }
        }
    }

}
