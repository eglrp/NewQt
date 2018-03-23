import QtQuick 2.0
import "../Common"
Item {

   anchors.top: titleBar.bottom
   anchors.topMargin: 30
   anchors.left: parent.left
   anchors.leftMargin: 30
   property alias text: promptInfo.text
	
    CamPreview{
        id: camPreview
    }

    Row {
        anchors.top: camPreview.bottom
        anchors.topMargin: 30
        anchors.horizontalCenter: camPreview.horizontalCenter
        spacing: 10
        Image { source: "../Images/info.png"; height: 35;width:  35 }
        Text {
            text: qsTr("温馨提示：")
			color: "#4B9B85"
            font.pixelSize: 25
        }
        Text {
            id: promptInfo
            width: 330
            text: qsTr("请将身份证放入读卡器，整理容貌，免冠进行人像扫描。")
            font.pixelSize: 25
            color: "#515151"
            wrapMode: Text.WordWrap
        }
    }
}
