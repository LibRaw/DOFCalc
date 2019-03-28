/* DOFCalc v0.1
   (C)opyright 2019 Alex Tutubalin, LibRaw LLC, lexa@libraw.org
   The program is licensed under the terms of GNU General Public License v3 */

import QtQuick 2.9
import QtQuick.Controls 2.5
Dialog { id: settingsDialog
    objectName: "settingsDialog"
    modal: true
    anchors.centerIn: parent
    standardButtons:  Dialog.Ok | Dialog.Cancel
    signal settingsDialogChanged()
    onAccepted: settingsDialogChanged()
    title: "Settings"
    contentItem: Grid {
        columns: 2
        width: parent.width
        verticalItemAlignment: Grid.AlignVCenter
        spacing: root.font.pixelSize*0.5
        SettingsDialogRowText	{ text: "Circle of<br>Confusion (\u03bcm):"}
        SpinBox					{ id: coc; objectName: "coc"; from: 3; to: 200;}
        SettingsDialogRowText	{ text: "Base font size:";  }
        SpinBox					{ id: fontSize; objectName: "fontSize"; from: 8; to: 36; }
        SettingsDialogRowText   { text: "Aperture scale:" }
        ComboBox                { width: coc.width; objectName: "apScale"; model: [ "1 stop", "1/2 stop", "1/3 stop", "1/4 stop" ]}
        SettingsDialogRowText   { text: "Distance scale:"}
        ComboBox                { width: coc.width; objectName: "diScale"; model: [ "coarse"," medium", "fine",  ] }
    }
}
