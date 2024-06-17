// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.0
import QtQuick.Controls 2.5

import app.blocs 1.0
import app.managers 1.0

import "qrc:/basic_items"

/// Pop up containing the software credits
Popup {
    property CreditsPopUpBloc creditsPopUpBloc: CreditsPopUpBloc {
        id: credits_pop_up_bloc
    }

    id: credits_pop_up
    modal: true
    focus: true
    height: SizeManager.getSizeBasedOnAppWidth(420)
    width: SizeManager.getSizeBasedOnAppWidth(700)
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    /// Button to close the pop up
    CircularButton {
        imgSource: "qrc:/images/cancel_icon_white.svg"
        palette.button: ColorsManager.orange400
        height: width
        width: SizeManager.getSizeBasedOnAppWidth(26)
        anchors.top: parent.top
        anchors.topMargin: SizeManager.getSizeBasedOnAppWidth(4)
        anchors.right: parent.right
        anchors.rightMargin: SizeManager.getSizeBasedOnAppWidth(4)
        padding: SizeManager.getSizeBasedOnAppWidth(4)
        onClicked: credits_pop_up.close()
    }

    /// Software logo
    Image {
        width: SizeManager.getSizeBasedOnAppWidth(218)
        height: SizeManager.getSizeBasedOnAppWidth(65)
        anchors.top: parent.top
        fillMode: Image.PreserveAspectFit
        source: "qrc:/images/easy_test_bench_full.svg"
        anchors.topMargin: SizeManager.getSizeBasedOnAppWidth(27)
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: SizeManager.getSizeBasedOnAppWidth(18)
        anchors.rightMargin: SizeManager.getSizeBasedOnAppWidth(19)

        CreditsText {
            text: credits_pop_up_bloc.appVersion
            color: ColorsManager.grey80
            font.family: FontsManager.latoRegular.name
            font.pixelSize: SizeManager.getSizeBasedOnAppWidth(16)
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.bottomMargin: SizeManager.getSizeBasedOnAppWidth(0)
            anchors.rightMargin: SizeManager.getSizeBasedOnAppWidth(0)
        }
    }

    /// Years of production and author of the software label
    CreditsText {
        text: credits_pop_up_bloc.yearsOfProduction + "   " + credits_pop_up_bloc.author
        font.pixelSize: SizeManager.getSizeBasedOnAppWidth(16)
        anchors.top: parent.top
        anchors.topMargin: SizeManager.getSizeBasedOnAppWidth(157)
        anchors.bottom: parent.bottom
        anchors.bottomMargin: SizeManager.getSizeBasedOnAppWidth(244)
        anchors.left: parent.left
        anchors.leftMargin: SizeManager.getSizeBasedOnAppWidth(56)
        anchors.right: parent.right
        anchors.rightMargin: SizeManager.getSizeBasedOnAppWidth(346)
    }

    /// Company info
    CreditsText {
        id: address_credits
        text: credits_pop_up_bloc.address + '\n' + credits_pop_up_bloc.country
              + '\n' + credits_pop_up_bloc.website
        font.pixelSize: SizeManager.getSizeBasedOnAppWidth(16)
        anchors.top: parent.top
        anchors.topMargin: SizeManager.getSizeBasedOnAppWidth(207)
        anchors.bottom: parent.bottom
        anchors.bottomMargin: SizeManager.getSizeBasedOnAppWidth(137)
        anchors.left: parent.left
        anchors.leftMargin: SizeManager.getSizeBasedOnAppWidth(56)
        anchors.right: parent.right
        anchors.rightMargin: SizeManager.getSizeBasedOnAppWidth(429)
    }

    /// Company Logo
    Image {
        id: all_circuits_design_center_logo
        source: "qrc:/images/all_circuits_design_center.png"
        fillMode: Image.PreserveAspectFit
        anchors.bottom: parent.bottom
        anchors.bottomMargin: SizeManager.getSizeBasedOnAppWidth(0)
        anchors.left: parent.left
        anchors.leftMargin: SizeManager.getSizeBasedOnAppWidth(570)
        anchors.right: parent.right
        anchors.rightMargin: SizeManager.getSizeBasedOnAppWidth(0)
    }

    background: Rectangle {
        color: ColorsManager.grey900
    }
}
