// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5

import app.managers 1.0
import app.blocs 1.0

import "qrc:/basic_items"

/// List that allows the user to select a project and a sequence
ColumnLayout {
    property alias title: title_text.text
    property alias model: list_view.model
    property alias count: list_view.count
    property alias currentIndex: list_view.currentIndex

    /// Title
    Text {
        id: title_text
        color: ColorsManager.grey80
        font.pixelSize: SizeManager.getSizeBasedOnAppWidth(24)
        Layout.bottomMargin: SizeManager.getSizeBasedOnAppWidth(8)
    }

    RowLayout {
        Layout.fillWidth: parent
        spacing: 0
        /// Projects display
        Rectangle {
            id: project_list_background
            color: ColorsManager.grey80
            Layout.fillWidth: parent
            Layout.preferredHeight: SizeManager.getSizeBasedOnAppHeight(200)

            ListView {
                id: list_view
                focus: true
                clip: true
                ScrollBar.vertical: scroll_bar
                anchors.fill: parent
                highlightMoveDuration: 100

                delegate: FileListItem {
                    id: list_item

                    required property bool missingFile
                    required property string name
                    required property string uniquePath
                    required property int index

                    nameText.text: missingFile ? name + qsTr(
                                                     " (TR) (missing)") : name
                    pathText.text: uniquePath
                    italicName: missingFile
                    nameText.color: ListView.isCurrentItem ? ColorsManager.white : ColorsManager.grey800
                    pathText.color: ListView.isCurrentItem ? ColorsManager.grey80 : ColorsManager.grey600
                    height: SizeManager.getSizeBasedOnAppWidth(34)
                    width: list_view.width
                    onClicked: list_view.currentIndex = index
                }

                highlight: Rectangle {
                    color: ColorsManager.orange400
                }
            }
        }

        VerticalScrollBar {
            id: scroll_bar
            flickable: list_view
            buttonsHeight: SizeManager.getSizeBasedOnAppWidth(16)
            Layout.preferredHeight: project_list_background.height
            Layout.preferredWidth: SizeManager.getSizeBasedOnAppWidth(20)
        }
    }
}
