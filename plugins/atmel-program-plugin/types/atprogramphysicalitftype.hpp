// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


/** @brief Represents the type of physical interface you can use with atprogram */
class AtProgramPhysicalItfType : public QObject
{
    Q_OBJECT

    public:
        enum Enum {
            UPDI,
            Unknown
        };
        Q_ENUM(Enum)

    public:
        /** @brief Parse the atprogram physical interface from string
            @param itf The physical interface
            @return The value parsed */
        static Enum parseFromString(const QString &itf);

        /** @brief Transform the physical interface to string
            @note The result of this method can be used as arguments for the atprogram
            @param itf The physical interface to stringify
            @return The interface type stringified*/
        static QString toString(Enum itf);

};
