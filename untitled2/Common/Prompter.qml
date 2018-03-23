import QtQuick 2.0

Item {
    property string source
    width: 250
    height: 250
    x: (infoCol.x - width)/2
    anchors.verticalCenter: infoCol.verticalCenter
    Image {
        id: image
        anchors.fill: parent
        source: parent.source
    }
    NumberAnimation {
        duration: 2000
        running: image.visible
        onStopped:{
            switch (ui.state){
                case "registerSucceed":
                case "registerFaild":
                    ui.state = "register"
                    break
                case "recognizeFaild":
                case "recognizeSucceed":
                    ui.state = "recognize"
                    break
            }
        }
    }
}
