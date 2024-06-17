// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.0
import QtQuick.Controls 2.5

/// Base class for circular buttons
RoundButton {
    id: round_button
    property alias imgSource: image.source

    contentItem: Image {
        id: image
        sourceSize.height: round_button.width
        sourceSize.width: round_button.width
        fillMode: Image.PreserveAspectFit
        anchors.centerIn: parent
    }

    ClickableItem {}
}
