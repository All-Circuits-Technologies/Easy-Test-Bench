// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import app.managers 1.0
import app.models 1.0

ColumnLayout {
    id: form_text_field

    readonly property FormTextFieldModel fieldModel: FormTextFieldModel {
        id: field_model
        text: text_field.text
    }

    property alias backgroundColor: text_field_background.color
    property alias placeholderTextColor: text_field.placeholderTextColor
    property alias color: text_field.color

    property alias text: text_field.text
    property alias isValid: field_model.isValid
    property alias readOnly: text_field.readOnly

    // Emitted when the enter button is pressed in the form
    signal enterPressed()

    TextField {
        id: text_field

        activeFocusOnTab: true

        leftPadding: SizeManager.getSizeBasedOnAppWidth(8)
        rightPadding: text_field.leftPadding
        topPadding: SizeManager.getSizeBasedOnAppHeight(4)
        bottomPadding: text_field.topPadding
        selectByMouse: true

        Layout.fillWidth: true

        placeholderTextColor: ColorsManager.grey800
        color: ColorsManager.grey900
        maximumLength: fieldModel.maxLength

        font.family: FontsManager.latoRegular.name
        font.pixelSize: SizeManager.getSizeBasedOnAppWidth(22)
        background: Rectangle {
            id: text_field_background
            color: isValid ? ColorsManager.grey80 : ColorsManager.red50
            border.color: isValid ? "transparent" : ColorsManager.red400
            border.width: isValid ? 0 : SizeManager.getSizeBasedOnAppWidth(2)
        }

        Keys.onEnterPressed: form_text_field.enterPressed()
        Keys.onReturnPressed: form_text_field.enterPressed()
    }

    ListView {
        id: errors_list
        Layout.fillWidth: true
        Layout.preferredHeight: contentHeight

        model: fieldModel.errors

        delegate: Text {
            required property string display

            text: display
            height: contentHeight
            width: errors_list.width
            wrapMode: Text.Wrap
            elide: Text.ElideNone
            color: ColorsManager.red400
            font.family: FontsManager.latoRegular.name
            font.pixelSize: SizeManager.getSizeBasedOnAppWidth(14)
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
    }

    function takeFocus() {
        text_field.forceActiveFocus();
    }
}
