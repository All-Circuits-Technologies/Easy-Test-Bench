// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "sequencelogcategory.hpp"

#include <QDebug>
#include <QMetaEnum>

SequenceLogCategory::Enums SequenceLogCategory::AllElements = {};
QVector<SequenceLogCategory::Enum> SequenceLogCategory::EnumList = {};


void SequenceLogCategory::RegisterMetaTypes()
{
    qRegisterMetaType<SequenceLogCategory::Enum>("SequenceLogCategory::Enum");
    qRegisterMetaType<SequenceLogCategory::Enums>("SequenceLogCategory::Enums");
}

QString SequenceLogCategory::toString(SequenceLogCategory::Enum value)
{
    return QString::fromLatin1(QMetaEnum::fromType<Enum>().valueToKey(value));
}

SequenceLogCategory::Enums SequenceLogCategory::parseFromVector(const QVector<QString> &values)
{
    Enums enumsValues;

    for(const QString &value : values)
    {
        bool ok = false;
        Enum enumValue = parseFromString(value, &ok);

        if(!ok)
        {
            return {};
        }

        enumsValues.setFlag(enumValue);
    }

    return enumsValues;
}

SequenceLogCategory::Enum SequenceLogCategory::parseFromString(const QString &value,
                                                               bool *ok,
                                                               Enum defaultValue)
{
    auto manageOk = [ok](bool isOk)
    {
        if(ok != nullptr)
        {
            *ok = isOk;
        }
    };

    QString lowerValue = value.toLower();
    QMetaEnum metaEnum = QMetaEnum::fromType<Enum>();

    for(int idx = 0; idx < metaEnum.keyCount(); idx++)
    {
        QString strValue(metaEnum.key(idx));

        if(strValue.toLower() == lowerValue)
        {
            manageOk(true);
            return static_cast<Enum>(metaEnum.value(idx));
        }
    }

    manageOk(false);
    return defaultValue;
}

SequenceLogCategory::Enums SequenceLogCategory::getAllFlags()
{
    if(AllElements == 0)
    {
        const QMetaEnum metaEnum = QMetaEnum::fromType<Enum>();

        for(int idx = 0; idx < metaEnum.keyCount(); idx++)
        {
            AllElements |= static_cast<Enum>(metaEnum.value(idx));
        }
    }

    return AllElements;
}

QVector<SequenceLogCategory::Enum> SequenceLogCategory::getAllEnums()
{
    if(EnumList.isEmpty())
    {
        const QMetaEnum metaEnum = QMetaEnum::fromType<Enum>();

        for(int idx = 0; idx < metaEnum.keyCount(); idx++)
        {
            EnumList.append(static_cast<Enum>(metaEnum.value(idx)));
        }
    }

    return EnumList;
}
