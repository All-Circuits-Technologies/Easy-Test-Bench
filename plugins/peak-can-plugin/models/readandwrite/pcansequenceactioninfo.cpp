// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "pcansequenceactioninfo.hpp"

#include <QCanBusFrame>
#include <QDebug>

#include "qtpeakcanlib/src/models/expectedcanframemask.hpp"

#include "numberutility/number.hpp"


PCanSequenceActionInfo::PCanSequenceActionInfo(
    const QVector<const PCanExpectedMessage *> &expectedMsgsVerif,
    const ExpectedCanFrameMask *expectedCanFrameMask,
    QObject *parent)
    : APCanSeqActionInfo{parent},
    _expectedMsgVerifs{expectedMsgsVerif},
    _expectedCanFrameMask{expectedCanFrameMask}
{
}

PCanSequenceActionInfo::~PCanSequenceActionInfo()
{
    qDeleteAll(_expectedMsgVerifs);
}

const PCanSequenceActionInfo *PCanSequenceActionInfo::parsePCanSequenceActionInfo(
    const QString &cmd,
    bool hasBitrateSwitch,
    bool isExtendedFrameFormat,
    const QVector<const PCanExpectedMessage *> &expectedMsgVerifs,
    const Number &msgId)
{
    const ExpectedCanFrameMask *expectedCanFrameMask{nullptr};
    if(msgId.isValid())
    {
        expectedCanFrameMask = PCanExpectedMessage::createPeakCanMessage(
            msgId.toUint32(),
            expectedMsgVerifs);

        if(expectedCanFrameMask == nullptr)
        {
            qWarning() << "A problem occurred when tried to create the peak can expected frame "
                       << "mask, from cmd: " << cmd;
            return nullptr;
        }
    }

    PCanSequenceActionInfo *action = new PCanSequenceActionInfo(expectedMsgVerifs,
                                                                expectedCanFrameMask);

    if(!action->fillFromParams(cmd, hasBitrateSwitch, isExtendedFrameFormat, msgId))
    {
        delete action;
        return nullptr;
    }
    
    return action;
}
