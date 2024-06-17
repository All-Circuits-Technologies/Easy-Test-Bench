// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "atprogramphysicalitftype.hpp"

#include <QMetaEnum>


AtProgramPhysicalItfType::Enum AtProgramPhysicalItfType::parseFromString(const QString &itf)
{
    QString lowerValue = itf.toLower();

    QMetaEnum metaEnum = QMetaEnum::fromType<Enum>();

    for(int idx = 0; idx < metaEnum.keyCount(); idx++)
    {
        QString strValue(metaEnum.key(idx));

        if(strValue.toLower() == lowerValue)
        {
            return static_cast<Enum>(metaEnum.value(idx));
        }
    }

    return Unknown;
}

QString AtProgramPhysicalItfType::toString(AtProgramPhysicalItfType::Enum itf)
{
    return QString::fromLatin1(QMetaEnum::fromType<Enum>().valueToKey(itf));
}
