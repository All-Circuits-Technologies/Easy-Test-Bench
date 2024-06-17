// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.13
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import app.managers 1.0
import app.blocs 1.0

import "qrc:/basic_items"

/// Manage radio list buttons input view
APluginInputView {
    id: radio_list_buttons_input_view

    aInputBloc: RadioListButtonsInputBloc {
        id: radio_list_buttons_input_bloc
    }

    columnContent: Item {
        id: grid_view_container

        anchors.fill: grid_view_container.parent

        GridLayout {
            id: grid_view
            columns: 2
            columnSpacing: SizeManager.getSizeBasedOnAppWidth(3)
            rowSpacing: SizeManager.getSizeBasedOnAppHeight(3)

            anchors.bottom: grid_view_container.bottom
            anchors.left: grid_view_container.left
            anchors.right: grid_view_container.right

            Repeater {
                model: radio_list_buttons_input_bloc.castedModel.buttons

                delegate: CustomMainButton {
                    id: button_item

                    required property int index
                    required property string key
                    required property string label
                    required property color fontColor
                    required property color backgroundColor
                    required property string keyShortcut

                    readonly property bool displayFull: needToDisplayedFull()
                    readonly property bool isDefault:
                        (radio_list_buttons_input_bloc.castedModel.defaultValue === key)

                    onActiveFocusChanged: function(focusb) {
                        // When a plugin is removed from the view, all the plugins are reloaded
                        // in the parent loader, but QML doesn't recreates all the items (it caches
                        // them).
                        // When this is happening,the current plugin (and its buttons) loses the
                        // focus. Here we look if when loosing the focus on a button, the parent
                        // item gets the focus of its children, if it does, we force focus
                        if(!focusb && radio_list_buttons_input_view.parentLoader.focus) {
                            button_item.forceActiveFocus();
                        }
                    }

                    enabledColor: backgroundColor
                    textColor: fontColor
                    text: label
                    keyboardShortcut: keyShortcut
                    displayTimer: radio_list_buttons_input_bloc.castedModel.timerEnabled &&
                                    isDefault

                    timeoutInS: button_item.displayTimer ?
                                    radio_list_buttons_input_bloc.castedModel.timeoutLeftInS : 0

                    Layout.columnSpan: button_item.displayFull ? 2 : 1

                    Layout.preferredWidth: button_item.displayFull ?
                                       grid_view.width :
                                       ((grid_view.width - grid_view.columnSpacing)/2)

                    onClicked: radio_list_buttons_input_bloc.onBtnSelected(key)

                    function needToDisplayedFull() {
                        let length = radio_list_buttons_input_bloc.castedModel.buttonsList.length;

                        if((length % 2) === 0) {
                            // In that case the number of elements is even, nothing has to be done
                            return false;
                        }

                        return button_item.index === (length - 1);
                    }

                    Connections {
                        enabled: isDefault
                        target: radio_list_buttons_input_bloc
                        function onBlocInitialized() {
                            button_item.forceActiveFocus();
                        }
                    }

                    Component.onCompleted: {
                        if(isDefault) {
                            button_item.forceActiveFocus();
                        }
                    }
                }
            }
        }
    }
}
