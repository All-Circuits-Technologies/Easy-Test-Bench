// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.0
import QtQuick.Controls 2.15

import app.blocs 1.0
import app.managers 1.0

/// Project openning sub-page dedicated to select a project
AFileSelectionPage {
    fileSelectionBloc: ProjectSelectionBloc {
        id: project_selection_bloc
    }

    fileListTitle: qsTr("(TR) Recent projects")
    cancelButtonVisible: false
}
