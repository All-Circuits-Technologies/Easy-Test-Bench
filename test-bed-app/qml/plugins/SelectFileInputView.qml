// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.13
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3

import app.managers 1.0
import app.blocs 1.0

import "qrc:/basic_items"

/// Manage select file input view
APluginInputView {
    id: select_file_input_view

    aInputBloc: SelectFileInputBloc {
        id: select_file_input_bloc
        onBlocInitialized: function() {
            browse_button.forceActiveFocus()
        }
    }

    columnContent: ColumnLayout {
        id: input_content

        anchors.fill: input_content.parent

        spacing: select_file_input_view.spacingBetweenElems

        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true

            FormTextField {
                id: text_field

                readOnly: true
                text: select_file_input_bloc.inputModel.valueSet
            }

            MainButton {
                id: browse_button
                text: qsTr("(TR) Browse")
                Layout.preferredWidth: SizeManager.getSizeBasedOnAppWidth(100)
                onClicked: {
                    file_dialog.open()
                }

                onActiveFocusChanged: function(focusb) {
                    // When a plugin is removed from the view, all the plugins are reloaded
                    // in the parent loader, but QML doesn't recreates all the items (it caches
                    // them).
                    // When this is happening,the current plugin (and its buttons) loses the
                    // focus. Here we look if when loosing the focus on a button, the parent
                    // item gets the focus of its children, if it does, we force focus
                    if(!focusb && select_file_input_view.parentLoader.focus) {
                        browse_button.forceActiveFocus();
                    }
                }
            }
        }

        Component {
            id: component_text_area

            Text {
                text: select_file_input_bloc.errorTxt
                wrapMode: Text.Wrap
                elide: Text.ElideNone
                color: ColorsManager.red400
                font.family: FontsManager.latoRegular.name
                font.bold: true
                font.pixelSize: SizeManager.getSizeBasedOnAppWidth(18)
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignTop
            }
        }

        Component {
            id: blank
            Item {}
        }

        Loader {
            id: error_text_area
            Layout.fillWidth: true
            sourceComponent: (select_file_input_bloc.errorTxt.length > 0) ? component_text_area : blank
        }

        MainButton {
            id: validate_btn
            text: qsTr("(TR) Validate")
            enabled: select_file_input_bloc.validBtnEnabled
            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true
            onClicked: select_file_input_bloc.inputModel.finishView()

            onActiveFocusChanged: function(focusb) {
                // When a plugin is removed from the view, all the plugins are reloaded
                // in the parent loader, but QML doesn't recreates all the items (it caches
                // them).
                // When this is happening,the current plugin (and its buttons) loses the
                // focus. Here we look if when loosing the focus on a button, the parent
                // item gets the focus of its children, if it does, we force focus
                if(!focusb && select_file_input_view.parentLoader.focus) {
                    validate_btn.forceActiveFocus();
                }
            }
        }
    }

    FileDialog {
        id: file_dialog
        title: qsTr("(TR) Select a file")
        folder: select_file_input_bloc.folderPath
        nameFilters: select_file_input_bloc.castedModel.filesExtensions
        onAccepted: select_file_input_bloc.onFileSelected(file_dialog.fileUrl)
    }
}
