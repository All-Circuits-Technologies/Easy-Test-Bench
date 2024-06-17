// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "autoanssequenceinfo.hpp"

#include "models/canmsgseqparams.hpp"
#include "models/autoanswer/autoansexpectedmessage.hpp"
#include "models/autoanswer/autoanssequactioninfo.hpp"


AutoAnsSequenceInfo::AutoAnsSequenceInfo(
    const QVector<const AutoAnsExpectedMessage*> &expectedMessages,
    QObject *parent)
    : QObject{parent},
    _expectedMessages{expectedMessages}
{
}

AutoAnsSequenceInfo::~AutoAnsSequenceInfo()
{
}

QVector<const AutoAnsExpectedMessage *> AutoAnsSequenceInfo::getMatchedElements(
    const QCanBusFrame &messageReceived) const
{
    QVector<const AutoAnsExpectedMessage *> matchedElements;
    for(auto citer = _expectedMessages.cbegin(); citer != _expectedMessages.cend(); ++citer)
    {
        if((*citer)->checkIfMessageReceivedIsValid(messageReceived, true))
        {
            matchedElements.append(*citer);
        }
    }

    return matchedElements;
}

const AutoAnsSequenceInfo *AutoAnsSequenceInfo::parsePCanSequenceInfo(
    const CanMsgSeqParams &msgSeqParams)
{
    const QVector<QString> &cmds = msgSeqParams.getCmds();
    const QVector<QString> &verifs  = msgSeqParams.getVerifs();

    QVector<const AutoAnsSequActionInfo*> seqActionsInfo;

    for(auto citer = cmds.cbegin(); citer != cmds.cend(); ++citer)
    {
        const AutoAnsSequActionInfo* seqActionInfo =
            AutoAnsSequActionInfo::parsePCanSequenceActionInfo(
                *citer,
                msgSeqParams.hasBitrateSwitch(),
                msgSeqParams.isExtendedFrameFormat());

        if(seqActionInfo == nullptr)
        {
            qWarning() << "A problem occurred when tried to parse an answer to send for the PCan "
                          "auto answer module";
            qDeleteAll(seqActionsInfo);
            return nullptr;
        }

        seqActionsInfo.append(seqActionInfo);
    }

    QVector<const AutoAnsExpectedMessage *> expectedMsgs;
    for(int idx = 0; idx < verifs.length(); ++idx)
    {
        QVector<const AutoAnsSequActionInfo*> tmpSeqActionsInfo;
        AutoAnsSequActionInfo::extractSequAction(idx, seqActionsInfo, tmpSeqActionsInfo);

        if(tmpSeqActionsInfo.isEmpty())
        {
            qWarning() << "A problem occured when tried to parse the auto answer expected "
                       << "messages: we haven't found a command to send linked to the expected "
                          "message at index: " << idx;
            qDeleteAll(seqActionsInfo);
            qDeleteAll(expectedMsgs);
            return nullptr;
        }

        const AutoAnsExpectedMessage *expectedMsg = AutoAnsExpectedMessage::parseExpectedMessage(
            verifs.at(idx),
            tmpSeqActionsInfo);

        if(expectedMsg == nullptr)
        {
            qWarning() << "A problem occurred when tried to parse the auto answer expected "
                       << "message at index: " << idx;
            qDeleteAll(seqActionsInfo);
            qDeleteAll(tmpSeqActionsInfo);
            qDeleteAll(expectedMsgs);
            return nullptr;
        }

        expectedMsgs.append(expectedMsg);
    }

    return new AutoAnsSequenceInfo(expectedMsgs);
}
