// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import app.managers 1.0

//Generic label with a vertical scrollBar that appears when text doesn't fit its area
Rectangle {
    property alias contentText: content_text
    id: area_background

    RowLayout {
        height: parent.height
        width: parent.width
        spacing: 0

        Flickable {
            id: flickable_area
            interactive: flickable_area.contentHeight > flickable_area.height ? true : false
            clip: true
            ScrollBar.vertical: scroll_bar
            flickableDirection: Flickable.VerticalFlick
            contentHeight: content_text.height + content_text.anchors.topMargin
                           + SizeManager.getSizeBasedOnAppWidth(10)
            contentWidth: width
            Layout.fillHeight: parent
            Layout.fillWidth: parent

            Text {
                id: content_text
                wrapMode: Text.Wrap
                anchors.top: parent.top
                anchors.topMargin: SizeManager.getSizeBasedOnAppWidth(10)
                anchors.left: parent.left
                anchors.leftMargin: SizeManager.getSizeBasedOnAppWidth(10)
                anchors.right: parent.right
                anchors.rightMargin: SizeManager.getSizeBasedOnAppWidth(10)
            }
        }

        VerticalScrollBar {
            id: scroll_bar
            flickable: flickable_area
            buttonsHeight: SizeManager.getSizeBasedOnAppWidth(16)
            Layout.preferredWidth: SizeManager.getSizeBasedOnAppWidth(20)
            Layout.preferredHeight: area_background.height
        }
    }
}
