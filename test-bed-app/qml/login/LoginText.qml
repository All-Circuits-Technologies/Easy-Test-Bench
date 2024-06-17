// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.0
import QtQuick.Controls 2.5

import app.managers 1.0

/// Text format used in the login page
Text {
    color: ColorsManager.grey80
    font.family: FontsManager.latoRegular.name
    font.pixelSize: SizeManager.getSizeBasedOnAppWidth(24)
    fontSizeMode: Text.Fit
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignRight
}
