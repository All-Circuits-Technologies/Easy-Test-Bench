// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>
#include <QString>
#include <QVector>


/** @brief The unit type known by the library */
class UnitType : public QObject
{
    public:
        enum Enum
        {
            Ampere,
            Volt,
            Watt,
            CelciusDegree,
            Ohm,
            Second,
            Hertz,
            Adc,
            Lumen,
            Farad,
            Henry,
            Kelvin,
            BitPerSecond,
            Other,
            None
        };
        Q_ENUM(Enum)

    public:
        /** @brief Parse a string in order to get an unit
            @param strValue The string value to parse
            @return The unit type parsed */
        static Enum parseFromString(const QString &strValue);

        /** @brief Get the long name of the unit
            @param enumValue The unit type to get the long name from
            @return The long name of unit */
        static QString getLongName(Enum enumValue);

        /** @brief Get the symbol of the unit
            @param enumValue The unit type to get the symbol from
            @return The symbol of unit */
        static QString getSymbol(Enum enumValue);

    private:
        static const QHash<Enum, QString> UnitInfos;
};
