// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3

import app.blocs 1.0
import app.managers 1.0

import "qrc:/basic_items"
import "qrc:/file_selection"
import "qrc:/menus"
import "qrc:/pop_up"

/// Base page dedicated to open projects
ABasePage {
    required property AFileSelectionBloc fileSelectionBloc

    property alias cancelButtonVisible: cancel_button.visible
    property alias fileListTitle: file_list_view.title

    id: base_page
    topBar.title: qsTr("(TR) Open a project")
    topBar.leftButtonText: qsTr("(TR) File")
    topBar.fileMenu.menuModel: fileSelectionBloc.fileMenuModel

    ScrollableTextPopUp {
        id: loading_error_pop_up
        title: qsTr("(TR) Can't open the file")
        popUpModel: fileSelectionBloc.popUpModel
        anchors.centerIn: parent
    }

    ColumnLayout {
        width: SizeManager.getSizeBasedOnAppWidth(512)
        anchors.centerIn: parent
        enabled: fileSelectionBloc.formButtonsEnabled

        FileListView {
            id: file_list_view
            Layout.fillWidth: parent
            model: fileSelectionBloc.fileListModel
        }

        RowLayout {
            Layout.fillWidth: parent

            MainButton {
                id: open_button
                text: qsTr("(TR) Open")
                enabled: (file_list_view.count > 0)
                Layout.topMargin: SizeManager.getSizeBasedOnAppWidth(15)
                Layout.fillWidth: parent
                onClicked: fileSelectionBloc.onOpenButtonClicked()
            }

            MainButton {
                id: cancel_button
                text: qsTr("(TR) Cancel")
                isPrimaryColor: false
                Layout.preferredWidth: parent.width / 2
                Layout.topMargin: SizeManager.getSizeBasedOnAppWidth(15)
                onClicked: fileSelectionBloc.onCancelButtonClicked()
            }
        }
    }

    FileDialog {
        id: file_dialog
        title: qsTr("(TR) Select a project file")
        folder: fileSelectionBloc.folderPath
        nameFilters: [
            "Project files (*.tb" + (fileSelectionBloc.yamlParsingEnabled ? " *.ytb" : "") + ")"
        ]
        onAccepted: fileSelectionBloc.onFileSelectedFromFileExplorer(
                        file_dialog.fileUrl)
    }

    Connections {
        target: fileSelectionBloc
        function onOpenFileExplorer() {
            file_dialog.open()
        }
    }

    Connections {
        target: fileSelectionBloc.popUpModel
        function onDisplayPopUp(errorMessage) {
            loading_error_pop_up.scrollableText.contentText.text = errorMessage
            loading_error_pop_up.open()
        }
    }

    Component.onCompleted: {
        fileSelectionBloc.currentlySelected = Qt.binding(function () {
            return file_list_view.currentIndex
        })
    }
}
