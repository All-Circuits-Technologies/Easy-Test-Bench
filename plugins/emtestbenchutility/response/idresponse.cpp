// SPDX-FileCopyrightText: 2021 - 2022 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "idresponse.hpp"

#include <QRegularExpression>


IdResponse::IdResponse(const QString &response, QObject *parent) :
    AResponse(response, parent)
{
    responseType = AResponse::IdResponse;
}

IdResponse::~IdResponse()
{
}

bool IdResponse::parseResponse(const QString &response)
{
    QStringList list;
    QString value = response;

    list = value.remove(QLatin1Char(';')).split(QLatin1Char(','));
    _idResponseEnum = InstrumentId::parseFromString(list.at(instrumentIdx));

    if(_idResponseEnum == InstrumentId::Unknown)
    {
        return false;
    }

    return true;
}
