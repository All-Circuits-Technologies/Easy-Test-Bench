// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "pulseenum.hpp"

#include <QMetaEnum>


PulseEnum::Enum PulseEnum::parseFromString(const QString &strValue)
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

QString PulseEnum::toString(PulseEnum::Enum pulseHeader)
{
    return QString::fromLatin1(QMetaEnum::fromType<Enum>().valueToKey(pulseHeader));
}
