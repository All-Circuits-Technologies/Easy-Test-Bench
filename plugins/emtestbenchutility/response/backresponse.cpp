// SPDX-FileCopyrightText: 2021 - 2022 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "backresponse.hpp"

#include <QRegularExpression>
#include <QDebug>


BackResponse::BackResponse(const QString &response, QObject *parent) :
    AResponse(response, parent)
{
    responseType = AResponse::BackResponse;
}

BackResponse::~BackResponse()
{
}

bool BackResponse::parseResponse(const QString &response)
{
   QString backResponse;
   backResponse = response.simplified();
   backResponse.replace(" ","");

   _backResponseEnum = BackResponseType::parseFromString(backResponse);

   if(_backResponseEnum == BackResponseType::Unknown)
   {
       qWarning() << "Unknown back response enum";
       return false;
   }

   return true;
}

const BackResponseCategory::Enum BackResponse::getBackResponseCategory() const
{
    switch (_backResponseEnum)
    {
        case BackResponseType::RR00:
        case BackResponseType::RR01:
        case BackResponseType::RR02:
        case BackResponseType::RR07:
        case BackResponseType::RR13:
        case BackResponseType::RR14:
            return BackResponseCategory::InfoResponse;
        case BackResponseType::RR05:
        case BackResponseType::RR06:
        case BackResponseType::RR10:
        case BackResponseType::RR11:
        case BackResponseType::RR15:
        case BackResponseType::RR17:
        case BackResponseType::RR20:
            return BackResponseCategory::ErrorResponse;
        default:
            return BackResponseCategory::Unknown;
    }
}
