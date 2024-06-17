// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.0
import QtQuick.Window 2.13
import QtQuick.Controls 2.13

import app.managers 1.0

import "qrc:/credits"
import "qrc:/top_bar"

/// Base class for app pages
Page {
    property int topBarHeight : SizeManager.getSizeBasedOnAppWidth(40)
    property int topBarLeftButtonX : top_bar.leftButtonX
    property string topBarTitle
    property string topBarLeftButtonText
    property bool profilTopBarItemDisplayed: true
    signal topBarLeftButtonCLicked

    background: Rectangle
    {
        color : ColorsManager.darkGrey1
    }

    TopBar {
        id: top_bar
        title: topBarTitle
        leftButtonText: topBarLeftButtonText
        height: topBarHeight
        width: parent.width
        profilItemDisplayed: profilTopBarItemDisplayed
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 0
        anchors.topMargin: 0
        onLeftButtonClicked: topBarLeftButtonCLicked()
        onLogoButtonClicked: credits_pop_up.open()
    }

    CreditsPopUp {
        id: credits_pop_up
        anchors.centerIn: parent
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.5}
}
##^##*/
