// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "aresponse.hpp"

#include <QMetaEnum>
#include <QRegularExpression>


AResponse::AResponse(const QString &response, QObject *parent) :
    QObject(parent)
{
}

AResponse::~AResponse()
{
}

AResponse::ResponseType AResponse::getResponseType(const QString &response)
{
    if(response.startsWith(AResponse::Back_Response))
    {
        return BackResponse;
    }

    if (response.startsWith(AResponse::VDS200) ||
        response.startsWith(AResponse::LD200)  ||
        response.startsWith(AResponse::MPG200))
    {
        return IdResponse;
    }

    return Unknown;
}
