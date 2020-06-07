import QtQuick 2.5
import QtQuick.Window 2.2

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Rectangle{
        anchors.fill: parent
        color: "#000000"
        radius: 20

        ShineText{
            anchors.centerIn: parent
            text: '> 滑动来解锁'
            fontSize: 16
        }
    }
}
