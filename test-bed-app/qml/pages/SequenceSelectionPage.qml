// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.0
import QtQuick.Controls 2.13

import app.blocs 1.0

import "qrc:/pop_up"

/// Project openning sub-page dedicated to select a sequence
AFileSelectionPage {
    fileSelectionBloc: SequenceSelectionBloc {
        id: sequence_selection_bloc
    }
    property bool projectLoadingError: false

    fileListTitle: qsTr("(TR) Select a sequence")
}
