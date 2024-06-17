// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QVector>


/** @brief List of the value type managed */
class TypeManaged : public QObject
{
    Q_OBJECT

    public:
        enum Enum
        {
            Boolean,
            String,
            Number,
            BooleanList,
            StringList,
            NumberList,
            Unknown
        };

        Q_ENUM(Enum)

    public:
        /** @brief Parse the enum from string
            @param strValue The string to parse
            @return The enum parsed */
        static Enum parseFromString(const QString &strValue);

        /** @brief Return a string representation of the Type Managed
            @param typeManaged The type to get the string representation from
            @return The string representation */
        static QString toString(Enum typeManaged);

        /** @brief Test if the type managed is a list
            @param typeManaged The type to test
            @return True if the type is a list */
        static bool isList(Enum typeManaged);

        /** @brief Get all the available elements
            @return All the enums */
        static QVector<Enum> getAvailableElements();
};
