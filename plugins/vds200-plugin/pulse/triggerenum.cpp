// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "triggerenum.hpp"

#include <QMetaEnum>

QVector<TriggerEnum::Enum> TriggerEnum::enumList = {};


TriggerEnum::Enum TriggerEnum::parseFromString(const QString &strValue)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<Enum>();

    for(int idx = 0; idx < metaEnum.keyCount(); idx++)
    {
        QString value(metaEnum.key(idx));

        if(value.toLower() == strValue.toLower())
        {
            return static_cast<Enum>(metaEnum.value(idx));
        }
    }

    return Unknown;
}

QString TriggerEnum::toString(TriggerEnum::Enum trigger)
{
    return QString::fromLatin1(QMetaEnum::fromType<Enum>().valueToKey(trigger));
}

const QVector<TriggerEnum::Enum> &TriggerEnum::getAllEnums()
{
    if(enumList.isEmpty())
    {
        const QMetaEnum metaEnum = QMetaEnum::fromType<Enum>();

        for(int idx = 0; idx < metaEnum.keyCount(); idx++)
        {
            Enum value = static_cast<Enum>(metaEnum.value(idx));
            enumList.append(value);
        }
    }

    return enumList;
}
