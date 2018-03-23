import QtQuick 2.5
import QtGraphicalEffects 1.0

Item {

    width: 400
    height: 400
    Grid {
        columns: 2
        anchors.centerIn: parent
        anchors.verticalCenterOffset: 20
        columnSpacing: 30
        rowSpacing: 50

        ChoiceUi{
            source: "../Images/unchoiced.tiff"
            text: qsTr("1.信息注册")
        }

        ChoiceUi{
            source: "../Images/unchoiced.tiff"
            text: qsTr("2.自动识别")
        }

        ChoiceUi{
            source: "../Images/unchoiced.tiff"
            text: qsTr("3.刷卡识别")
        }

        ChoiceUi{
            source: "../Images/unchoiced.tiff"
            text: qsTr("4.相机标定")
        }
    }

    Keys.onPressed: {
        switch(event.key){
        case Qt.Key_1:
            ui.state = "register"
			interFace.StartVideo()
			interFace.EnterRegisterUi()
            break
        case Qt.Key_2:
            ui.state = "autoRecognize"
			interFace.StartVideo()
			interFace.EnterRecognizeUi()
            break
        case Qt.Key_3:
            ui.state = "recognize"
			interFace.StartVideo()
			interFace.EnterRecognizeUi()
            break
       case Qt.Key_4:
             ui.state = "calibration"
			 interFace.StartVideo()
			 interFace.EnterCalibUi()
             break
         default:
        }
        event.accepted = true
    }
}
