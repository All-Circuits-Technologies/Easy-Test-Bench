// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "sequencelogformattype.hpp"

#include <QMetaEnum>

QVector<SequenceLogFormatType::Enum> SequenceLogFormatType::enumList = {};


SequenceLogFormatType::Enum SequenceLogFormatType::parseFromString(const QString &value)
{
    QString lowerValue = value.toLower();

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

QString SequenceLogFormatType::toString(SequenceLogFormatType::Enum value)
{
    return QString::fromLatin1(QMetaEnum::fromType<Enum>().valueToKey(value));
}

const QVector<SequenceLogFormatType::Enum> &SequenceLogFormatType::getAllEnums()
{
    if(Q_UNLIKELY(enumList.isEmpty()))
    {
        const QMetaEnum metaEnum = QMetaEnum::fromType<Enum>();

        for(int idx = 0; idx < metaEnum.keyCount(); idx++)
        {
            enumList.append(static_cast<Enum>(metaEnum.value(idx)));
        }
    }

    return enumList;
}
