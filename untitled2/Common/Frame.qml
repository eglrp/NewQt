import QtQuick 2.0

Item {
    id: frame
    property string source
    property int margins
    Image {
        source: frame.source
        anchors.fill: parent
    }
}
