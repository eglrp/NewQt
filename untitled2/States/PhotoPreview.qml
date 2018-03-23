import QtQuick 2.0
import "../Common"
Item {
    property string pictureSource1
    property string pictureSource2
    signal  pCloudFinished
    property Image icon: icon
    Row {
        id: pictureRow
        spacing:20

        FramePicture{
            width: 260
            height: 350
            id: leftPicture
            frameSource: "../Images/camFrame.png"
            pictureSource: pictureSource1
            margins: 15
        }

        FramePicture{
            id: rightPicture
            width: 260
            height: 350
            frameSource: "../Images/camFrame.png"
            pictureSource: pictureSource2
            margins: 15
        }
    }

    Row {
        anchors.top: pictureRow.bottom
        anchors.topMargin: 30
        x: 47
        spacing: 10
        Image {
            id: icon
            source: "../Images/wait.png"
            height: 35
            width:  35
        }
        RotationAnimation{
            target: icon
            from: 0
            to: 720
            duration: 2000
            running: ui.state == "photoPreview"
            onStopped: {
                ui.state = "pointCloud"
            }
        }
        Text {
            text: qsTr("正在构建三维人脸，请稍后...")
            font.pixelSize: 25
            color: "#515151"
        }
    }
}
