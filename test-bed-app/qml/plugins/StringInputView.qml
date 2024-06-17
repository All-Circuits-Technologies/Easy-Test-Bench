// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.13
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.5

import app.managers 1.0
import app.blocs 1.0

import "qrc:/basic_items"

//Manage string input view
APluginInputView {
    id: string_input_view

    aInputBloc: StringInputBloc {
        id: string_input_bloc
        onBlocInitialized: function() {
            text_field.takeFocus()
        }
    }

    columnContent: ColumnLayout {
        id: input_content

        anchors.fill: input_content.parent

        spacing: string_input_view.spacingBetweenElems

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true

            FormTextField {
                id: text_field

                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0

                fieldModel.maxLength: string_input_bloc.castedModel.maxLength
                fieldModel.minLength: string_input_bloc.castedModel.minLength
                fieldModel.regex: string_input_bloc.castedModel.regex

                text: string_input_bloc.inputModel.valueSet

                onEnterPressed: function() {
                    if(text_field.isValid) {
                        accept_button.click()
                    } else if(string_input_bloc.castedModel.clearInputIfError) {
                        // In that case we clear the input
                        text_field.text = ""
                    }
                }

                onActiveFocusChanged: function(focusb) {
                    // When a plugin is removed from the view, all the plugins are reloaded
                    // in the parent loader, but QML doesn't recreates all the items (it caches
                    // them).
                    // When this is happening,the current plugin (and its buttons) loses the
                    // focus. Here we look if when loosing the focus on a button, the parent
                    // item gets the focus of its children, if it does, we force focus
                    if(!focusb && string_input_view.parentLoader.focus) {
                        text_field.takeFocus();
                    }
                }
            }
        }

        MainButton {
            id: accept_button

            text: qsTr("(TR) Accept")
            enabled: text_field.isValid
            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true
            keyboardShortcut: StandardKey.InsertParagraphSeparator
            displayShorcut: false
            onClicked: {
                if (text_field.isValid) {
                    string_input_bloc.onAcceptClicked(text_field.text)
                }
            }

            onActiveFocusChanged: function(focusb) {
                // When a plugin is removed from the view, all the plugins are reloaded
                // in the parent loader, but QML doesn't recreates all the items (it caches
                // them).
                // When this is happening,the current plugin (and its buttons) loses the
                // focus. Here we look if when loosing the focus on a button, the parent
                // item gets the focus of its children, if it does, we force focus
                if(!focusb && string_input_view.parentLoader.focus) {
                    accept_button.forceActiveFocus();
                }
            }
        }
    }

    Component.onCompleted: {
        text_field.takeFocus()
    }
}
