// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.0
import QtQuick.Controls 2.15

import app.managers 1.0

/// Useful class to manage the button background colors depending of its state
Rectangle {
    id: button_background

    property ButtonColors buttonColors: ButtonColors {}

    border.color: Qt.darker(button_background.color, 1.5)
    border.width: parent.activeFocus ? SizeManager.getSizeBasedOnAppWidth(2) : 0

    QtObject {
        id: priv_button_background

        readonly property string enabledState: "enabled"
        readonly property string disabledState: "disabled"
        readonly property string hoverState: "hover"
        readonly property string focusedState: "focused"
        readonly property string checkedState: "checked"
        readonly property string pressedState: "pressed"

        property bool ready: false

        function calculateState(button, buttonBackground) {
            if(priv_button_background === null ||
                    priv_button_background.ready !== true) {
                // Nothing to do
                return;
            }

            var tmpState = enabledState

            if (!button.enabled) {
                tmpState = disabledState
            } else if (button.checkable && button.checked) {
                tmpState = checkedState
            } else if (button.down) {
                tmpState = pressedState
            } else if (button.hovered) {
                tmpState = hoverState
            }

            if(button_background.state === tmpState) {
                // Nothing to do
                return;
            }

            buttonBackground.state = tmpState
        }
    }

    state: priv_button_background.enabledState

    states: [
        State {
            name: priv_button_background.enabledState
            PropertyChanges {
                target: button_background
                color: buttonColors.enabledColor
            }
        },
        State {
            name: priv_button_background.disabledState
            PropertyChanges {
                target: button_background
                color: buttonColors.disabledColor
            }
        },
        State {
            name: priv_button_background.hoverState
            PropertyChanges {
                target: button_background
                color: buttonColors.hoveredColor
            }
        },
        State {
            name: priv_button_background.pressedState
            PropertyChanges {
                target: button_background
                color: buttonColors.pressedColor
            }
        },
        State {
            name: priv_button_background.checkedState
            PropertyChanges {
                target: button_background
                color: buttonColors.checkedColor
            }
        }
    ]

    Component.onCompleted: {
        if (parent.enabled !== undefined && parent.checked !== undefined
                && parent.down !== undefined && parent.hovered !== undefined
                && parent.focus !== undefined) {
            parent.enabledChanged.connect(
                        priv_button_background.calculateState.bind(
                            this, parent, button_background))
            parent.checkedChanged.connect(
                        priv_button_background.calculateState.bind(
                            this, parent, button_background))
            parent.downChanged.connect(
                        priv_button_background.calculateState.bind(
                            this, parent, button_background))
            parent.hoveredChanged.connect(
                        priv_button_background.calculateState.bind(
                            this, parent, button_background))
            priv_button_background.ready = true
            priv_button_background.calculateState(parent, button_background)
        }
    }

    Component.onDestruction: {
        if (parent.enabled !== undefined && parent.checked !== undefined
                && parent.down !== undefined && parent.hovered !== undefined
                && parent.focus !== undefined) {
            parent.enabledChanged.disconnect(
                        priv_button_background.calculateState.bind(
                            this, parent, button_background))
            parent.checkedChanged.disconnect(
                        priv_button_background.calculateState.bind(
                            this, parent, button_background))
            parent.downChanged.disconnect(
                        priv_button_background.calculateState.bind(
                            this, parent, button_background))
            parent.hoveredChanged.disconnect(
                        priv_button_background.calculateState.bind(
                            this, parent, button_background))
            priv_button_background.ready = false
        }
    }
}
