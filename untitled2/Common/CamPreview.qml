import QtQuick 2.5
import QtMultimedia 5.6

//import an.qt.interFace 1.0

Item {
    //id: root
    width: 550
    height: 350
	property alias spaceing: rowView.spacing

    Row{
		id:rowView
        spacing: 30
        Frame {
            id: frame1
            source: "../Images/camFrame.png"
            width: 260
            height: 350
            margins: 15
            Image {
                id: left
				anchors.fill: parent
                anchors.margins: 15
                source: "image://imageProvider/0" 
				cache: false
            }	
        }

        Frame {
            id: frame2
            source: "../Images/camFrame.png"
            width: 260
            height: 350
            margins: 15
            Image {
                id: right
				anchors.fill: parent
                anchors.margins: 15
                source: "image://imageProvider/1" 
				cache: false
            }		
	  }
	 Connections{
				target: interFace;
				onImageRefresh:{
					left.source=""
					left.source="image://imageProvider/0" 

					right.source=""
					right.source="image://imageProvider/1"
				}
			}
	}
}
