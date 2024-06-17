// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.0
import QtQuick.Controls 2.5

import app.blocs 1.0

import "qrc:/basic_items"

/// Base class for rectangular buttons
Button {
    id: base_button

    readonly property RecButtonBloc recButtonBloc: RecButtonBloc {};

    property alias backgroundRadius: button_background.radius
    property alias buttonColors: button_background.buttonColors

    highlighted: true
    activeFocusOnTab: true

    Keys.onReturnPressed: recButtonBloc.click(base_button)
    Keys.onEnterPressed: recButtonBloc.click(base_button)

    background: ButtonBackground {
        id: button_background
    }

    ClickableItem {}
}
