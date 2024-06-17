// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.0
import QtQuick.Controls 2.5

import app.managers 1.0

import "qrc:/basic_items"

/// Generic vertical scroll bar with buttons
ScrollBar {
    property Flickable flickable
    property int buttonsHeight
    property int buttonFlick: 300
    property bool alwaysVisible: false

    id: scroll_bar
    visible: alwaysVisible ? true : flickable.contentHeight > flickable.height ? true : false
    minimumSize: 0.1
    topPadding: buttonsHeight
    bottomPadding: buttonsHeight
    leftPadding: 0
    rightPadding: 0

    /// Top button
    RecButton {
        buttonColors.enabledColor: ColorsManager.grey600
        buttonColors.pressedColor: ColorsManager.grey500
        height: buttonsHeight
        width: scroll_bar.width
        icon.source: "qrc:/images/up_arrow.svg"
        icon.color: ColorsManager.grey50
        icon.height: width
        icon.width: width
        padding: SizeManager.getSizeBasedOnAppWidth(4)
        anchors.top: parent.top
        onPressed: flickable.flick(0, buttonFlick)
    }

    /// Bottom button
    RecButton {
        buttonColors.enabledColor: ColorsManager.grey600
        buttonColors.pressedColor: ColorsManager.grey500
        height: buttonsHeight
        width: scroll_bar.width
        icon.source: "qrc:/images/down_arrow.svg"
        icon.color: ColorsManager.grey50
        icon.height: width
        icon.width: width
        padding: SizeManager.getSizeBasedOnAppWidth(4)
        anchors.bottom: parent.bottom
        onPressed: flickable.flick(0, -buttonFlick)
    }

    /// Scroll item
    contentItem: Rectangle {
        color: pressed ? ColorsManager.grey400 : ColorsManager.grey500
        implicitWidth: scroll_bar.width
        ClickableItem {}
    }

    background: Rectangle {
        color: ColorsManager.grey200
    }
}
