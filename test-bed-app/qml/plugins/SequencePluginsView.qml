// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5

import app.blocs 1.0
import app.managers 1.0
import app.models 1.0
import app.types 1.0

import "qrc:/basic_items"
import "qrc:/logs"

/// Screen that displays plugin inputs
RowLayout {
    id: sequence_plugin_view

    property alias sequenceState: sequence_plugins_bloc.sequenceState

    readonly property SequencePluginsBloc sequencePluginsBloc: SequencePluginsBloc {
        id: sequence_plugins_bloc

        onInputModelsListChanged: function (prevNb, newNb) {
            sequence_plugin_view.pluginsCount = newNb
        }

        onAskVisibilityWithThreshold: function(askedVisibility) {
            askVisibility(askedVisibility)
        }
    }

    readonly property int maxPluginItemWidth: SizeManager.getSizeBasedOnAppWidth(450)
    readonly property int minPluginItemWidth: SizeManager.getSizeBasedOnAppWidth(300)

    property alias logsModel: log_view.model
    property int inputHeight

    property int pluginsCount

    signal askVisibility(bool visibility)

    GridView {
        id: grid_view

        readonly property int spacing: SizeManager.getSizeBasedOnAppWidth(6)

        clip: true
        ScrollBar.vertical: scroll_bar
        contentHeight: height
        contentWidth: width
        Layout.fillHeight: true
        Layout.fillWidth: true
        cellHeight: height
        cellWidth: calculateCellWidth(width)
        snapMode: GridView.SnapToRow

        model: sequence_plugins_bloc.inputGenModels

        highlightMoveDuration: 100
        delegate: Item {
            id: parent_element_loader
            required property int index

            width: grid_view.cellWidth
            height: grid_view.cellHeight

            Loader {
                id: element_loader

                height: parent_element_loader.height
                width: grid_view.cellWidth - grid_view.spacing

                anchors.top: parent_element_loader.top
                anchors.left: parent_element_loader.left

                Component.onCompleted: {
                    let inputModel = sequence_plugins_bloc.inputModels[parent_element_loader.index]
                    let url = ""

                    switch (inputModel.widgetType) {
                    case PluginWidgetType.String:
                        url = "StringInputView.qml"
                        break
                    case PluginWidgetType.RadioListButtons:
                        url = "RadioListButtonsInputView.qml"
                        break
                    case PluginWidgetType.SelectFile:
                        url = "SelectFileInputView.qml"
                        break
                    default:
                        console.warn("The type of plugin widget isn't managed")
                        break
                    }

                    if (url.length > 0) {
                        element_loader.setSource(url, { "inputModel": inputModel,
                                                     "parentLoader": parent_element_loader })
                    }
                }
            }
        }

        function compareBestWidth(widthToTest) {
            if (widthToTest < sequence_plugin_view.minPluginItemWidth) {
                return -1
            }

            if (widthToTest > sequence_plugin_view.maxPluginItemWidth) {
                return 1
            }

            return 0
        }

        function calculateCellWidth(parentWidth) {
            var elemByLine = 1
            var bestWidth = parentWidth
            var result = -1
            while (result != 0) {
                var calculatedWidth = parentWidth / elemByLine

                result = compareBestWidth(calculatedWidth)

                if (result < 0) {
                    break
                } else if (result > 0) {
                    elemByLine++
                }

                bestWidth = calculatedWidth
            }

            return bestWidth
        }
    }

    VerticalScrollBar {
        id: scroll_bar
        flickable: grid_view
        buttonsHeight: SizeManager.getSizeBasedOnAppWidth(30)
        Layout.fillHeight: true
        Layout.preferredWidth: SizeManager.getSizeBasedOnAppWidth(30)
        Layout.rightMargin: SizeManager.getSizeBasedOnAppWidth(6)
        snapMode: ScrollBar.SnapAlways
    }

    CompactLogView {
        id: log_view
        Layout.alignment: Qt.AlignLeft
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.minimumWidth: SizeManager.getSizeBasedOnAppWidth(150)
        Layout.maximumWidth: SizeManager.getSizeBasedOnAppWidth(350)
    }
}
