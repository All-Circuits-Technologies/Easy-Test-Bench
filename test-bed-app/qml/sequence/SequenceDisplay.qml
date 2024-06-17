// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.0

import app.managers 1.0

/// Panel use to dsiplay sequence nnformation
Column {
    property alias rectangle: rectangle
    property string label: ""
    property string textDisplayed: ""

    spacing: SizeManager.getSizeBasedOnAppWidth(10)

    Rectangle {
        id: rectangle
        color: ColorsManager.grey80
        height: SizeManager.getSizeBasedOnAppWidth(75)
        width: SizeManager.getSizeBasedOnAppWidth(183)
        anchors.horizontalCenter: parent.horizontalCenter
        Text {
            text: textDisplayed
            color: ColorsManager.grey500
            font.bold: true
            font.family: FontsManager.latoRegular.name
            font.pixelSize: SizeManager.getSizeBasedOnAppWidth(50)
            elide: Text.ElideRight
            height: parent.height
            width: parent.width
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
    }

    Text {
        text: label
        color: ColorsManager.grey500
        font.family: FontsManager.latoRegular.name
        font.pixelSize: SizeManager.getSizeBasedOnAppWidth(18)
        height: contentHeight
        width: contentWidth
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
