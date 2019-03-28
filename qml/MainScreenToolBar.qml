/* DOFCalc v0.1
   (C)opyright 2019 Alex Tutubalin, LibRaw LLC, lexa@libraw.org
   The program is licensed under the terms of GNU General Public License v3 */

import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
ToolBar{
    Rectangle{ anchors.fill: parent; color: "#404040"}
    RowLayout {
        anchors.fill: parent
        WhiteText { text: " DOFCalc"; font.pixelSize: tbar.height*0.5; color: "#a0a0a0";  horizontalAlignment: Text.AlignLeft;  Layout.fillWidth: true}
        ToolButton {text: qsTr(":"); onClicked: appmenu.open(); font.pixelSize: tbar.height*0.5;
            Menu { id: appmenu
                MenuItem { text: "About"; onTriggered: aboutDialog.open();}
                MenuItem { text: "Settings"; onTriggered: settingsDialog.open();}
                MenuItem { text: "Quit"; onTriggered: root.close();}
            }
        }
    }
}
