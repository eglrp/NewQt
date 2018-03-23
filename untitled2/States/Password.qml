import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Item {
    id: root
    width: 400
    height: 400
    state: "init"
    states: [
        State {
            name: "input"
            PropertyChanges {
                target: input
                text: ""
            }
        },
        State {
            name: "worng"
            PropertyChanges {
                target: info
                text: "密码错误!"
                color: "#ED4241"
            }
        }
    ]
    Image {
        anchors.centerIn: parent
        id: bg
        height: 90
        width: 284
        source: "../Images/unchoiced.tiff"
        TextField {
            id: input
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -12
            width: 200
            echoMode: TextInput.Password
            font.pixelSize: 40
            textColor: "#515151"
            style: TextFieldStyle {
                background: Rectangle {
                    height: 70
                    width: 180
                    border.width: 0
                }
            }

            Keys.onReturnPressed: {
                if (text == "    "){
                    ui.state = "start"
                }
                else{
                    root.state = "worng"
                }
            }
            onVisibleChanged: {
                if (visible) {
                    input.forceActiveFocus()
                    input.text = ""
                    root.state = "input"
                }
            }
        }
    }


    Text {
        anchors.horizontalCenter: bg.horizontalCenter
        id: info
        anchors.top: bg.bottom
        anchors.topMargin: 10
        text: qsTr("请输入管理员密码")
        font.family: "Helvetica"
        font.pixelSize: 25
        color: "#515151"
    }
}
