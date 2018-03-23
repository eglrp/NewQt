import QtQuick 2.5

Item {
    height: 90
    width: 284
    property alias source : img.source
    property alias text : name.text
    Image {
        id: img
        height: 90
        width: 284
        Text {
            id:name
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -10
            font.family: "Helvetica"
            font.pixelSize: 35
            opacity: 0.8
            color: "#4B9B85"
        }
    }
}
