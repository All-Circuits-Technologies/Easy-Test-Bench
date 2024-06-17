// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.0

import app.managers 1.0

/// Right-aligned menuItem text format
Text {
    color: enabled ? ColorsManager.grey800 : ColorsManager.grey600
    font.italic: !enabled
    font.family: FontsManager.latoSemiBold.name
    font.pixelSize: SizeManager.getSizeBasedOnAppWidth(18)
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignRight
    anchors.right: parent.right
    anchors.rightMargin: SizeManager.getSizeBasedOnAppWidth(22)
}
