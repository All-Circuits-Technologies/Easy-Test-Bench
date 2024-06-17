// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.0
import QtGraphicalEffects 1.15

import "qrc:/basic_items"

import app.managers 1.0

/// Main button
CustomMainButton {
    id: rec_button

    property bool isPrimaryColor: true

    enabledColor: isPrimaryColor ? ColorsManager.orange400 : ColorsManager.grey400
    textColor: ColorsManager.white
}
