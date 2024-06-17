// SPDX-FileCopyrightText: 2023 Julien Lemasson <julien.lemasson@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "comparecansequenceinfo.hpp"

#include "models/canmsgseqparams.hpp"
#include "models/comparemsg/comparecansequenceactioninfo.hpp"
#include "models/comparemsg/compareexpectedmsg.hpp"


CompareCanSequenceInfo::CompareCanSequenceInfo(
    const QString &testGroupName,
    int timeoutInMs,
    const QVector<const CompareCanSequenceActionInfo *> &seqActionsInfo,
    QObject *parent)
    : QObject{parent},
    _seqActionsInfo{seqActionsInfo},
    _testGroupName{testGroupName},
    _timeoutInMs{timeoutInMs}
{
}

CompareCanSequenceInfo::~CompareCanSequenceInfo()
{
    qDeleteAll(_seqActionsInfo);
}

const CompareCanSequenceInfo *CompareCanSequenceInfo::parsePCanSequenceInfo(
    const CanMsgSeqParams &msgSeqParams,
    bool manageMultipleCmd)
{
    const QVector<QString> &cmds = msgSeqParams.getCmds();
    const QVector<Number> &expectedMsgsIds = msgSeqParams.getExpectedMsgsIds();
    const QVector<QString> &verifs  = msgSeqParams.getVerifs();

    if(!cmds.isEmpty() && !expectedMsgsIds.isEmpty() && expectedMsgsIds.length() != cmds.length())
    {
        qWarning() << "Fail to parse the PCAN Comparing info. If the 'expectedMsgIds' list "
                   << "contains elements, the length of this list and the 'cmds' list has to be "
                   << "equal.";
        return nullptr;
    }
    
    QVector<const CompareExpectedMsg *> expectedMsgs;

    for(auto citer = verifs.cbegin(); citer != verifs.cend(); ++citer)
    {
        const CompareExpectedMsg *expectedMsg = CompareExpectedMsg::parseExpectedMessage(
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

    QVector<const CompareCanSequenceActionInfo *> seqActionInfo;
    const bool hasExpectedMsg = !expectedMsgsIds.isEmpty();
    const bool hasCmdToSend = !cmds.isEmpty();
    const int length = qMax(expectedMsgsIds.length(), cmds.length());

    for(int idx = 0; idx < length; ++idx)
    {
        QVector<const CompareExpectedMsg *> tmpExpectedMsgs;

        if(hasExpectedMsg)
        {
            CompareExpectedMsg::extractExpectedMessage(idx, expectedMsgs, tmpExpectedMsgs);
        }

        const CompareCanSequenceActionInfo *cmd = CompareCanSequenceActionInfo::parsePCanSequenceActionInfo(
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

    return new CompareCanSequenceInfo(msgSeqParams.getTestGroupName(),
                                      msgSeqParams.getTimeoutInMs(),
                                      seqActionInfo);
}
