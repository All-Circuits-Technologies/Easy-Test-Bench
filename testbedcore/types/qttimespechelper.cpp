// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "qttimespechelper.hpp"

#include <QMetaEnum>

QVector<Qt::TimeSpec> QtTimeSpecHelper::enumList = {};


QString QtTimeSpecHelper::toString(Qt::TimeSpec timeSpec)
{
    return QString::fromLatin1(QMetaEnum::fromType<Qt::TimeSpec>().valueToKey(timeSpec));
}

Qt::TimeSpec QtTimeSpecHelper::parseFromString(const QString &timeSpec, bool *ok)
{
    auto manageOk = [ok](bool isOk)
    {
        if(ok != nullptr)
        {
            *ok = isOk;
        }
    };

    QString lowerValue = timeSpec.toLower();

    QMetaEnum metaEnum = QMetaEnum::fromType<Qt::TimeSpec>();

    for(int idx = 0; idx < metaEnum.keyCount(); idx++)
    {
        QString strValue(metaEnum.key(idx));

        if(strValue.toLower() == lowerValue)
        {
            manageOk(true);
            return static_cast<Qt::TimeSpec>(metaEnum.value(idx));
        }
    }

    manageOk(false);
    return Qt::UTC;
}

const QVector<Qt::TimeSpec> &QtTimeSpecHelper::getAllEnums()
{
    if(Q_UNLIKELY(enumList.isEmpty()))
    {
        const QMetaEnum metaEnum = QMetaEnum::fromType<Qt::TimeSpec>();

        for(int idx = 0; idx < metaEnum.keyCount(); idx++)
        {
            enumList.append(static_cast<Qt::TimeSpec>(metaEnum.value(idx)));
        }
    }

    return enumList;
}
