/* DOFCalc v0.1
   (C)opyright 2019 Alex Tutubalin, LibRaw LLC, lexa@libraw.org
   The program is licensed under the terms of GNU General Public License v3 */

import QtQuick 2.9
import QtQuick.Controls 2.5
Dialog{
    modal: true
    standardButtons: Dialog.Ok
    anchors.centerIn: parent
    width: root.width*0.7
    title: "DOFCalc v0.1"
    contentItem: WhiteText {
        text: "\u00a9 2019 Alex Tutubalin, LibRaw LLC<br/>lexa@libraw.org"
        font.pixelSize: root.font.pixelSize*0.8
        anchors.centerIn: parent
        horizontalAlignment: Text.AlignHCenter
    }
}
