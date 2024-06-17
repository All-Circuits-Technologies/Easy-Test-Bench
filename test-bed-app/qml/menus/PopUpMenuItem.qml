// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick.Controls 2.15
import QtQuick 2.0

import app.managers 1.0

import "qrc:/managers"
import "qrc:/basic_items"

/// Animated pop up menu item
MenuItem {
    id: pop_up_menu_item

    height: SizeManager.getSizeBasedOnAppWidth(50)
    width: parent.width

    property alias buttonColors: button_background.buttonColors

    background: ButtonBackground {
        id: button_background
        buttonColors.enabledColor: ColorsManager.grey80
        buttonColors.disabledColor: ColorsManager.grey80
        buttonColors.pressedColor: ColorsManager.grey200
        buttonColors.hoveredColor: ColorsManager.grey100
    }
}
