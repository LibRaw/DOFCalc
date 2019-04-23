/* DOFCalc v0.1
   (C)opyright 2019 Alex Tutubalin, LibRaw LLC, lexa@libraw.org
   The program is licensed under the terms of GNU General Public License v3 */

import QtQuick 2.9
import QtQuick.Controls 2.5
ScrollView  {
    width: root.width *0.74/3;
    height: mainGrid.height*0.9-focalString.height
    ScrollBar.vertical.policy: ScrollBar.AlwaysOff;
}
