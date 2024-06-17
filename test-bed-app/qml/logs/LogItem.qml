// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphaël Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.15
import QtQuick.Controls 2.15

import app.managers 1.0
import app.models 1.0

/// Item displaying log info, designed for the LogView
Item {
    id: log_item

    readonly property int topBottomPadding: SizeManager.getSizeBasedOnAppHeight(
                                                3)

    property alias timeX: time_text.x
    property alias categoryX: category_text.x
    property alias levelX: level_text.x
    property alias descriptionX: description_text.x
    property alias timeWidth: time_text.width
    property alias categoryWidth: category_text.width
    property alias levelWidth: level_text.width
    property alias descriptionWidth: description_text.width

    property bool alternateColor: false

    required property string dateTimeStr
    required property string timeStr
    required property string categoryStr
    required property int level
    required property string levelStr
    required property string description

    required property int textPadding

    height: description_text.height

    LogText {
        id: time_text
        toolTipText: dateTimeStr
        textContent.text: timeStr
        textContent.leftPadding: textPadding
        textContent.rightPadding: textPadding
        textContent.topPadding: topBottomPadding
        textContent.bottomPadding: topBottomPadding
        logCriticity: level
        alternateColor: log_item.alternateColor
        height: description_text.height
    }

    LogText {
        id: category_text
        toolTipText: categoryStr
        textContent.text: categoryStr
        textContent.leftPadding: textPadding
        textContent.rightPadding: textPadding
        textContent.topPadding: topBottomPadding
        textContent.bottomPadding: topBottomPadding
        logCriticity: level
        alternateColor: log_item.alternateColor
        height: description_text.height
    }

    LogText {
        id: level_text
        textContent.text: levelStr
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
        textContent.text: description
        textContent.wrapMode: Text.Wrap
        height: textContent.contentHeight + topBottomPadding * 2
        textContent.elide: Text.ElideNone
        textContent.leftPadding: textPadding
        textContent.rightPadding: textPadding
        textContent.topPadding: topBottomPadding
        textContent.bottomPadding: topBottomPadding
        logCriticity: level
        alternateColor: log_item.alternateColor
    }
}
