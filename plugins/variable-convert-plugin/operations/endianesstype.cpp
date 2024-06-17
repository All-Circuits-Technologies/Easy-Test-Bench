// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "endianesstype.hpp"

#include <QMetaEnum>


EndianessType::Enum EndianessType::parseFromString(const QString &value)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<Enum>();

    for(int idx = 0; idx < metaEnum.keyCount(); idx++)
    {
        QString strValue(metaEnum.key(idx));

        if(strValue.toLower() == value.toLower())
        {
            return static_cast<Enum>(metaEnum.value(idx));
        }
    }

    return Unknown;
}

QString EndianessType::toString(EndianessType::Enum value)
{
    return QString::fromLatin1(QMetaEnum::fromType<Enum>().valueToKey(value)).toLower();
}
