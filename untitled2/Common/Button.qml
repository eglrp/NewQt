import QtQuick 2.0

Item {
    id: button

    signal clicked

    property string text
    property color color: "white"
    property string source

    width : 100
    height: 50

    BorderImage {
        id: buttonImage
        source: button.source
        width: button.width; height: button.height
    }
    MouseArea {
        id: mouseRegion
        anchors.fill: buttonImage
        onClicked: { button.clicked() }
    }
    Text {
        id: btnText
        anchors.fill: buttonImage
        anchors.margins: 5
        text: button.text
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
        color: button.color
        font.bold: true
        style: Text.Raised
        styleColor: "black"
        font.pixelSize: 23
        visible: ui.state == "calibration"
    }
}
