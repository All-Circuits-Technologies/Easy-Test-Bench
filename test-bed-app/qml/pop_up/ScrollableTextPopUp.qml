// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphaël Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

import app.managers 1.0
import app.models 1.0

import "qrc:/basic_items"

/// Pop up with a title and a vertical scrollable text
ShadedPopUp {
    property string title

    required property ScrollablePopUpModel popUpModel

    property alias scrollableText: scrollable_text

    id: scrollable_text_pop_up
    height: SizeManager.getSizeBasedOnAppWidth(304)
    width: SizeManager.getSizeBasedOnAppWidth(600)
    background: Rectangle {
        id: pop_up_background
        color: ColorsManager.grey200
    }

    ColumnLayout {
        spacing: 0

        /// title
        Text {
            id: title_text
            text: title
            color: ColorsManager.white
            font.family: FontsManager.latoSemiBold.name
            font.pixelSize: SizeManager.getSizeBasedOnAppWidth(22)
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            Layout.preferredHeight: SizeManager.getSizeBasedOnAppWidth(40)
            Layout.preferredWidth: scrollable_text_pop_up.width
        }

        /// Core text area
        Item {
            height: SizeManager.getSizeBasedOnAppWidth(200)
            Layout.fillWidth: true
            Layout.leftMargin: SizeManager.getSizeBasedOnAppWidth(15)
            Layout.rightMargin: SizeManager.getSizeBasedOnAppWidth(15)

            VerticalScrollableText {
                id: scrollable_text
                color: ColorsManager.grey80
                height: parent.height
                width: parent.width
                contentText.color: ColorsManager.grey800
                contentText.font.family: FontsManager.latoRegular.name
                contentText.font.pixelSize: SizeManager.getSizeBasedOnAppWidth(
                                                19)
                contentText.textFormat: Text.RichText
            }
        }

        RowLayout {
            spacing: SizeManager.getSizeBasedOnAppWidth(15)
            Layout.preferredWidth: SizeManager.getSizeBasedOnAppWidth(270)
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.topMargin: SizeManager.getSizeBasedOnAppWidth(10)
            Layout.leftMargin: SizeManager.getSizeBasedOnAppWidth(15)
            Layout.rightMargin: SizeManager.getSizeBasedOnAppWidth(15)

            Repeater {
                model: popUpModel.neededBtns

                MainButton {
                    text: popUpModel.getBtnLabel(modelData)
                    Layout.maximumWidth: scrollable_text_pop_up.width / 2
                    Layout.fillWidth: parent
                    isPrimaryColor: popUpModel.isPrimary(modelData)

                    onClicked: {
                        scrollable_text_pop_up.close()
                        popUpModel.fireBtnClick(modelData)
                    }
                }
            }
        }
    }
}
