import QtQuick 2.9
import QtQuick.Window 2.2
import QtMultimedia 5.8
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import "Common"
import "States"
Window {
    id: root
    visible: true
    width: 800
    height: 600
    title: qsTr("三维人脸门禁终端")
    property string pwString : ""
    property string idNumberString : ""

    Image {
        id: ui
        source:"Images/background.png"
        anchors.fill: parent

        Rectangle {
            id: titleBar
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            height: 80
            color:"#4B9B85"
            anchors.leftMargin: 0
            anchors.topMargin: 0

            Image {
                id: logo
                source: "Images/logo.tiff"
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
                opacity: 0.9
                width: 190
                height: 60
            }

            Time {
                width: 100
                height: 50
                anchors.right: parent.right
                anchors.rightMargin: 30
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        state: "register"
        states: [
            State { name: "start"},
            State { name: "register"},
            State { name: "photoPreview"},
            State { name: "pointCloud"},
            State { name: "registerSucceed"},
            State { name: "registerFaild"},
            State { name: "recognize"},
            State { name: "password"},
            State { name: "recognizeFaild"},
            State { name: "recognizeSucceed"},
            State { name: "calibration"}
        ]

        Start {
            id: start
            anchors.centerIn: parent
            anchors.verticalCenterOffset: 20
            focus: ui.state == "start"
            visible: ui.state == "start"
        }


        Register{
            id: register
            anchors.top: titleBar.bottom
            anchors.topMargin: 30
            anchors.left: parent.left
            anchors.leftMargin: 30
            visible: ui.state == "register"
            focus: ui.state == "register"
            onVisibleChanged: {
                if (visible) {
                    idNumber.text = ""
                    name.text = ""
                }
            }
        }

        Register{
            id: recognize
            anchors.top: titleBar.bottom
            anchors.topMargin: 30
            anchors.left: parent.left
            anchors.leftMargin: 30
            text: "请整理容貌，点击拍照按钮进行识别。"
            visible: ui.state == "recognize"
            focus: ui.state == "recognize"
            onVisibleChanged: {
                if (visible) {
                    idNumber.text = ""
                    name.text = ""
                }
            }
        }

        PhotoPreview {
            id: photoPreview
            anchors.top: titleBar.bottom
            anchors.topMargin: 30
            anchors.left: parent.left
            anchors.leftMargin: 30
            visible: ui.state == "photoPreview"
        }

        PointCloud {
            anchors.top: titleBar.bottom
            anchors.topMargin: 30
            anchors.left: parent.left
            anchors.leftMargin: 40
            visible: ui.state == "pointCloud"
        }

        Prompter{
            source: "../Images/registerSucceed.png"
            visible: ui.state == "registerSucceed"
        }

        Prompter{
            source: "../Images/registerFaild.png"
            visible: ui.state == "registerFaild"
        }

        Prompter{
            source: "../Images/pass.tiff"
            visible: ui.state == "recognizeSucceed"
        }

        Prompter{
            source: "../Images/nopass.tiff"
            visible: ui.state == "recognizeFaild"
        }

        Column {
            id: infoCol

            anchors.top: titleBar.bottom
            anchors.topMargin: 30
            anchors.right: parent.right
            anchors.rightMargin: 35
            spacing: 6
            visible: ui.state != "start" && ui.state != "password"

            FramePicture {
                id: photoFrame
                frameSource: "../Images/photoFrame.png"
                height:195
                width: 156

            }

            Row {
                spacing: 6
                Image { source: "Images/name.png"; height: 22;width:  22 }
                Text { text: qsTr("姓名："); font.pixelSize: 18;color: "#4B9B85"}
            }

            Rectangle {
                id: nameFrame
                height: 35
                width: 160
                radius: 15
                color: "white"
                Text{
                    x:10
                    anchors.verticalCenter: parent.verticalCenter
                    id: name
                    font.pixelSize: 18
                    color: "#515151"
                }
            }

            Row {
                spacing: 6
                Image {source: "Images/idNumber.png"; height:22; width: 22}
                Text {text: qsTr("证件号码：");font.pixelSize: 18;color: "#4B9B85"}
            }

            Rectangle {
                height: 35
                width: 160
                radius: 15
                color: "white"
                Text{
                    height: 35
                    width: 140
                    anchors.verticalCenterOffset: 8
                    anchors.centerIn: parent
                    anchors.leftMargin: 10
                    anchors.rightMargin: 10
                    id: idNumber
                    font.pixelSize: 14
                    color: "#515151"
                }
            }
        }

        Button {
            id:shotButton
            source: "../Images/cam.tiff"
            anchors.horizontalCenter: infoCol.horizontalCenter
            anchors.top: infoCol.bottom
            anchors.topMargin: 30
            width: 73
            height: 73
            opacity: 0.9
            visible: ui.state != "start" && ui.state != "password"

        }
        Keys.onPressed: {
            var c = event.key - 48;
            switch(event.key){
            case Qt.Key_Return:
                if (ui.state === "register" || ui.state === "recognize"){
                    ui.state = "photoPreview"
                }
                break
            case Qt.Key_Backspace:
                idNumber.text = idNumber.text.substring(0, idNumber.text.length-1)
                pwString = pwString.substring(0, pwString.length-1)
                break
            default:
                pwString += (c)
                if(idNumber.text.length < 18)
                            idNumber.text += c
                if(pwString.length == 6){
                    if(pwString === "524869"){
                        ui.state = "start"

                        pwString = ""
                    }
                    else{
                        pwString = pwString.substring(1)
                    }
                }
                break;
            }
       }
    }
}


