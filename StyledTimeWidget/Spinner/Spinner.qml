/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0

Rectangle {
    id: root
    width: 100
    height: 100
    color: "transparent"

    property alias value: list.currentIndex
    property alias label: caption.text
    property alias labelSize: caption.font.pixelSize
    property alias labelColor: caption.color

    property int minium: 0
    property int maximum: 99

    property alias backgroundColor:picker.color
    property int numSize
    property string numColor

    signal changed(string value)
    Text {
        id: caption
        font.pixelSize: 25;
        text: "Spinner"
        color: "black"
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Rectangle {
        id: picker
        anchors.top: caption.bottom
        anchors.topMargin: 4
        anchors.horizontalCenter: parent.horizontalCenter
        height: root.height//48
        width: root.width//32
        color: "black"
        ListView {
            id: list
            anchors.fill: parent
            highlightRangeMode: ListView.StrictlyEnforceRange
            preferredHighlightBegin: height/3
            preferredHighlightEnd: height/3
            clip: true
            spacing: 10
            model: root.maximum - root.minium + 1
            delegate: Text {
                font.pixelSize: root.numSize;
                color: root.numColor;
                text: {
                    var v = root.minium + index;
                    return v < 10 ? "0" + v : v;
                }
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
        Rectangle {
            anchors.fill: parent
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#FF000000" }
                GradientStop { position: 0.5; color: "#00000000" }
                GradientStop { position: 0.5; color: "#00000000" }
                GradientStop { position: 1.0; color: "#FF000000" }
            }
        }
    }
    onValueChanged: {
        changed(value + root.minium);
    }
}
