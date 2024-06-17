// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.15
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.15

import app.models 1.0
import app.managers 1.0

import "qrc:/basic_items"

/// Screen that displays log data
Item {
    readonly property int panelSpace: SizeManager.getSizeBasedOnAppWidth(6)
    readonly property int contentPanelsWidth: width - 4 * panelSpace - search_icon.width
    readonly property int textMargin: SizeManager.getSizeBasedOnAppWidth(9)
    property alias model: list_view.model

    Grid {
        columns: 5
        rows: 2
        spacing: panelSpace
        anchors.fill: parent

        LogPanel {
            id: time_panel
            title: qsTr("(TR) Time")
            width: 0.12 * contentPanelsWidth
            titleMargin: textMargin
        }

        LogPanel {
            id: category_panel
            title: qsTr("(TR) Category")
            width: 0.13 * contentPanelsWidth
            titleMargin: textMargin
        }

        LogPanel {
            id: level_panel
            title: qsTr("(TR) Level")
            width: 0.11 * contentPanelsWidth
            titleMargin: textMargin
        }

        LogPanel {
            id: description_panel
            title: qsTr("(TR) Description")
            width: 0.64 * contentPanelsWidth
            titleMargin: textMargin
        }

        RecButton {
            id: search_icon

            buttonColors.enabledColor: ColorsManager.orange400
            buttonColors.pressedColor: ColorsManager.grey500
            height: SizeManager.getSizeBasedOnAppWidth(30)
            width: SizeManager.getSizeBasedOnAppWidth(30)
            icon.source: "qrc:/images/search.svg"
            icon.color: ColorsManager.grey50
            icon.height: width
            icon.width: width

            enabled: false
        }

        Rectangle {
            id: time_bg
            height: scroll_bar.height
            width: time_panel.width
            color: ColorsManager.grey200
        }

        Rectangle {
            id: category_bg
            height: scroll_bar.height
            width: category_panel.width
            color: ColorsManager.grey200
        }

        Rectangle {
            id: level_bg
            height: scroll_bar.height
            width: level_panel.width
            color: ColorsManager.grey200
        }

        Rectangle {
            id: description_bg
            height: scroll_bar.height
            width: description_panel.width
            color: ColorsManager.grey200
        }

        VerticalScrollBar {
            id: scroll_bar
            alwaysVisible: true
            flickable: list_view
            width: SizeManager.getSizeBasedOnAppWidth(30)
            buttonsHeight: SizeManager.getSizeBasedOnAppWidth(30)
            height: parent.height - panelSpace - search_icon.height
        }
    }

    ListView {
        id: list_view
        focus: true
        clip: true
        x: time_bg.x
        y: time_bg.y + textMargin
        height: parent.height - panelSpace - search_icon.height - 2 * textMargin
        width: parent.width - panelSpace - scroll_bar.width
        ScrollBar.vertical: scroll_bar

        highlightMoveDuration: 100
        delegate: LogItem {
            id: list_item

            required property int index

            timeX: time_panel.x
            timeWidth: time_panel.width

            categoryX: category_panel.x
            categoryWidth: category_panel.width

            levelX: level_panel.x
            levelWidth: level_panel.width

            descriptionX: description_panel.x
            descriptionWidth: description_panel.width

            textPadding: textMargin

            alternateColor: ((index % 2) != 0)
        }

        Connections {
            target: list_view.model
            function onCountChanged() {
                // Creates an automatic scroll when the cursor is positioned at the end of the list
                if (list_view.atYEnd) {
                    list_view.positionViewAtEnd()
                }
            }
        }
    }
}
