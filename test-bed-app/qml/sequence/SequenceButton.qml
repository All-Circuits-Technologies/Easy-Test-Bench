// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.0

import app.managers 1.0

import "qrc:/basic_items"

/// Button with label used to manage sequence execution
Column {
    property alias button: button
    property string label: ""

    spacing: SizeManager.getSizeBasedOnAppHeight(6)

    RecButton {
        id: button

        buttonColors.enabledColor: ColorsManager.grey400
        buttonColors.hoveredColor: ColorsManager.grey500
        buttonColors.pressedColor: ColorsManager.grey600
        height: SizeManager.getSizeBasedOnAppWidth(75)
        backgroundRadius: SizeManager.getSizeBasedOnAppWidth(12)
        width: SizeManager.getSizeBasedOnAppWidth(75)
        icon.color: ColorsManager.grey50
        icon.height: width * 0.6
        icon.width: height * 0.6
        anchors.horizontalCenter: parent.horizontalCenter
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
