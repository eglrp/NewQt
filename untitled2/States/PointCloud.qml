import QtQuick 2.0
import "../Common"
import OpenGL3DShow 1.0


Item {
    width: 550
    height:390
	FramePicture {
        id: pointCloud
		objectName: "myCloud"
        frameSource: "../Images/PCloud.png"
        margins: 15
        pictureSource:"../Images/pointCloud.jpg"
        anchors.fill: parent
		 

		// onCompleted:{
		//	view.get_windows_hwnd_from_title("myCloud")
		//	view.InitOpenGL(view.readOpenGL)
		//}
    }

	// View3D
	//	{
	//		id: view
	//		anchors.fill: parent
	//		ParallelAnimation
	//		{
	//			running: true
	//			NumberAnimation
	//			{
	//				target: cube
	//				property: "rotateAngle"
	//				from: 0
	//				to: 360
	//				duration: 5000
	//			}
	//
	//			Vector3dAnimation
	//			{
	//				target: cube
	//				property: "axis"
	//				from: Qt.vector3d( 0, 1, 0 )
	//				to: Qt.vector3d( 1, 0, 0 )
	//				duration: 5000
	//			}
	//			loops: Animation.Infinite
	//		}
	//	}
	 
	
    

    Row {
        anchors.top: pointCloud.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: pointCloud.horizontalCenter
        spacing: 10
         
		

         

        Text {
            text: qsTr("构建完成")
            font.pixelSize: 25
            color: "#515151"
        }
    }

   
}
