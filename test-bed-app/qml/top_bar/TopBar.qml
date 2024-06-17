// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.5
import QtQuick.Layouts 1.13
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtLocation 5.6
import QtGraphicalEffects 1.0

import app.blocs 1.0
import app.managers 1.0

import "qrc:/basic_items"
import "qrc:/credits"
import "qrc:/menus"

Rectangle {
    id: top_bar_background

    /// Backend bloc linked with this view
    readonly property TopBarBloc topBarBloc: TopBarBloc {
        id: top_bar_bloc
    }

    color: ColorsManager.grey400
    property alias title: title_text.text
    property alias secondaryTitle: secondary_text.text
    property string leftButtonText
    property bool profilItemDisplayed: true
    property alias fileMenu: file_menu

    RowLayout {
        id: left_part
        anchors.left: parent.left
        anchors.top: parent.top
        height: parent.height
        spacing: SizeManager.getSizeBasedOnAppWidth(15)

        /// App logo button that displays the credits when clicked
        RecButton {
            id: logo_button
            icon.source: "qrc:/images/easy_test_bench_reduce.svg"
            icon.color: ColorsManager.grey50
            icon.height: parent.height
            icon.width: parent.height
            buttonColors.pressedColor: ColorsManager.grey300
            buttonColors.hoveredColor: ColorsManager.grey200
            Layout.preferredHeight: parent.height
            Layout.preferredWidth: parent.height
            padding: SizeManager.getSizeBasedOnAppWidth(3)
            onClicked: credits_pop_up.open()
        }

        /// Button at the left of the top bar, changes depending on the page
        RecButton {
            id: left_button
            Layout.preferredHeight: parent.height
            Layout.preferredWidth: left_button_label.contentWidth +
                                   SizeManager.getSizeBasedOnAppWidth(10)
            Layout.minimumWidth: parent.height
            buttonColors.pressedColor: ColorsManager.grey300
            buttonColors.hoveredColor: ColorsManager.grey200
            onClicked: file_menu.openMenu()

            Text {
                id: left_button_label
                text: leftButtonText
                color: ColorsManager.grey50
                font.family: FontsManager.latoSemiBold.name
                font.pixelSize: SizeManager.getSizeBasedOnAppWidth(18)
                anchors.centerIn: parent
            }
        }
    }

    /// Page title label
    RowLayout {
        anchors.centerIn: parent
        height: parent.height
        spacing: SizeManager.getSizeBasedOnAppWidth(13)

        Item {
            Layout.minimumWidth: secondary_text.contentWidth
            Layout.preferredWidth: secondary_text.contentWidth
            Layout.maximumWidth: secondary_text.contentWidth
            height: parent.height
            visible: secondary_text.text.length > 0
        }

        Text {
            id: title_text
            color: ColorsManager.grey50
            font.family: FontsManager.latoRegular.name
            font.bold: true
            font.pixelSize: SizeManager.getSizeBasedOnAppWidth(22)
            height: parent.height
            elide: Text.ElideRight
        }

        Item {
            id: secondary_text_item
            Layout.minimumWidth: secondary_text.contentWidth
            Layout.preferredWidth: secondary_text.contentWidth
            Layout.maximumWidth: secondary_text.contentWidth
            height: parent.height
            visible: secondary_text.text.length > 0

            Text {
                id: secondary_text
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.bottomMargin: SizeManager.getSizeBasedOnAppWidth(6)
                verticalAlignment: Text.AlignBottom
                color: ColorsManager.grey80
                font.family: FontsManager.latoSemiBold.name
                font.pixelSize: SizeManager.getSizeBasedOnAppWidth(18)
                height: parent.height
            }
        }
    }

    /// Profil clickable item
    RecButton {
        id: profil_button
        buttonColors.pressedColor: ColorsManager.grey300
        buttonColors.hoveredColor: ColorsManager.grey200
        height: top_bar_background.height
        width: full_name_text.contentWidth + full_name_text.anchors.rightMargin
               + user_logo.width + user_logo.anchors.rightMargin * 2

        visible: profilItemDisplayed
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        onClicked: profil_menu.open()

        Text {
            id: full_name_text
            color: ColorsManager.grey50
            font.family: FontsManager.latoSemiBold.name
            font.pixelSize: SizeManager.getSizeBasedOnAppWidth(18)
            font.italic: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignRight
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: user_logo.left
            anchors.rightMargin: SizeManager.getSizeBasedOnAppWidth(7)
        }

        Image {
            id: user_logo
            source: "qrc:/images/user_icon.svg"
            sourceSize: Qt.size(SizeManager.getSizeBasedOnAppWidth(21),
                                SizeManager.getSizeBasedOnAppWidth(21))
            fillMode: Image.PreserveAspectFit
            antialiasing: true
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: SizeManager.getSizeBasedOnAppWidth(8)
        }

        ColorOverlay {
            anchors.fill: user_logo
            source: user_logo
            color: "#FFFFFF"
        }
    }

    /// Menu displayed when clicking on profil item
    TopBarProfilMenu {
        id: profil_menu
        width: SizeManager.getSizeBasedOnAppWidth(200)
        y: profil_button.height
        x: top_bar_background.width - width
        Component.onCompleted: topBarBloc.retrieveCurrentUserFullName()
        onLogOutClicked: top_bar_bloc.onLogoutClicked()
    }

    /// Menu displayed when clicking on file item
    TopBarFileMenu {
        id: file_menu
        width: SizeManager.getSizeBasedOnAppWidth(200)
        y: top_bar_background.height
        x: left_button.x
    }

    CreditsPopUp {
        id: credits_pop_up
        anchors.centerIn: Overlay.overlay
    }

    Connections {
        target: top_bar_bloc
        function onCurrentUserFullNameRetrieved(currentUserFullName) {
            full_name_text.text = currentUserFullName
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.75;height:40;width:1280}
}
##^##*/

