import QtQuick 2.0

Item {

    id: root
    property string frameSource
    property string pictureSource
    property int margins
    Image {
        id: frame
        source: root.frameSource
        width: root.width
        height: root.height
        Image {
            source: root.pictureSource
            anchors.fill: parent
            anchors.margins: root.margins
            fillMode: Image.PreserveAspectCrop
        }
    }
}
