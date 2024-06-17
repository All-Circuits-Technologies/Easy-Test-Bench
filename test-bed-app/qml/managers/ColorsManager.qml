// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

pragma Singleton

import QtQuick 2.13

/// Class which lists the colors of the application
QtObject {
    readonly property color black: "#000000"
    readonly property color white: "#FFFFFF"

    readonly property color grey50: "#F8F8F8"
    readonly property color grey60: "#E4E4E4"
    readonly property color grey80: "#D0D0D0"
    readonly property color grey100: "#BCBCBC"
    readonly property color grey200: "#A8A8A8"
    readonly property color grey300: "#949494"
    readonly property color grey400: "#808080"
    readonly property color grey500: "#6C6C6C"
    readonly property color grey600: "#585858"
    readonly property color grey700: "#444444"
    readonly property color grey800: "#303030"
    readonly property color grey900: "#1C1C1C"

    readonly property color orange50: "#FFCC85"
    readonly property color orange100: "#FFBB5C"
    readonly property color orange200: "#FFAA33"
    readonly property color orange300: "#FF990A"
    readonly property color orange400: "#D37A00"
    readonly property color orange500: "#B86B00"
    readonly property color orange600: "#8F5300"
    readonly property color orange700: "#663C00"
    readonly property color orange800: "#3D2400"
    readonly property color orange900: "#140C00"

    readonly property color red50: "#FFEBEE"
    readonly property color red100: "#FFCDD2"
    readonly property color red200: "#EF9A9A"
    readonly property color red300: "#E57373"
    readonly property color red400: "#EF5350"
    readonly property color red500: "#F44336"
    readonly property color red600: "#E53935"
    readonly property color red700: "#D32F2F"
    readonly property color red800: "#C62828"
    readonly property color red900: "#B71C1C"

    readonly property color teal50: "#E0F2F1"
    readonly property color teal100: "#B2DFDB"
    readonly property color teal200: "#80CBC4"
    readonly property color teal300: "#4DB6AC"
    readonly property color teal400: "#26A69A"
    readonly property color teal500: "#009688"
    readonly property color teal600: "#00897B"
    readonly property color teal700: "#00796B"
    readonly property color teal800: "#00695C"
    readonly property color teal900: "#004D40"

    function getColorWithAlpha(color, alpha) {
        return Qt.hsla(color.hslHue, color.hslSaturation,
                       color.hslLightness, alpha)
    }
}
