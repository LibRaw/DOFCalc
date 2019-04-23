/* DOFCalc v0.1
   (C)opyright 2019 Alex Tutubalin, LibRaw LLC, lexa@libraw.org
   The program is licensed under the terms of GNU General Public License v3 */

import QtQuick 2.9
WhiteText {
    font.pixelSize:         root.font.pixelSize*1.2
    horizontalAlignment:    Text.AlignHCenter
    height:                 root.font.pixelSize*2
    padding:                root.font.pixelSize/4
    width:                  root.width*0.74/3
    Rectangle  { height: 2;  width: parent.width; y: parent.height-2;color: "#f0f0f0" }
}
