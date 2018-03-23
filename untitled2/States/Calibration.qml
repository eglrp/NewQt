import QtQuick 2.0
import "../Common"
import an.qt.interFace 1.0


Item {
    id: root
    width: 800
    height: 350
    anchors.top: titleBar.bottom
    anchors.topMargin: 30
    property int count: 0
    state: "calibrating"
    states: [
        State {
            name: "calibrating"
            PropertyChanges {
                target: shotButton
                text: count
            }
            PropertyChanges {
                target: promptInfo
                text: "请按下确认按钮，拍摄第 "+count+"/9 张图片。"
            }
            PropertyChanges {
                target: root
                count: 1
            }
            PropertyChanges {
                target: icon
                 source: "../Images/info.png";
            }
        },

		State {
            name: "calibStepFailed"
            PropertyChanges {
                target: shotButton
                text: count
            }
            PropertyChanges {
                target: promptInfo
                text: "拍摄失败，请重新拍摄第 "+count+"/9 张图片。"
            }
        },

        State {
            name: "calculating"
            PropertyChanges {
                target: shotButton
                text: ""
            }
            PropertyChanges {
                target: promptInfo
                text: "正在计算，请稍后..."
            }
            PropertyChanges {
                target: icon
                source: "../Images/wait.png";
            }
        },
        State {
            name: "successFinish"
            PropertyChanges {
                target: icon
                 source: "../Images/info.png";
            }
            PropertyChanges {
                target: promptInfo
                text: "标定完成，请按确认键返回主菜单。"
            }
        },
		State {
            name: "failedFinish"
            PropertyChanges {
                target: promptInfo
                text: "标定失败完成，请重新标定。"
            }
        }

    ]
    CamPreview{
        x:105
        id: camPreview
        spaceing: 60
    }

    Row {
        anchors.top: camPreview.bottom
        anchors.topMargin: 30
        x:60
        Image { id:icon; height: 35;width:  35 }
        Text {
            text: qsTr("系统标定：")
            font.pixelSize: 25
            color: "#4B9B85"
        }

        RotationAnimation{
            target: icon
            from: 0
            to: 720
            duration: 2000
            running: state == "calculating"
            onStopped: {
                state = "finish"
            }
        }
        Text {
            id: promptInfo
            width: 353
            font.pixelSize: 25
            color: "#515151"
            wrapMode: Text.WordWrap
        }
    }

    Keys.onPressed:{
        switch(event.key){
            case Qt.Key_Return:
                switch(state){
                case "calibrating":
                    if(count < 9){
						interFace.TriggerCalibStep(count);
                    }
                    else{
                        state = "calculating"
                    }
                    break
                case "successFinish":
				case "failedFinish":
                    ui.state = "start"
                    break
                default:
                    break
                }
                break
            case Qt.Key_Backspace:
                if(state == "finish")
                    state = "calibrating"
                break;
            default:
                break;
        }
    }

    onVisibleChanged: {
        if(visible)
            state = "calibrating"
    }

	Connections{
				target: interFace
				onCalibStepSuccess:{
					count++
					state = "calibrating"
				}

				onCalibStepFailed:{
					state = "calibStepFailed"
				}

				onCalibUnitSuccess:{
					state="successFinish"
				}

				onCalibUnitFailed:{
					state="failedFinish"
				}

			}

}
