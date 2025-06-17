import QtQuick 2.0

Image {
    property string category;
    property string specific;

    function updateSource() {
        source = "../graphics/logo.png"
    }

    source: "../graphics/logo.png"

    onCategoryChanged: updateSource()
    onSpecificChanged: updateSource()
}
