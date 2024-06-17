// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "apcanexpectedmsg.hpp"

#include <QDebug>
#include <QCanBusFrame>

#include "qtpeakcanlib/src/models/expectedcanframemask.hpp"

#include "byteutility/bytearrayhelper.hpp"
#include "stringutility/stringhelper.hpp"


APCanExpectedMsg::APCanExpectedMsg(quint8 maskIdx,
                                   const QByteArray &mask,
                                   const QByteArray &expectedMaskResult,
                                   QObject *parent)
    : QObject{parent},
    _maskIdx{maskIdx},
    _mask{mask},
    _expectedMaskResult{expectedMaskResult}
{
}

APCanExpectedMsg::~APCanExpectedMsg()
{
}

bool APCanExpectedMsg::checkIfMessageReceivedIsValid(const QCanBusFrame &messageReceived,
                                                     bool silent) const
{
    return ExpectedCanFrameMask::checkMessageReceivedValidity(messageReceived,
                                                              _maskIdx,
                                                              _mask,
                                                              _expectedMaskResult,
                                                              silent);
}

bool APCanExpectedMsg::getInfoFromParameters(const QStringList &parametersParts,
                                             int maskIdxPartIdx,
                                             int maskPartIdx,
                                             int expectedMaskResultPartIdx,
                                             quint8 &maskIdx,
                                             QByteArray &mask,
                                             QByteArray &expectedMaskResult)
{
    bool ok = false;

    maskIdx = StringHelper::toQuint8(parametersParts[maskIdxPartIdx], &ok);
    if(!ok)
    {
        qWarning() << "We were trying to parse the can writing expected answer: " << parametersParts
                   << ", but the mask index part can't be converted to a quint8";
        return false;
    }

    if(!ByteArrayHelper::parseFromHex(parametersParts[maskPartIdx], mask))
    {
        qWarning() << "We were trying to parse the can writing expected answer: " << parametersParts
                   << ", but the mask part can't be parsed";
        return false;
    }

    if(!ByteArrayHelper::parseFromHex(parametersParts[expectedMaskResultPartIdx],
                                       expectedMaskResult))
    {
        qWarning() << "We were trying to parse the can writing expected answer: " << parametersParts
                   << ", but the expected mask result part can't be parsed";
        return false;
    }

    if(expectedMaskResult.length() != mask.length())
    {
        qWarning() << "We were trying to parse the can writing expected answer: " << parametersParts
                   << ", but the expected mask result length is not equal to the mask length";
        return false;
    }

    return true;
}
