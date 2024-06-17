// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.15
import QtQuick.Controls 2.15

import app.managers 1.0

/// Item displaying log info, designed for the CompactLogView
Item {
    id: log_item

    readonly property int topBottomPadding: SizeManager.getSizeBasedOnAppHeight(
                                                3)

    property alias levelX: level_text.x
    property alias descriptionX: description_text.x
    property alias levelWidth: level_text.width
    property alias descriptionWidth: description_text.width

    property bool alternateColor: false

    required property int level
    required property string levelStr
    required property string shortLevelStr
    required property string description

    required property int textPadding

    height: description_text.height

    LogText {
        id: level_text

        toolTipText: levelStr
        textContent.text: shortLevelStr
        textContent.clip: true
        textContent.elide: Text.ElideNone
        textContent.horizontalAlignment: Text.AlignHCenter
        textContent.leftPadding: textPadding
        textContent.rightPadding: textPadding
        textContent.topPadding: topBottomPadding
        textContent.bottomPadding: topBottomPadding

        logCriticity: level
        alternateColor: log_item.alternateColor

        height: description_text.height
    }

    LogText {
        id: description_text

        toolTipText: description
        textContent.text: description
        textContent.leftPadding: textPadding
        textContent.rightPadding: textPadding
        textContent.topPadding: topBottomPadding
        textContent.bottomPadding: topBottomPadding

        logCriticity: level
        alternateColor: log_item.alternateColor

        height: textContent.contentHeight + topBottomPadding * 2
    }
}
