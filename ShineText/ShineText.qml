import QtQuick 2.0
import QtGraphicalEffects 1.0
Rectangle{
    id: root

    property string text
    property int fontSize: 8
    property bool fontBold: false

    // 背景文字（灰色）
    Text{
        text: root.text
        font.pointSize: root.fontSize
        font.bold: root.fontBold
        color: 'gray'
        opacity: 0.5
    }

    // 前景文字（白色）
    Text{
        id: txt
        text: root.text
        font.pointSize: root.fontSize
        font.bold: root.fontBold
        color: 'white'
        visible: false
    }

    // 半透明遮罩
    OpacityMask {
        id: effect
        width: txt.contentWidth;
        height: txt.contentHeight
        source: txt
        maskSource: gradient
    }

    // 用渐变做遮罩层
    RadialGradient {
        id: gradient
        visible: false
        width: txt.contentWidth;
        height: txt.contentHeight
        horizontalRadius: width/2
        verticalRadius: height*4/5
        horizontalOffset: 0
        gradient: Gradient{
            GradientStop{position: 0.2; color:'white'}
            GradientStop{position: 0.8; color:'transparent'}
        }

        // 用动画移动遮罩
        PropertyAnimation{
            target: gradient
            properties: 'horizontalOffset'
            from: -gradient.width
            to: gradient.width + gradient.horizontalRadius
            duration: 2000
            running: true
            loops: -1
        }
    }
}
