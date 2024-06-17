// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "popupbtntype.hpp"

#include <QDebug>
#include <QMetaEnum>

QVector<PopUpBtnType::Enum> PopUpBtnType::EnumList = {};


QString PopUpBtnType::toDisplayString(Enum btnType)
{
    switch(btnType)
    {
        case PopUpBtnType::OkBtn:
            return OkLabel;
        case PopUpBtnType::YesBtn:
            return YesLabel;
        case PopUpBtnType::NoBtn:
            return NoLabel;
        case PopUpBtnType::Unknown:
            return {};
    }

    qWarning() << "The PopUpBtn enum: " << btnType << ", has no displayable string";
    return {};
}

const QVector<PopUpBtnType::Enum> &PopUpBtnType::getAllEnums()
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

QVector<PopUpBtnType::Enum> PopUpBtnType::getEnumsList(Enums btnTypes)
{
    QVector<PopUpBtnType::Enum> typesList;

    for(PopUpBtnType::Enum type : getAllEnums())
    {
        if(type == PopUpBtnType::Unknown)
        {
            // We don't add Unknown
            continue;
        }

        if(btnTypes.testFlag(type))
        {
            typesList.append(type);
        }
    }

    return typesList;
}

void PopUpBtnType::registerMetaTypes()
{
    qRegisterMetaType<PopUpBtnType::Enum>("PopUpBtnType::Enum");
    qRegisterMetaType<PopUpBtnType::Enums>("PopUpBtnType::Enums");
}
