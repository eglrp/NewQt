import QtQuick 2.0

Item {

        Text {
            id: time
            font.family: "Chalkboard SE"
            font.pixelSize:19
            color: "#515151"
        }
        Text {
            anchors.top: time.bottom
            anchors.horizontalCenter: time.horizontalCenter
            id: date
            font.family: "Chalkboard SE"
            font.pixelSize:19
            color: "#515151"
        }


    Timer {
            interval: 500; running: true; repeat: true
            onTriggered: {
                time.text = getTime()
                date.text = getDate()
            }
        }

    function getDate(){
        var now = new Date()
        var year = now.getYear()+1900
        var month = now.getMonth() +1
        var day = now.getDate()
        return year+'-'+transform(month)+'-'+transform(day)
    }

    function getTime(){
        var now = new Date()
        var hour = now.getHours()
        var minute = now.getMinutes()
        var second = now.getSeconds()
        return transform(hour)+':'+transform(minute)+':'+transform(second)
    }

    function transform(time){
         return  time < 10 ? '0'+ time : time
    }
}
