// SPDX-FileCopyrightText: 2021 - 2022 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "writeprocess.hpp"

#include "definesutility/definesutility.hpp"

#include "command/acmd.hpp"
#include "qtvisacomlib/src/avisacom.hpp"
#include "qtvisacomlib/src/visacommanager.hpp"
#include "response/aresponse.hpp"
#include "response/backresponse.hpp"
#include "response/idresponse.hpp"


WriteProcess::WriteProcess(const QString &interfaceId, quint16 timeout, QObject *parent) :
    QObject(parent)
{
    _visaCom = VisacomManager::getInstance().getVisaCom(interfaceId);
    _visaCom->setTimeout(timeout);

    connect(_visaCom.data(), &AVisacom::messageReceived, this, &WriteProcess::onResponse);
    connect(_visaCom.data(), &AVisacom::errorOccured, this, &WriteProcess::errorOccured);
}

bool WriteProcess::write(ACmd *cmd)
{
    QByteArray commandToSend;
    RETURN_IF_FALSE(cmd->formatCommand(commandToSend));

    return _visaCom->asyncWrite(commandToSend);
}

bool WriteProcess::query(ACmd *cmd)
{
    QByteArray commandToSend;
    RETURN_IF_FALSE(cmd->formatCommand(commandToSend));

    return _visaCom->asyncQuery(commandToSend);
}

void WriteProcess::manageResponse(const QString &response)
{
    AResponse::ResponseType responseEnum = AResponse::getResponseType(response);

    switch (responseEnum) {
        case AResponse::BackResponse:
        {
            QSharedPointer<BackResponse> backResponse
                    = QSharedPointer<BackResponse>(new BackResponse(response, this));

            if(!backResponse->parseResponse(response))
            {
                emit errorOccured();
            }

            emit responseReceived(backResponse);
            break;
        }

        case AResponse::IdResponse:
        {
            QSharedPointer<IdResponse> idResponse
                    = QSharedPointer<IdResponse>(new IdResponse(response, this));

            if(!idResponse->parseResponse(response))
            {
                emit errorOccured();
            }

            emit responseReceived(idResponse);
            break;
        }

        default:
        {
            emit errorOccured();
            break;
        }
    }
}

void WriteProcess::onResponse(const QByteArray &response)
{
    manageResponse(response);
}
