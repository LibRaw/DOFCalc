/* DOFCalc v0.1
    (C)opyright 2019 Alex Tutubalin, LibRaw LLC, lexa@libraw.org
    The program is licensed under the terms of GNU General Public License v3 */

import QtQuick 2.9
import QtQuick.Controls 2.5
ListView {
    signal myViewIndexChanged(int idx)
    onCurrentIndexChanged: myViewIndexChanged(currentIndex)
    anchors.fill: parent
    highlight: Rectangle { color: "gray"; radius: 5; border.color: "white"; border.width: 1;}
    preferredHighlightBegin: height/2 - root.font.pixelSize*0.75
    preferredHighlightEnd: height/2 +  root.font.pixelSize*0.75
    highlightRangeMode: ListView.StrictlyEnforceRange
    delegate: ItemDelegate {
        text: displayname;
        width: parent.width
        height: root.font.pixelSize * (0.8+0.6*model.rowsize);
        highlighted: ListView.isCurrentItem
        contentItem:  WhiteText {
            color: highlighted? "white" : "black"
            text: parent.text
            font: parent.font
        }
        onClicked: { ListView.view.currentIndex = index; }
    }
    Rectangle {
        opacity: 0.5
        width: parent.width; height: parent.height; z: -1;
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#202020" }
            GradientStop { position: 0.5; color: "white" }
            GradientStop { position: 1.0; color: "#202020" }
        }
    }
}
