// SPDX-FileCopyrightText: 2021 - 2022 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


/** @brief Manage the different instrumentID Value */
class InstrumentId : public QObject
{
    Q_OBJECT

    public:
        enum Enum: quint16
        {
            VDS200 = 0,
            MPG200 = 1,
            LD200  = 2,
            Unknown
        };

        Q_ENUM(Enum)

    public:
        /** @brief Parse the enum from string
            @param strValue The string to parse
            @return The enum parsed */
        static Enum parseFromString(const QString &strValue);

        /** @brief Return a string representation of the instrumentID
            @param InstrumentID The enum instrument value
            @return The string representation */
        static QString toString(Enum backResponse);

};
