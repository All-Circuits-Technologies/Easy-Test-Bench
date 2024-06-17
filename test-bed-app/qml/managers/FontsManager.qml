// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

pragma Singleton

import QtQuick 2.13

/// Class which allows to use external/internal fonts in the application
Item {
    readonly property FontLoader latoRegular: FontLoader {
        id: lato_regular
        source: "qrc:/fonts/Lato-Regular.ttf"
    }

    readonly property FontLoader latoSemiBold: FontLoader {
        id: lato_semi_bold
        source: "qrc:/fonts/Lato-SemiBold.ttf"
    }
}
