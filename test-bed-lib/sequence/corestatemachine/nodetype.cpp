// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "nodetype.hpp"

#include <QMetaEnum>


NodeType::Enum NodeType::parseFromString(const QString &strValue)
{
    QString lowerValue = strValue.toLower();

    QMetaEnum metaEnum = QMetaEnum::fromType<Enum>();

    for(int idx = 0; idx < metaEnum.keyCount(); idx++)
    {
        QString value(metaEnum.key(idx));

        if(value.toLower() == lowerValue)
        {
            return static_cast<Enum>(metaEnum.value(idx));
        }
    }

    return UnknownNode;
}

QString NodeType::toString(NodeType::Enum value)
{
    return QString::fromLatin1(QMetaEnum::fromType<Enum>().valueToKey(value));
}
