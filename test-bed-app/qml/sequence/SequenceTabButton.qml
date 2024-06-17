// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.15
import QtQuick.Controls 2.5

import app.managers 1.0

import "qrc:/basic_items"

/// Button designed for the sequence tab bar
TabButton {
    id: tab_button

    property alias label: label

    height: parent.height
    width: SizeManager.getSizeBasedOnAppWidth(100)

    contentItem: Text {
        id: label
        font.family: FontsManager.latoSemiBold.name
        font.pixelSize: SizeManager.getSizeBasedOnAppWidth(20)
        color: checked ? ColorsManager.white : ColorsManager.grey800
    }

    background: ButtonBackground {
        id: button_background
    }
}
