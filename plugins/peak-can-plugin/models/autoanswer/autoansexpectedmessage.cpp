// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "autoansexpectedmessage.hpp"

#include <QCanBusFrame>
#include <QDebug>

#include "numberutility/number.hpp"

#include "models/autoanswer/autoanssequactioninfo.hpp"
#include "peakcanconstants.hpp"


AutoAnsExpectedMessage::AutoAnsExpectedMessage(quint32 msgId,
                                               quint8 maskIdx,
                                               const QByteArray &mask,
                                               const QByteArray &expectedMaskResult,
                                               QVector<const AutoAnsSequActionInfo *> seqActions,
                                               QObject *parent)
    : APCanExpectedMsg{maskIdx, mask, expectedMaskResult, parent},
    _msgId{msgId},
    _seqActions{seqActions}
{
}

AutoAnsExpectedMessage::~AutoAnsExpectedMessage()
{
    qDeleteAll(_seqActions);
}

bool AutoAnsExpectedMessage::checkIfMessageReceivedIsValid(const QCanBusFrame &messageReceived,
                                                           bool silent) const
{
    if(messageReceived.frameId() != _msgId)
    {
        if(!silent)
        {
            qWarning() << "The message received with id: " << messageReceived.frameId()
                       << ", doesn't match the expected message id: " << _msgId;
        }

        return false;
    }

    return APCanExpectedMsg::checkIfMessageReceivedIsValid(messageReceived, silent);
}

const AutoAnsExpectedMessage *AutoAnsExpectedMessage::parseExpectedMessage(
    const QString &moduleParameter,
    const QVector<const AutoAnsSequActionInfo *> &seqActions)
{
    QStringList parametersParts = moduleParameter.split(PeakCanConstants::Parser::StrSeparator);


    if(parametersParts.length() != PartsNb)
    {
        qWarning() << "The AutoAnswer PCAN expected message can't be parsed, because it doesn't "
                   << "contain the right count of sub elements: " << moduleParameter;
        return nullptr;
    }

    Number numbMsgId = Number::fromString(parametersParts[MsgIdPartIdx]);
    if(!numbMsgId.isValid())
    {
        qWarning() << "We were trying to parse the can writing expected answer: " << parametersParts
                   << ", but the command index part can't be converted to a number";
        return nullptr;
    }

    bool ok = false;
    quint32 msgId = numbMsgId.toUint32(&ok);
    if(!ok)
    {
        qWarning() << "We were trying to parse the can writing expected answer: " << parametersParts
                   << ", but the command index part can't be converted to a quint32";
        return nullptr;
    }

    quint8 maskIdx = 0;
    QByteArray mask;
    QByteArray expectedMaskResult;
    
    if(!getInfoFromParameters(parametersParts,
                               MaskIdxPartIdx,
                               MaskPartIdx,
                               ExpectedMaskResultPartIdx,
                               maskIdx,
                               mask,
                               expectedMaskResult))
    {
        return nullptr;
    }

    return new AutoAnsExpectedMessage(msgId, maskIdx, mask, expectedMaskResult, seqActions);
}
