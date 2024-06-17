// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

import QtQuick 2.13
import QtQuick.Window 2.13
import QtQuick.Controls 2.13

import app.managers 1.0
import app.managers.navigation 1.0

/// This is the main application page
Window {

    minimumWidth: SizeManager.getAppWidth() / 2
    minimumHeight: SizeManager.getAppWidth() / 2
    visible: true
    visibility: "Maximized"

    function getUrl(pageType) {
        switch (pageType) {
        case NavigationPageType.LoginPage:
            return Qt.resolvedUrl("pages/LoginPage.qml")
        case NavigationPageType.ProjectSelectionPage:
            return Qt.resolvedUrl("pages/ProjectSelectionPage.qml")
        case NavigationPageType.SequenceSelectionPage:
            return Qt.resolvedUrl("pages/SequenceSelectionPage.qml")
        case NavigationPageType.SequenceRunPage:
            return Qt.resolvedUrl("pages/SequenceRunPage.qml")
        }

        return null
    }

    /// Manage the display of the right view
    StackView {
        id: stack

        readonly property var initPage: NavigationPageType.LoginPage

        anchors.fill: parent
        focus: true

        Component.onCompleted: {
            stack.push(getUrl(initPage), {
                           "objectName": NavigationManager.getPageName(initPage)
                       })
        }
    }

    Connections {
        target: NavigationManager

        function onChangePageTo(newPage, behavior) {
            const pageUrl = getUrl(newPage)
            const pageName = NavigationManager.getPageName(newPage)

            switch (behavior) {
            case NavigationBehavior.PushOrPop:
                var prev = stack.find(function (item, index) {
                    return (item.objectName === pageName)
                })

                if (prev !== null) {
                    stack.pop(prev)
                } else {
                    stack.push(pageUrl, {
                                   "objectName": pageName
                               })
                }
                break
            case NavigationBehavior.Replace:
                stack.replace(pageUrl, {
                                  "objectName": pageName
                              })
                break
            case NavigationBehavior.PopAllAndPush:
                stack.replace(null, pageUrl, {
                                  "objectName": pageName
                              })
            }

            NavigationManager.currentPage = newPage
        }
    }
}
