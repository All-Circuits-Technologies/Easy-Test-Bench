// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "instrumentid.h"

#include <QMetaEnum>


InstrumentId::Enum InstrumentId::parseFromString(const QString &strValue)
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

QString InstrumentId::toString(Enum backResponse)
{
    return QString::fromLatin1(QMetaEnum::fromType<Enum>().valueToKey(backResponse));
}
