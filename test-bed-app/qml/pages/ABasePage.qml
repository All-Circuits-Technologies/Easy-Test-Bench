// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.0
import QtQuick.Window 2.13
import QtQuick.Controls 2.13

import app.managers 1.0

import "qrc:/top_bar"

/// Base class for app pages
Page {
    signal topBarLeftButtonClicked

    property int topBarHeight: SizeManager.getSizeBasedOnAppWidth(40)
    property alias topBar: top_bar

    background: Rectangle {
        color: ColorsManager.grey800
    }

    TopBar {
        id: top_bar
        leftButtonText: topBarLeftButtonText
        height: topBarHeight
        width: parent.width
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 0
        anchors.topMargin: 0
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.5}
}
##^##*/

