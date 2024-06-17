// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.0

import app.managers 1.0

/// Represent the colors of a button depending of its state
QtObject {
    property color enabledColor: ColorsManager.grey400
    property color disabledColor: ColorsManager.grey600
    property color pressedColor: ColorsManager.grey200
    property color hoveredColor: ColorsManager.grey300
    property color checkedColor: ColorsManager.orange400
}
