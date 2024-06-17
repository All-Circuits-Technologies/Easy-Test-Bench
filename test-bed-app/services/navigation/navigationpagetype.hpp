// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


/** @brief The different type of pages */
class NavigationPageType : public QObject
{
        Q_OBJECT

    public:
        enum Enum
        {
            LoginPage,
            ProjectSelectionPage,
            SequenceSelectionPage,
            SequenceRunPage
        };
        Q_ENUM(Enum)

    public:
        /** @brief Stringify the page type
            @param pageType The pageType to get its name
            @return The str value of the enum given */
        static QString toString(NavigationPageType::Enum pageType);

    public:
        /** @brief Register meta type linked to this enum */
        static void registerMetaTypes();

        /** @brief Register QML type linked to this enum */
        static void registerQmlTypes();
};
