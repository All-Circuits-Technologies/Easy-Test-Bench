// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphaël Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import app.managers 1.0

import "qrc:/basic_items"

/// Item designed for projectListView
AbstractButton {
    id: file_list_item

    property alias nameText: name_text
    property alias pathText: path_text
    property alias italicName: name_text.font.italic

    ToolTip.text: name_text.text + " " + path_text.text
    ToolTip.visible: hovered
    ToolTip.delay: 500

    RowLayout {
        Layout.maximumWidth: parent.width

        Text {
            id: name_text
            elide: Text.ElideRight
            font.family: FontsManager.latoRegular.name
            font.pixelSize: SizeManager.getSizeBasedOnAppWidth(24)
            Layout.preferredHeight: file_list_item.height
            Layout.maximumWidth: file_list_item.width - path_text.width
            Layout.leftMargin: SizeManager.getSizeBasedOnAppWidth(9)
        }

        Text {
            id: path_text
            elide: Text.ElideRight
            font.family: FontsManager.latoRegular.name
            font.pixelSize: SizeManager.getSizeBasedOnAppWidth(20)
            verticalAlignment: Text.AlignVCenter
            Layout.preferredHeight: file_list_item.height
            Layout.maximumWidth: file_list_item.width / 2
            Layout.leftMargin: SizeManager.getSizeBasedOnAppWidth(5)
            Layout.alignment: Qt.AlignVCenter
        }
    }

    ClickableItem {}
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

