// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.15
import QtQuick.Controls 2.15

import app.managers 1.0
import app.types 1.0

/// Text format of the log content
Rectangle {
    id: log_text

    property alias textContent: log_text_content

    property string defaultBgColor: ColorsManager.grey200
    property string toolTipText
    required property int logCriticity
    property bool alternateColor: false

    Text {
        id: log_text_content

        width: log_text.width

        color: ColorsManager.grey800
        elide: Text.ElideRight
        font.family: FontsManager.latoRegular.name
        font.pixelSize: SizeManager.getSizeBasedOnAppWidth(18)

        ToolTip.visible: loader_text_area.toolTipDisplayed
        ToolTip.text: log_text.toolTipText

        Component {
            id: component_text_area

            MouseArea {
                id: text_area
                anchors.fill: parent
                hoverEnabled: true

                onContainsMouseChanged: loader_text_area.toolTipDisplayed = containsMouse
            }
        }

        Component {
            id: blank
            Item {}
        }

        Loader {
            id: loader_text_area

            property bool toolTipDisplayed: false

            anchors.fill: parent
            sourceComponent: (log_text.toolTipText.length > 0) ? component_text_area : blank
        }
    }

    state: logCriticity

    states: [
        State {
            name: LogMsgType.Debug
            PropertyChanges {
                target: log_text_content
                color: ColorsManager.teal800
            }
            PropertyChanges {
                target: log_text
                color: manageBgColor(defaultBgColor)
            }
        },
        State {
            name: LogMsgType.Info
            PropertyChanges {
                target: log_text_content
                color: ColorsManager.grey800
            }
            PropertyChanges {
                target: log_text
                color: manageBgColor(defaultBgColor)
            }
        },
        State {
            name: LogMsgType.Warning
            PropertyChanges {
                target: log_text_content
                color: ColorsManager.orange100
            }
            PropertyChanges {
                target: log_text
                color: manageBgColor(ColorsManager.grey500)
            }
        },
        State {
            name: LogMsgType.Critical
            PropertyChanges {
                target: log_text_content
                color: ColorsManager.red200
            }
            PropertyChanges {
                target: log_text
                color: manageBgColor(ColorsManager.grey600)
            }
        },
        State {
            name: LogMsgType.Fatal
            PropertyChanges {
                target: log_text_content
                color: ColorsManager.red50
            }
            PropertyChanges {
                target: log_text
                color: manageBgColor(ColorsManager.grey700)
            }
        }
    ]

    function manageBgColor(colorStr) {
        if (log_text.alternateColor) {
            return Qt.darker(colorStr, 1.07)
        }

        return colorStr
    }
}
