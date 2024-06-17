// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

import app.managers 1.0

import "qrc:/basic_items"

/// Pop up with a shaded effect
Popup
{
    id: shaded_pop_up
    closePolicy: Popup.NoAutoClose
    modal: true
    focus: true
    leftPadding: 0
    rightPadding: 0
    topPadding: 0
    bottomPadding: 0

    DropShadow {
        color: ColorsManager.getColorWithAlpha(ColorsManager.black, 0.6)
        horizontalOffset: 5
        verticalOffset: 5
        radius: 5
        samples: 7
        width: shaded_pop_up.width
        height: shaded_pop_up.height
        source: pop_up_background
    }
}
