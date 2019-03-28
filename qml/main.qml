/* DOFCalc v0.1
   (C)opyright 2019 Alex Tutubalin, LibRaw LLC, lexa@libraw.org
   The program is licensed under the terms of GNU General Public License v3 */

import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
ApplicationWindow { id: root;
    visible: true
    width: 600
    height: 800
    title: "DOFCalc"
    background:   Rectangle     { color: "#202020"  }
    font.pixelSize:  globalFontSize // set globally before engine started
    HeaderItem { id: topHeader
        height: root.height/8
        anchors.top: root.top
        Row{
            anchors.fill: parent
            HeaderItem {
                width:  parent.width/3
                HeaderTextCell      { id: hyperfocalPrefix; objectName: "hyperfocalPrefix"; width: parent.width*0.35;}
                HeaderTextCellBig   { id: hyperfocalString; objectName: "hyperfocalString"; anchors.left: hyperfocalPrefix.right; }
            }
            HeaderItem {
                width:  parent.width*2/3
                HeaderTextCell      { id: dofNearPrefix; objectName:"dofNearPrefix"  ; width: parent.width * 0.15; }
                HeaderTextCellBig   { id: dofNearString; objectName: "dofNearString"; anchors.left: dofNearPrefix.right; width: parent.width*0.35 }
                HeaderTextCell      { id: dofFarPrefix;  objectName: "dofFarPrefix" ; anchors.left: dofNearString.right; width: parent.width * 0.15; }
                HeaderTextCellBig   { objectName: "dofFarString"; anchors.left: dofFarPrefix.right; width: parent.width*0.35; }
            }
        }
    }
    Grid { id:          mainGrid
        anchors.top:    topHeader.bottom
        height:         root.height*14/15 - topHeader.height
        width:          root.width*8/10
        leftPadding:    (root.width - width) / 2
        columnSpacing:  width/8
		rowSpacing:     height/20
        columns:        3
        WheelHeading {  id: focalString;     objectName: "focalString";}
        WheelHeading {  id: apertureString;  objectName: "apertureString";}
        WheelHeading {  id: distanceString;  objectName: "distanceString"; }
        ScrollerCell {  ListWheel { id: focalView;      objectName: "focalList";    model: focalModel;} }
        ScrollerCell {  ListWheel { id: apertureView;   objectName: "apertureList"; model: apertureModel;} }
        ScrollerCell {  ListWheel { id: distanceView;   objectName: "distanceList"; model: distanceModel } }
    }
    header:    MainScreenToolBar { id:tbar;  width: root.width; height: root.height/15}
    AboutDialog     { id: aboutDialog; }
    SettingsDialog  { id: settingsDialog; objectName: "settingsDialog" }
}
