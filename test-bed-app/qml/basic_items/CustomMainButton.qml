// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.15
import QtGraphicalEffects 1.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "qrc:/basic_items"

import app.managers 1.0

/// Custom Main button
RecButton {
    id: rec_button

    property color enabledColor
    property color textColor
    property string keyboardShortcut
    property bool displayShorcut: true

    property bool displayTimer: false
    property int timeoutInS

    buttonColors.enabledColor: rec_button.enabledColor
    buttonColors.pressedColor: Qt.lighter(rec_button.enabledColor, 1.4)
    buttonColors.hoveredColor: Qt.lighter(rec_button.enabledColor, 1.2)
    buttonColors.disabledColor: ColorsManager.grey600
    backgroundRadius: SizeManager.getSizeBasedOnAppWidth(7)
    implicitHeight: SizeManager.getSizeBasedOnAppWidth(48)

    ToolTip.visible: hovered
    ToolTip.text: rec_button.text
    ToolTip.delay: 1000

    Component {
        id: blank
        Item {
            width: 0
            height: 0
        }
    }

    contentItem: Item {
        id: text_row

        anchors.left: parent.left
        anchors.leftMargin: SizeManager.getSizeBasedOnAppWidth(3)
        anchors.right: parent.right
        anchors.rightMargin: SizeManager.getSizeBasedOnAppWidth(3)
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        RowLayout {
            id: button_text_layout

            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter

            Text {
                id: button_text
                text: rec_button.text
                color: rec_button.enabled ? rec_button.textColor : ColorsManager.grey800

                elide: Text.ElideRight
                wrapMode: Text.Wrap
                maximumLineCount: 1

                Layout.maximumWidth: text_row.width - loader_key_shortcut.width

                font.family: FontsManager.latoSemiBold.name
                font.pixelSize: SizeManager.getSizeBasedOnAppWidth(18)
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            Component {
                id: button_shortcut_text
                Text {
                    id: button_text
                    text: displayShorcut ? "(%1)".arg(rec_button.keyboardShortcut) : ""
                    color: rec_button.enabled ? rec_button.textColor : ColorsManager.grey800


                    font.family: FontsManager.latoRegular.name
                    font.pixelSize: SizeManager.getSizeBasedOnAppWidth(16)
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                    Shortcut {
                        id: button_shortcut
                        sequence: rec_button.keyboardShortcut
                        onActivated: recButtonBloc.click(rec_button)
                    }
                }
            }

            Loader {
                id: loader_key_shortcut

                sourceComponent: (rec_button.keyboardShortcut.length > 0) ?
                                     button_shortcut_text : blank
            }
        }

        Component {
            id: button_timeout_text
            Text {
                id: button_text
                text: rec_button.timeoutInS
                color: rec_button.enabled ? rec_button.textColor : ColorsManager.grey800


                font.family: FontsManager.latoRegular.name
                font.pixelSize: SizeManager.getSizeBasedOnAppWidth(10)
                font.italic: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        Loader {
            id: loader_timeout

            anchors.bottom: parent.bottom
            anchors.bottomMargin: SizeManager.getSizeBasedOnAppWidth(2)
            anchors.horizontalCenter: parent.horizontalCenter

            sourceComponent: rec_button.displayTimer ? button_timeout_text : blank
        }
    }

    function click() {
        recButtonBloc.click(rec_button)
    }
}
