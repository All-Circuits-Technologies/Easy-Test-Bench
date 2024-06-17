// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphaël Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5

import app.blocs 1.0
import app.managers 1.0
import app.types 1.0

import "qrc:/logs"
import "qrc:/sequence"
import "qrc:/plugins"
import "qrc:/pop_up"

/// Sequence execution and log viewing page
ABasePage {
    readonly property SequenceRunBloc sequenceRunBloc: SequenceRunBloc {
        id: sequence_run_bloc
    }

    topBar.title: sequence_run_bloc.projectName
    topBar.profilItemDisplayed: true
    topBar.leftButtonText: qsTr("(TR) File")
    topBar.fileMenu.menuModel: sequence_run_bloc.fileMenuModel
    topBar.fileMenu.onItemClicked: sequence_run_bloc.onMenuItemClicked(index)

    ScrollableTextPopUp {
        id: loading_error_pop_up
        title: qsTr("(TR) Can't launch the sequence")
        popUpModel: sequence_run_bloc.popUpModel
        anchors.centerIn: parent
    }

    ColumnLayout {
        anchors.top: topBar.bottom
        anchors.topMargin: SizeManager.getSizeBasedOnAppHeight(20)
        anchors.left: parent.left
        anchors.leftMargin: SizeManager.getSizeBasedOnAppWidth(29)
        anchors.right: parent.right
        anchors.rightMargin: SizeManager.getSizeBasedOnAppWidth(29)
        anchors.bottom: parent.bottom
        anchors.bottomMargin: SizeManager.getSizeBasedOnAppHeight(20)
        spacing: SizeManager.getSizeBasedOnAppWidth(6)

        TabBar {
            id: bar
            width: parent.width
            height: SizeManager.getSizeBasedOnAppWidth(25)
            spacing: SizeManager.getSizeBasedOnAppWidth(6)
            background: Rectangle {
                color: "transparent"
            }

            SequenceTabButton {
                label.text: qsTr("(TR) Logs")
            }

            SequenceTabButton {
                id: plugins_tab_button
                label.text: qsTr("(TR) Plugins")
            }
        }

        StackLayout {
            id: stack_layout

            Layout.fillHeight: true
            currentIndex: bar.currentIndex

            LogView {
                id: log_view
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: sequence_run_bloc.logViewModel
            }

            SequencePluginsView {
                id: sequence_plugins_view
                inputHeight: stack_layout.height
                Layout.alignment: Qt.AlignLeft
                Layout.fillWidth: true
                Layout.fillHeight: true
                logsModel: sequence_run_bloc.logViewModel
                onAskVisibility: function (visibility) {
                    bar.currentIndex = visibility ? 1 : 0
                }
                onPluginsCountChanged: manageCount(pluginsCount)
                sequenceState: sequence_run_bloc.sequenceState

                Component.onCompleted: {
                    bar.currentIndex = 0
                    manageCount(sequence_plugins_view.pluginsCount)
                }

                function manageCount(count) {
                    if (count === 0) {
                        plugins_tab_button.enabled = false
                    } else if (!plugins_tab_button.enabled) {
                        plugins_tab_button.enabled = true
                    }
                }
            }
        }

        Item {
            Layout.minimumHeight: stop_button.height
            Layout.fillWidth: true
            Layout.topMargin: SizeManager.getSizeBasedOnAppHeight(20)

            SequenceButton {
                id: play_button
                label: qsTr("(TR) Start")
                button.icon.source: "qrc:/images/start.svg"
                anchors.centerIn: parent
                button.onClicked: {
                    sequence_run_bloc.onPlayButtonClicked()
                }

                state: sequenceRunBloc.sequenceState
                states: [
                    State {
                        name: SequenceStateType.Running
                        PropertyChanges {
                            target: play_button
                            button.icon.source: "qrc:/images/pause.svg"
                            label: qsTr("(TR) Pause")
                        }
                        PropertyChanges {
                            target: topBar
                            secondaryTitle: qsTr("(TR) Running")
                        }
                        PropertyChanges {
                            target: stop_button
                            button.enabled: true
                        }
                    },
                    State {
                        name: SequenceStateType.InPause
                        PropertyChanges {
                            target: play_button
                            button.icon.source: "qrc:/images/resume.svg"
                            label: qsTr("(TR) Resume")
                        }
                        PropertyChanges {
                            target: topBar
                            secondaryTitle: qsTr("(TR) Pause")
                        }
                        PropertyChanges {
                            target: stop_button
                            button.enabled: true
                        }
                    },
                    State {
                        name: SequenceStateType.Stopped
                        PropertyChanges {
                            target: play_button
                            button.icon.source: "qrc:/images/start.svg"
                            label: qsTr("(TR) Start")
                        }
                        PropertyChanges {
                            target: topBar
                            secondaryTitle: qsTr("(TR) Stop")
                        }
                        PropertyChanges {
                            target: stop_button
                            button.enabled: false
                        }
                    },
                    State {
                        name: SequenceStateType.Unknown
                        PropertyChanges {
                            target: play_button
                            button.icon.source: "qrc:/images/start.svg"
                            label: qsTr("(TR) Start")
                            button.enabled: false
                        }
                        PropertyChanges {
                            target: topBar
                            secondaryTitle: qsTr("(TR) Loading")
                        }
                        PropertyChanges {
                            target: stop_button
                            button.enabled: false
                        }
                    }
                ]
            }

            SequenceButton {
                id: stop_button
                label: qsTr("(TR) Stop")
                button.icon.source: "qrc:/images/stop.svg"
                anchors.top: play_button.top
                anchors.right: sequences_number_display.left
                anchors.left: play_button.right
                button.onClicked: {
                    sequence_run_bloc.onStopButtonClicked()
                }
            }

            SequenceDisplay {
                id: sequences_number_display
                label: qsTr("(TR) Sequences performed")
                textDisplayed: sequence_run_bloc.sequencesPerformed
                anchors.top: stop_button.top
                anchors.right: parent.right
            }
        }
    }

    Connections {
        target: sequence_run_bloc.popUpModel
        function onDisplayPopUp(errorMessage) {
            loading_error_pop_up.scrollableText.contentText.text = errorMessage
            loading_error_pop_up.open()
        }
    }
}
