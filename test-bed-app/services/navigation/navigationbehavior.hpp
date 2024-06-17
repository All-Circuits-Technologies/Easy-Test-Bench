// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


/** @brief Specify the navigation behavior between QML pages */
class NavigationBehavior : public QObject
{
    Q_OBJECT

    public:
        enum Enum {
            Replace,    /*!< @brief Replace the current page by the wanted one
                             @note This only changes the current page and doesn't modify the current
                                   stack */
            PushOrPop,  /*!< @brief Try to find the page in the current stack:
                                    - If the page exists, pop all the pages to reach the wanted one
                                    - If the page is the current one, do nothing
                                    - If the page doesn't exist in the current stack, push the page
                                      to the stack */
            PopAllAndPush /*!< @brief Pop all the other pages and push the new one */
        };
        Q_ENUM(Enum)

    public:
        /** @brief Register meta type linked to this enum */
        static void registerMetaTypes();

        /** @brief Register QML type linked to this enum */
        static void registerQmlTypes();
};
