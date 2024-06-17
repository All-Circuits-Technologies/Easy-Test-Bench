// SPDX-FileCopyrightText: 2023 Julien Lemasson <julien.lemasson@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "compareexpectedmsg.hpp"

#include <QCanBusFrame>
#include <QDebug>

#include "qtpeakcanlib/src/models/expectedcanframemask.hpp"

#include "byteutility/bytearrayhelper.hpp"
#include "byteutility/bytearrayhelper.hpp"
#include "stringutility/stringhelper.hpp"

#include "peakcanconstants.hpp"


CompareExpectedMsg::CompareExpectedMsg(quint8 msgIdx,
                                       quint8 maskIdx,
                                       const QByteArray &mask,
                                       bool isLsb,
                                       const QByteArray &expectedMaskResult,
                                       bool waitUntilReceived,
                                       const QString &testNameBusinessLog,
                                       const Number infTolerance,
                                       const Number supTolerance,
                                       QObject *parent)
    : APCanExpectedMsg{maskIdx, mask, expectedMaskResult, parent},
    _msgIdx{msgIdx},
    _waitUntilReceived{waitUntilReceived},
    _maskIdx{maskIdx},
    _mask{mask},
    _isLsb{isLsb},
    _testNameBusinessLog{testNameBusinessLog},
    _infTolerance{infTolerance},
    _supTolerance{supTolerance}
{
}

CompareExpectedMsg::~CompareExpectedMsg()
{
}

const CompareExpectedMsg *CompareExpectedMsg::parseExpectedMessage(const QString &moduleParameter,
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

void CompareExpectedMsg::extractExpectedMessage(int msgIdx,
                                                QVector<const CompareExpectedMsg *> &allMessages,
                                                QVector<const CompareExpectedMsg *> &expectMsgs)
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

const bool CompareExpectedMsg::parseFromParameters(const QStringList &parametersParts,
                                                   quint8 &maskIdx,
                                                   QByteArray &mask,
                                                   bool &isLsb,
                                                   Number &infTolerance,
                                                   Number &supTolerance)
{
    bool ok = false;

    maskIdx = StringHelper::toQuint8(parametersParts[MaskIdxPartIdx], &ok);
    if(!ok)
    {
        qWarning() << "We were trying to parse the can writing expected answer: " << parametersParts
                   << ", but the mask index part can't be converted to a quint8";
        return false;
    }

    if(!ByteArrayHelper::parseFromHex(parametersParts[MaskPartIdx], mask))
    {
        qWarning() << "We were trying to parse the can writing expected answer: " << parametersParts
                   << ", but the mask part can't be parsed";
        return false;
    }

    isLsb = StringHelper::toBool(parametersParts[IsLsbFirstPartIdx], &ok);
    if(!ok)
    {
        qWarning() << "We were trying to parse the can writing expected answer: " << parametersParts
                   << ", but the is lsb part can't be converted to a boolean";
        return false;
    }

    infTolerance = Number::fromString(parametersParts[InfTolerancePartIdx]);
    supTolerance = Number::fromString(parametersParts[SupTolerancePartIdx]);

    return true;
}

const ExpectedCanFrameMask *CompareExpectedMsg::createPeakCanMessage(
    quint32 receivedMsgId,
    const QVector<const CompareExpectedMsg *> &expectMsgs)
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
        const CompareExpectedMsg *expMsg = *citer;

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

const CompareExpectedMsg *CompareExpectedMsg::parseExpectedMessageWithoutLogsAndExt(
    const QStringList &parametersParts,
    bool waitUntilReceived,
    const QString &testNameBusinessLog)
{
    bool ok = false;
    const quint8 msgIdx = StringHelper::toQuint8(parametersParts[MsgIdxPartIdx], &ok);
    if(!ok)
    {
        qWarning() << "We were trying to parse the can writing expected answer: " << parametersParts
                   << ", but the command index part can't be converted to a quint8";
        return nullptr;
    }

    quint8 maskIdx = 0;
    QByteArray mask;
    bool isLsb;
    QByteArray expectedMaskResult;
    Number intTolerance;
    Number supTolerance;


    if(!parseFromParameters(parametersParts,
                            maskIdx,
                            mask,
                            isLsb,
                            intTolerance,
                            supTolerance))
    {
        return nullptr;
    }

    return new CompareExpectedMsg(msgIdx,
                                  maskIdx,
                                  mask,
                                  isLsb,
                                  expectedMaskResult,
                                  waitUntilReceived,
                                  testNameBusinessLog,
                                  intTolerance,
                                  supTolerance);
}

const CompareExpectedMsg *CompareExpectedMsg::parseExpectedMessageWithExtension(
    const QStringList &parametersParts,
    const QString &testNameBusinessLog)
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
                                                 testNameBusinessLog);
}

const CompareExpectedMsg *CompareExpectedMsg::parseExpectedMessageWithLogs(
    const QStringList &parametersParts)
{
    const QString testNameBusinessLog = parametersParts[TestNameLogPartIdx];

    return parseExpectedMessageWithExtension(parametersParts,
                                             testNameBusinessLog);
}

bool CompareExpectedMsg::checkIfMessageReceivedIsValid(const QCanBusFrame &messageReceived,
                                                       bool silent) const
{
    const QByteArray receivedPayload = messageReceived.payload();
    const int expectedAnswerLengthFromMask = _maskIdx + _mask.length();

    // First check the size of the payload, with the mask size
    if(receivedPayload.length() < expectedAnswerLengthFromMask)
    {
        if(!silent)
        {
            qWarning() << "The expected mask: " << _mask << ", at idx: " << _maskIdx << ", isn't "
                       << "contained in the CAN answer received: " << messageReceived.toString();
        }

        return false;
    }

    QByteArray expectedResult{_mask.length(), 0};
    int charIdx = 0;

    for(int idx = _maskIdx; idx < expectedAnswerLengthFromMask; ++idx)
    {
        const char payloadChar = receivedPayload.at(idx);
        const char maskChar = _mask.at(idx - _maskIdx);

        expectedResult[charIdx] = (payloadChar & maskChar);
        charIdx++;
    }

    _compareValue = Number::fromHex(expectedResult, _isLsb);
    if(!_compareValue.isValid() || (_compareValue > _supTolerance || _compareValue < _infTolerance))
    {
        if(!silent)
        {
            if(!_compareValue.isValid())
            {
                qWarning() << "Error while triying to parse received value on CAN message "
                           << "for compare";
            }
            else
            {
                qWarning() << "The value received from the CAN message is outside the specified "
                           << "limits: " << _infTolerance << " < value > " << _supTolerance;
            }
        }

        return false;
    }

    return true;
}
