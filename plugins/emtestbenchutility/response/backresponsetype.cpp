// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "backresponsetype.hpp"

#include <QMetaEnum>


BackResponseType::Enum BackResponseType::parseFromString(const QString &strValue)
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

QString BackResponseType::toString(BackResponseType::Enum backResponse)
{
    return QString::fromLatin1(QMetaEnum::fromType<Enum>().valueToKey(backResponse));
}

QString BackResponseType::reponseToString(Enum backResponse)
{
    switch(backResponse)
    {
        case BackResponseType::RR00:
            return BackResponseType::RR_00;

        case BackResponseType::RR01:
            return BackResponseType::RR_01;

        case BackResponseType::RR02:
            return BackResponseType::RR_02;

        case BackResponseType::RR05:
            return BackResponseType::RR_05;

        case BackResponseType::RR06:
            return BackResponseType::RR_06;

        case BackResponseType::RR07:
            return BackResponseType::RR_07;

        case BackResponseType::RR10:
            return BackResponseType::RR_10;

        case BackResponseType::RR11:
            return BackResponseType::RR_11;

        case BackResponseType::RR13:
            return BackResponseType::RR_13;

        case BackResponseType::RR14:
            return BackResponseType::RR_14;

        case BackResponseType::RR15:
            return BackResponseType::RR_15;

        case BackResponseType::RR17:
            return BackResponseType::RR_17;

        case BackResponseType::RR20:
            return BackResponseType::RR_20;

        case BackResponseType::Unknown:
            return BackResponseType::Unknown_Msg;
    }
}

void BackResponseType::registerMetaType()
{
    qRegisterMetaType<BackResponseType::Enum>("BackResponseType::Enum");
}
