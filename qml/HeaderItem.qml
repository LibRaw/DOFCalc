/* DOFCalc v0.1
   (C)opyright 2019 Alex Tutubalin, LibRaw LLC, lexa@libraw.org
   The program is licensed under the terms of GNU General Public License v3 */

import QtQuick 2.9
Rectangle {
    height: parent.height
    width: parent.width
    color: "#404040"
    border.color: "black"
    border.width: 4
    gradient: Gradient {
        GradientStop { position: 0.0; color: "#101010" }
        GradientStop { position: 0.5; color: "#404040" }
        GradientStop { position: 1.0; color: "#101010" }
    }
}
