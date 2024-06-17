// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.0
import QtQuick.Controls 2.5

/// Area that allows to change the mouse icon when the button is hovered
MouseArea {
    hoverEnabled: true
    anchors.fill: parent

    /// Prevent this area to steal mouse events from parents classes
    propagateComposedEvents : true
    onClicked: mouse.accepted = false;
    onPressed: mouse.accepted = false;
    onReleased: mouse.accepted = false;

    cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
}
