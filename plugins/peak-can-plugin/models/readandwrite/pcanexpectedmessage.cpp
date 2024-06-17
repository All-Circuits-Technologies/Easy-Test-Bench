// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "pcanexpectedmessage.hpp"

#include <QCanBusFrame>
#include <QDebug>

#include "qtpeakcanlib/src/models/expectedcanframemask.hpp"

#include "byteutility/bytearrayhelper.hpp"
#include "definesutility/definesutility.hpp"
#include "stringutility/stringhelper.hpp"

#include "peakcanconstants.hpp"


PCanExpectedMessage::PCanExpectedMessage(quint8 msgIdx,
                                         quint8 maskIdx,
                                         const QByteArray &mask,
                                         const QByteArray &expectedMaskResult,
                                         bool waitUntilReceived,
                                         const QString &testNameBusinessLog,
                                         const QString &successBusinessLog,
                                         const QString &failBusinessLog,
                                         QObject *parent)
    : APCanExpectedMsg{maskIdx, mask, expectedMaskResult, parent},
    _msgIdx{msgIdx},
    _waitUntilReceived{waitUntilReceived},
    _testNameBusinessLog{testNameBusinessLog},
    _successBusinessLog{successBusinessLog},
    _failBusinessLog{failBusinessLog}
{
}

PCanExpectedMessage::~PCanExpectedMessage()
{
}

const PCanExpectedMessage *PCanExpectedMessage::parseExpectedMessage(const QString &moduleParameter,
                                                                     bool manageMultipleMsgs)
{
    QStringList parametersParts = moduleParameter.split(PeakCanConstants::Parser::StrSeparator);

    if(!manageMultipleMsgs)
    {
        parametersParts.insert(0, DefaultMsgIdxValue);
    }

    const int partsNb = parametersParts.length();

    if(partsNb == PartsNb)
    {
        return parseExpectedMessageWithoutLogsAndExt(parametersParts);
    }

    if(partsNb == PartsNbWithExtension)
    {
        return parseExpectedMessageWithExtension(parametersParts);
    }

    if(partsNb == PartsNbWithLogs)
    {
        return parseExpectedMessageWithLogs(parametersParts);
    }

    qWarning() << "The PCAN expected message can't be parsed, because it doesn't contain the "
               << "right count of sub elements: " << moduleParameter;

    return nullptr;
}

void PCanExpectedMessage::extractExpectedMessage(
    int msgIdx,
    QVector<const PCanExpectedMessage *> &allMessages,
    QVector<const PCanExpectedMessage *> &expectMsgs)
{
    auto iter = allMessages.begin();
    while(iter != allMessages.end())
    {
        if((*iter)->getMsgIdx() == msgIdx)
        {
            expectMsgs.append(*iter);
            allMessages.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

const ExpectedCanFrameMask *PCanExpectedMessage::createPeakCanMessage(
    quint32 receivedMsgId,
    const QVector<const PCanExpectedMessage *> &expectMsgs)
{
    if(expectMsgs.isEmpty())
    {
        return new ExpectedCanFrameMask(receivedMsgId);
    }

    QVector<quint8> maskIdxList;
    QVector<QByteArray> maskList;
    QVector<QByteArray> finalMaskResults;
    int msgIdx = expectMsgs.first()->getMsgIdx();

    for(auto citer = expectMsgs.cbegin(); citer != expectMsgs.cend(); ++citer)
    {
        const PCanExpectedMessage *expMsg = *citer;

        if(!expMsg->hasToWaitUntilReceivedExpected())
        {
            // It's useless to add the mask in the ExpectedCanFrameMask, because we don't wait for
            // the value
            continue;
        }

        int tmpMsgIdx = expMsg->getMsgIdx();
        if(msgIdx != tmpMsgIdx)
        {
            qWarning() << "A problem occurred: all the expected messages haven't the same message "
                       << "index, expected: " << msgIdx << ", current: "
                       << tmpMsgIdx << ", for received message id: " << receivedMsgId
                       << ", in the createPeakCanMessage function";
            return nullptr;
        }

        const quint8 maskIdx = expMsg->getMaskIdx();
        const QByteArray &mask = expMsg->getMask();
        const QByteArray &expectedMask = expMsg->getExpectedMaskResult();

        maskIdxList.append(maskIdx);
        maskList.append(mask);

        // We remove the non necessary expectedMask bits
        QByteArray realExpectedMask = ByteArrayHelper::operatorAnd(mask, expectedMask);

        if(realExpectedMask != expectedMask)
        {
            qWarning() << "A problem occurred, the expected mask bits: "
                       << ByteArrayHelper::toStringHex(expectedMask) <<  ", overflows the mask "
                       << "covers: " << ByteArrayHelper::toStringHex(mask) << ", for received "
                       << "message id: " << receivedMsgId << ", in the createPeakCanMessage "
                       << "function";
            return nullptr;
        }

        finalMaskResults.append(expectedMask);
    }

    if(ByteArrayHelper::testIfCollidingBits(maskList, maskIdxList))
    {
        qWarning() << "A problem occurred: some masks are overlapping for message id: "
                   << receivedMsgId << ", and message idx: " << expectMsgs.first()->getMsgIdx()
                   << ", in the waiting until received function";
        return nullptr;
    }

    QPair<QByteArray, quint8> newMask = ByteArrayHelper::operatorOr(maskList, maskIdxList);

    QPair<QByteArray, quint8> realExpectedFinal = ByteArrayHelper::operatorOr(finalMaskResults,
                                                                              maskIdxList);

    return new ExpectedCanFrameMask(receivedMsgId,
                                    newMask.second,
                                    newMask.first,
                                    realExpectedFinal.first,
                                    !newMask.first.isEmpty());
}

const PCanExpectedMessage *PCanExpectedMessage::parseExpectedMessageWithoutLogsAndExt(
    const QStringList &parametersParts,
    bool waitUntilReceived,
    const QString &testNameBusinessLog,
    const QString &successBusinessLog,
    const QString &failBusinessLog)
{
    bool ok = false;
    const quint8 cmdIdx = StringHelper::toQuint8(parametersParts[MsgIdxPartIdx], &ok);
    if(!ok)
    {
        qWarning() << "We were trying to parse the can writing expected answer: " << parametersParts
                   << ", but the command index part can't be converted to a quint8";
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
    
    return new PCanExpectedMessage(cmdIdx,
                                   maskIdx,
                                   mask,
                                   expectedMaskResult,
                                   waitUntilReceived,
                                   testNameBusinessLog,
                                   successBusinessLog,
                                   failBusinessLog);
}

const PCanExpectedMessage *PCanExpectedMessage::parseExpectedMessageWithExtension(
    const QStringList &parametersParts,
    const QString &testNameBusinessLog,
    const QString &successBusinessLog,
    const QString &failBusinessLog)
{
    bool ok = false;

    bool waitUntilReceived = StringHelper::toBool(parametersParts[WaitUntilPartIdx], &ok);
    if(!ok)
    {
        qWarning() << "We were trying to parse the can writing expected answer: " << parametersParts
                   << ", but the wait until received part can't be converted to a boolean";
        return nullptr;
    }

    return parseExpectedMessageWithoutLogsAndExt(parametersParts,
                                           waitUntilReceived,
                                           testNameBusinessLog,
                                           successBusinessLog,
                                           failBusinessLog);
}

const PCanExpectedMessage *PCanExpectedMessage::parseExpectedMessageWithLogs(
                                                                const QStringList &parametersParts)
{
    const QString testNameBusinessLog = parametersParts[TestNameLogPartIdx];
    const QString successBusinessLog = parametersParts[SuccessLogPartIdx];
    const QString failBusinessLog = parametersParts[FailLogPartIdx];

    return parseExpectedMessageWithExtension(parametersParts,
                                             testNameBusinessLog,
                                             successBusinessLog,
                                             failBusinessLog);
}
