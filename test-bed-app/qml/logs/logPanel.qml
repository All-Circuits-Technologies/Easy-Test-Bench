// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.15
import QtQuick.Layouts 1.15

import app.managers 1.0

/// Panel that displays a single piece of data from a log
Rectangle {
    id: header_background

    property alias title: header_text.text
    property int titleMargin

    color: ColorsManager.black
    height: SizeManager.getSizeBasedOnAppWidth(30)
    width: parent.width

    Text {
        id: header_text

        elide: Text.ElideRight
        color: ColorsManager.grey60
        font.family: FontsManager.latoSemiBold.name
        font.pixelSize: SizeManager.getSizeBasedOnAppWidth(18)
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.Alignleft
        anchors.fill: parent
        anchors.leftMargin: titleMargin
    }
}
