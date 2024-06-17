// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


/** @brief Used to represent the endianess type of an integer */
class EndianessType : public QObject
{
    Q_OBJECT

    public:
        enum Enum
        {
            LsbFirst,
            MsbFirst,
            Unknown
        };
        Q_ENUM(Enum)

    public:
        /** @brief Parse the endianess type from string
            @param value The endianess type to parse
            @return The value parsed */
        static Enum parseFromString(const QString &value);

        /** @brief Get a string representation of the enum
            @param value The enum value to stringify
            @return The string representation of the enum */
        static QString toString(Enum value);
};
