// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "qtdateformathelper.hpp"

#include <QMetaEnum>

QVector<Qt::DateFormat> QtDateFormatHelper::enumList = {};


QString QtDateFormatHelper::toString(Qt::DateFormat dateFormat)
{
    return QString::fromLatin1(QMetaEnum::fromType<Qt::DateFormat>().valueToKey(dateFormat));
}

Qt::DateFormat QtDateFormatHelper::parseFromString(const QString &dateFormat, bool *ok)
{
    auto manageOk = [ok](bool isOk)
    {
        if(ok != nullptr)
        {
            *ok = isOk;
        }
    };

    QString lowerValue = dateFormat.toLower();

    QMetaEnum metaEnum = QMetaEnum::fromType<Qt::DateFormat>();

    for(int idx = 0; idx < metaEnum.keyCount(); idx++)
    {
        QString strValue(metaEnum.key(idx));

        if(strValue.toLower() == lowerValue)
        {
            manageOk(true);
            return static_cast<Qt::DateFormat>(metaEnum.value(idx));
        }
    }

    manageOk(false);
    return Qt::ISODateWithMs;
}

const QVector<Qt::DateFormat> &QtDateFormatHelper::getAllEnums()
{
    if(Q_UNLIKELY(enumList.isEmpty()))
    {
        const QMetaEnum metaEnum = QMetaEnum::fromType<Qt::DateFormat>();

        for(int idx = 0; idx < metaEnum.keyCount(); idx++)
        {
            enumList.append(static_cast<Qt::DateFormat>(metaEnum.value(idx)));
        }
    }

    return enumList;
}
