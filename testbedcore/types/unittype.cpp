// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "unittype.hpp"

#include <QMetaEnum>

const QHash<UnitType::Enum, QString> UnitType::UnitInfos = {
    { Ampere,           "A" },
    { Volt,             "V" },
    { Watt,             "W" },
    { CelciusDegree,    "°C" },
    { Ohm,              "Ω" },
    { Second,           "s"},
    { Hertz,            "Hz" },
    { Adc,              "Adc" },
    { Lumen,            "lm" },
    { Farad,            "F" },
    { Henry,            "H" },
    { Kelvin,           "K" },
    { BitPerSecond,     "Bit/s" },
    { Other,            "" }
};


UnitType::Enum UnitType::parseFromString(const QString &strValue)
{
    QString lowerValue = strValue.toLower();

    QMetaEnum metaEnum = QMetaEnum::fromType<Enum>();

    for(int idx = 0; idx < metaEnum.keyCount(); idx++)
    {
        QString strKey(metaEnum.key(idx));
        Enum value = static_cast<Enum>(metaEnum.value(idx));

        // We test if the given string matches the enum name
        if(strKey.toLower() == lowerValue)
        {
            return value;
        }

        // We test if the given string matches the enum symbol
        if(UnitInfos.contains(value) && UnitInfos[value].toLower() == lowerValue)
        {
            return value;
        }
    }

    return Enum::Other;
}

QString UnitType::getLongName(UnitType::Enum enumValue)
{
    return QString::fromLatin1(QMetaEnum::fromType<Enum>().valueToKey(enumValue));
}

QString UnitType::getSymbol(UnitType::Enum enumValue)
{
    return UnitInfos.value(enumValue, UnitInfos[Other]);
}
