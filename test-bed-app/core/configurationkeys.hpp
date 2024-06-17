// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphaël Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


/** @brief List of the configuration keys */
class ConfigurationKeys : public QObject
{
    Q_OBJECT

    public:
        enum Enum
        {
            YearsOfProduction,
            Author,
            Address,
            Country,
            Website
        };

        Q_ENUM(Enum)

    public:
        /** @brief Convert key enum value to string
            @param configurationKey The enum value to convert
            @return Key value formatted to string */
        static QString toString(Enum configurationKey);
};
