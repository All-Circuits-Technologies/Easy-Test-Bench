// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.11
import QtQuick.Window 2.15

import app.blocs 1.0
import app.managers 1.0

import "qrc:/basic_items"
import "qrc:/login"

ABasePage {
    id: login_page

    /// Backend bloc linked with this view
    readonly property LoginBloc loginBloc: LoginBloc {
        id: login_bloc
    }

    topBar.profilItemDisplayed: false
    topBar.title: qsTr("(TR) Login")
    topBar.leftButtonText: qsTr("(TR) File")
    topBar.fileMenu.menuModel: loginBloc.fileMenuModel

    /// Software logo
    Image {
        id: easy_test_bench_logo
        source: "qrc:/images/easy_test_bench_full.svg"
        sourceSize.height: SizeManager.getSizeBasedOnAppWidth(64)
        sourceSize.width: SizeManager.getSizeBasedOnAppWidth(367)
        fillMode: Image.PreserveAspectFit
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: SizeManager.getSizeBasedOnAppHeight(115)
    }

    GridLayout {
        id: login_grid
        columns: 2
        columnSpacing: SizeManager.getSizeBasedOnAppWidth(16)
        rowSpacing: SizeManager.getSizeBasedOnAppHeight(41)
        rows: 3
        anchors.top: easy_test_bench_logo.bottom
        anchors.topMargin: SizeManager.getSizeBasedOnAppHeight(116)
        anchors.right: easy_test_bench_logo.right
        anchors.rightMargin: SizeManager.getSizeBasedOnAppHeight(23)

        LoginText {
            text: qsTr("(TR) Username")
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
        }

        /// Username input
        LoginTextField {
            id: username_text_field
            font.family: FontsManager.latoRegular.name
            Layout.preferredWidth: SizeManager.getSizeBasedOnAppWidth(320)
            enabled: !login_bloc.loading
            onTextChanged: login_bloc.onUsernameUpdated(
                               username_text_field.text)
        }

        LoginText {
            text: qsTr("(TR) Password")
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
        }

        /// Password input
        LoginTextField {
            id: password_text_field
            echoMode: TextInput.Password
            Layout.fillWidth: true
            enabled: !login_bloc.loading
            onTextChanged: login_bloc.onPasswordUpdated(
                               password_text_field.text)
        }

        Item {
            height: 1
            width: 1
        }

        /// Button to initiate a connection attempt
        MainButton {
            id: login_button
            Layout.fillWidth: true
            Layout.topMargin: SizeManager.getSizeBasedOnAppWidth(10)
            Layout.maximumWidth: SizeManager.getSizeBasedOnAppWidth(320)
            Layout.alignment: Qt.AlignLeft | Qt.AlignCenter
            enabled: login_bloc.buttonEnabled
            text: login_bloc.buttonText
            onClicked: login_bloc.onConnexionButtonClicked(
                           username_text_field.text, password_text_field.text)
        }
    }

    /// Message displayed in case of connection problem
    Text {
        id: login_error_text
        color: ColorsManager.orange400
        font.family: FontsManager.latoRegular.name
        font.pixelSize: SizeManager.getSizeBasedOnAppWidth(24)
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: SizeManager.getSizeBasedOnAppHeight(590)
        visible: login_error_text.text.length > 0
    }

    Connections {
        target: login_bloc
        function onLoginFailed(error) {
            login_error_text.text = error
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:0.75;height:800;width:1280}
}
##^##*/

