import QtQuick 2.7
import QtQuick.Window 2.2
import QtMultimedia 5.9
import CustomFilter 1.0

Rectangle {
    id: root
    color: "transparent"

    width: 300
    height: 400

    signal inputQrCode

    Camera {
        objectName: "CameraObject"
        id: camera
        digitalZoom: 1
        imageProcessing.whiteBalanceMode: CameraImageProcessing.WhiteBalanceFlash
        exposure {
            exposureCompensation: -1.0
            exposureMode: Camera.ExposurePortrait
        }
        flash.mode: Camera.FlashRedEyeReduction
        focus {
            focusMode: Camera.FocusContinuous + Camera.FocusAuto
            focusPointMode: Camera.FocusPointCenter
        }
    }

    VideoOutput {
        id: videoOutput
        objectName: "videoOutputObject"
        source: camera
        anchors.fill: parent
        focus : visible
        autoOrientation: true
        filters: [filter]
        fillMode: VideoOutput.PreserveAspectCrop
    }

    CustomFilter {
        objectName: "cameraFilterObject"
        id: filter
        orientation: videoOutput.orientation
    }

    Rectangle{
        id: viewRect
        x: root.x + root.width/4
        y: root.y + root.height/6
        width: root.width/2
        height:root.width/2
        border.color: "gray"
        border.width: 2
        radius: 5
        color:"transparent"
    }

    Timer {
        interval: 2000; running: true; repeat: true
        onTriggered:{
            viewRect.border.color = "gray"
            if (camera.lockStatus == Camera.Unlocked){
                camera.searchAndLock();
                viewRect.border.color = "green";
            }else if(camera.lockStatus == Camera.Locked){
                camera.unlock();
                camera.searchAndLock();
                viewRect.border.color = "green";
            }
        }
    }

    Text{
        id: scanTips
        width: root.width
        anchors.top: viewRect.bottom
        text:"将二维码放入框内，即可快速扫描。"
        font.family: "MicrosoftYaHei"
        font.pointSize: 16
        color: "white"
        horizontalAlignment:Text.AlignHCenter
    }

    Text{
        objectName: "inputQRCodeObject"

        topPadding:100
        width: root.width
        anchors.top: scanTips.bottom
        text:"手动输入资产编码"
        font.family: "MicrosoftYaHei"
        font.pointSize: 20
        color: "white"
        horizontalAlignment:Text.AlignHCenter

        MouseArea{
            anchors.fill:parent
            onClicked:{
                root.inputQrCode()
                console.log("input qr code mannually.")
            }
        }
    }
}
