// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.15
import QtQuick.Controls 2.5

import app.managers 1.0

import "qrc:/basic_items"

/// Log view in compact format
Item {
    property int panelSpace: SizeManager.getSizeBasedOnAppWidth(6)
    property int contentPanelsWidth: width - panelSpace
    property int textMargin: SizeManager.getSizeBasedOnAppWidth(4)
    property alias model: list_view.model

    Grid {
        columns: 2
        rows: 2
        spacing: panelSpace
        anchors.fill: parent

        LogPanel {
            id: level_panel
            title: qsTr("(TR) L")
            width: contentPanelsWidth * 0.09
            titleMargin: textMargin
        }

        LogPanel {
            id: description_panel
            title: qsTr("(TR) Description")
            width: contentPanelsWidth * 0.91
            titleMargin: textMargin
        }

        Rectangle {
            id: level_bg
            height: parent.height - level_panel.height - panelSpace
            width: level_panel.width
            color: ColorsManager.grey200
        }

        Rectangle {
            id: description_bg
            height: level_bg.height
            width: description_panel.width
            color: ColorsManager.grey200
        }
    }

    ListView {
        id: list_view
        focus: true
        clip: true
        x: level_bg.x
        y: level_bg.y + textMargin
        height: level_bg.height - 2 * textMargin
        width: parent.width

        highlightMoveDuration: 100
        delegate: CompactLogItem {
            id: list_item

            required property int index

            levelX: level_panel.x
            levelWidth: level_panel.width

            descriptionX: description_panel.x
            descriptionWidth: description_panel.width

            textPadding: textMargin

            alternateColor: ((index % 2) != 0)
        }
    }

    Connections {
        target: list_view.model
        function onCountChanged() {
            /// Creates an automatic scroll
            list_view.positionViewAtEnd()
        }
    }
}
