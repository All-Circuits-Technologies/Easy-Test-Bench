// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.0
import QtQuick.Controls 2.15

/// Top bar menu associated to user profil
Menu {
    signal logOutClicked

    PopUpMenuItem {
        contentItem: RightMenuItemText {
            text: qsTr("(TR) My profil")
            enabled: false
        }
        enabled: false
    }

    PopUpMenuItem {
        onTriggered: logOutClicked()
        contentItem: RightMenuItemText {
            text: qsTr("(TR) Log out")
        }
    }
}
