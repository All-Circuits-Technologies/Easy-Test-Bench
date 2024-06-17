// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.0
import QtQuick.Controls 2.15

import "qrc:/file_selection"
import app.managers 1.0

/// Top bar menu associated to files
Menu {
    id: file_menu

    signal itemClicked(int index)

    property bool menuEnabled: true
    property alias menuModel: menu_repeater.model

    Repeater {
        id: menu_repeater

        PopUpMenuItem {
            id: menu_item

            required property var model
            required property string name
            required property bool itemClicked

            contentItem: LeftMenuItemText {
                id: content_item
                text: model.name
                enabled: model.enabled
            }
            enabled: model.enabled
            onClicked: model.itemClicked = true
        }
    }

    function openMenu() {
        if (menuEnabled) {
            file_menu.open()
        } else {
            onTriggered: Qt.callLater(Qt.quit)
        }
    }
}
