// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "pcansequenceinfo.hpp"

#include "models/canmsgseqparams.hpp"
#include "models/readandwrite/pcansequenceactioninfo.hpp"
#include "models/readandwrite/pcanexpectedmessage.hpp"


PCanSequenceInfo::PCanSequenceInfo(const QString &testGroupName,
                                   int timeoutInMs,
                                   const QVector<const PCanSequenceActionInfo *> &seqActionsInfo,
                                   QObject *parent)
    : QObject{parent},
    _seqActionsInfo{seqActionsInfo},
    _testGroupName{testGroupName},
    _timeoutInMs{timeoutInMs}
{
}

PCanSequenceInfo::~PCanSequenceInfo()
{
    qDeleteAll(_seqActionsInfo);
}

const PCanSequenceInfo *PCanSequenceInfo::parsePCanSequenceInfo(
    const CanMsgSeqParams &msgSeqParams,
    bool manageMultipleCmd)
{
    const QVector<QString> &cmds = msgSeqParams.getCmds();
    const QVector<Number> &expectedMsgsIds = msgSeqParams.getExpectedMsgsIds();
    const QVector<QString> &verifs  = msgSeqParams.getVerifs();

    if(!cmds.isEmpty() && !expectedMsgsIds.isEmpty() && expectedMsgsIds.length() != cmds.length())
    {
        qWarning() << "Fail to parse the PCAN Writing info. If the 'expectedMsgIds' list contains "
                   << "elements, the length of this list and the 'cmds' list has to be equal.";
        return nullptr;
    }
    
    QVector<const PCanExpectedMessage *> expectedMsgs;

    for(auto citer = verifs.cbegin(); citer != verifs.cend(); ++citer)
    {
        const PCanExpectedMessage *expectedMsg = PCanExpectedMessage::parseExpectedMessage(
                                                                                *citer,
                                                                                manageMultipleCmd);

        if(expectedMsg == nullptr)
        {
            qWarning() << "A problem occurred when tried to parse an expected answer for the PCan "
                          "writing module";
            qDeleteAll(expectedMsgs);
            return nullptr;
        }

        expectedMsgs.append(expectedMsg);
    }

    QVector<const PCanSequenceActionInfo *> seqActionInfo;
    const bool hasExpectedMsg = !expectedMsgsIds.isEmpty();
    const bool hasCmdToSend = !cmds.isEmpty();
    const int length = qMax(expectedMsgsIds.length(), cmds.length());

    for(int idx = 0; idx < length; ++idx)
    {
        QVector<const PCanExpectedMessage *> tmpExpectedMsgs;

        if(hasExpectedMsg)
        {
            PCanExpectedMessage::extractExpectedMessage(idx, expectedMsgs, tmpExpectedMsgs);
        }

        const PCanSequenceActionInfo *cmd = PCanSequenceActionInfo::parsePCanSequenceActionInfo(
            hasCmdToSend ? cmds.at(idx) : QString(),
            msgSeqParams.hasBitrateSwitch(),
            msgSeqParams.isExtendedFrameFormat(),
            tmpExpectedMsgs,
            hasExpectedMsg ? expectedMsgsIds.at(idx) : Number());

        if(cmd == nullptr)
        {
            qWarning() << "A problem occurred when tried to parse a CAN sequence action info for "
                       << "a PCan module";
            qDeleteAll(seqActionInfo);
            qDeleteAll(tmpExpectedMsgs);
            qDeleteAll(expectedMsgs);
            return nullptr;
        }

        seqActionInfo.append(cmd);
    }

    if(!expectedMsgs.isEmpty())
    {
        qWarning() << "There is some expected message which aren't linked to one of expected "
                   << "message ids";
        qDeleteAll(seqActionInfo);
        qDeleteAll(expectedMsgs);
        return nullptr;
    }

    return new PCanSequenceInfo(msgSeqParams.getTestGroupName(),
                                msgSeqParams.getTimeoutInMs(),
                                seqActionInfo);
}
