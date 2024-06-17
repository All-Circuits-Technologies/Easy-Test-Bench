// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.0
import QtQuick.Layouts 1.15

import app.blocs 1.0
import app.managers 1.0
import app.models 1.0
import app.types 1.0

// Base class for plugin input views
Rectangle {
    id: plugin_input

    required property APluginInputBloc aInputBloc
    required property APluginInputModel inputModel

    required property Item parentLoader

    property alias columnContent: other_content.children

    readonly property int spacingBetweenElems: SizeManager.getSizeBasedOnAppHeight(
                                                   20)

    color: ColorsManager.grey200

    border.color: Qt.darker(ColorsManager.grey200, 1.2)
    border.width: SizeManager.getSizeBasedOnAppWidth(3)

    Component {
        id: blank
        Item {
            width: 0
            height: 0
        }
    }

    ColumnLayout {
        id: header_content

        spacing: spacingBetweenElems

        anchors.top: plugin_input.top
        anchors.topMargin: SizeManager.getSizeBasedOnAppWidth(18)
        anchors.left: plugin_input.left
        anchors.leftMargin: anchors.topMargin
        anchors.right: plugin_input.right
        anchors.rightMargin: anchors.topMargin
        anchors.bottom: plugin_input.bottom
        anchors.bottomMargin: anchors.topMargin



        Loader {
            id: image_loader

            Layout.preferredWidth: plugin_input.width - ( 2 * spacingBetweenElems )
            Layout.preferredHeight: plugin_input.height / 3
            Layout.alignment: Qt.AlignHCenter

            sourceComponent: (!inputModel || inputModel.imageType === ImageType.None) ?
                                 blank :
                                 ((inputModel.imageType === ImageType.Animated) ?
                                      anim_image_component : image_component)
        }

        Component {
            id: image_component
            Image {
                id: image
                fillMode: Image.PreserveAspectFit
                source: aInputBloc.inputModel.imagePath
            }
        }

        Component {
            id: anim_image_component
            AnimatedImage {
                id: anim_image
                fillMode: Image.PreserveAspectFit
                source: aInputBloc.inputModel.imagePath
            }
        }

        Text {
            id: description
            text: aInputBloc.inputModel.description
            color: ColorsManager.grey800
            font.family: FontsManager.latoRegular.name
            font.pixelSize: SizeManager.getSizeBasedOnAppWidth(18)
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            Layout.fillWidth: true
        }

        Item {
            id: other_content
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }

    Component {
        id: timeout_value

        Rectangle {
            width: SizeManager.getSizeBasedOnAppWidth(40)
            height: SizeManager.getSizeBasedOnAppWidth(40)
            color: aInputBloc.inputModel.timeoutLeftInS > 5 ? plugin_input.border.color :
                                                              ColorsManager.red400

            Text {
                text: aInputBloc.inputModel.timeoutLeftInS
                color: aInputBloc.inputModel.timeoutLeftInS > 5 ? ColorsManager.grey800 :
                                                                  ColorsManager.grey50
                font.family: FontsManager.latoSemiBold.name
                font.pixelSize: SizeManager.getSizeBasedOnAppWidth(18)
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                anchors.centerIn: parent
            }
        }
    }

    Loader {
        id: timer_loader

        anchors.right: parent.right
        anchors.top: parent.top

        sourceComponent: aInputBloc.inputModel.timerEnabled ? timeout_value : blank
    }

    Component.onCompleted: {
        aInputBloc.inputModel = plugin_input.inputModel
        aInputBloc.inputModel.displayed = true;
    }
}
