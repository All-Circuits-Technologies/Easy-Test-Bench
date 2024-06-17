// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "memorytype.hpp"

#include <QMetaEnum>

QVector<MemoryType::Enum> MemoryType::EnumList = {};
QVector<MemoryType::Enum> MemoryType::EnumListWithoutUnknown = {};


QString MemoryType::toString(MemoryType::Enum memoryType)
{
    return QString::fromLatin1(QMetaEnum::fromType<Enum>().valueToKey(memoryType)).toLower();
}

MemoryType::Enum MemoryType::parseFromString(const QString &value)
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

QString MemoryType::toProgramArgumentStr(MemoryType::Enum memoryType)
{
    switch(memoryType)
    {
        case MemoryType::Eeprom:
        return ShortEepromName;

        case MemoryType::Flash:
            return ShortFlashName;

        case MemoryType::Unknown:
            return {};
    }

    return {};
}

const QVector<MemoryType::Enum> &MemoryType::getAllEnums(bool excludeUnknown)
{
    if(EnumList.isEmpty())
    {
        const QMetaEnum metaEnum = QMetaEnum::fromType<Enum>();

        for(int idx = 0; idx < metaEnum.keyCount(); idx++)
        {
            Enum value = static_cast<Enum>(metaEnum.value(idx));
            if(value != Unknown)
            {
                EnumListWithoutUnknown.append(value);
            }
            
            EnumList.append(value);
        }
    }

    if(excludeUnknown)
    {
        return EnumListWithoutUnknown;
    }
    
    return EnumList;
}
