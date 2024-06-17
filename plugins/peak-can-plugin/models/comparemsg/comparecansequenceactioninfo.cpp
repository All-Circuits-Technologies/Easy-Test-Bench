// SPDX-FileCopyrightText: 2023 Julien Lemasson <julien.lemasson@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "comparecansequenceactioninfo.hpp"

#include <QCanBusFrame>
#include <QDebug>

#include "numberutility/number.hpp"


CompareCanSequenceActionInfo::CompareCanSequenceActionInfo(
    const QVector<const CompareExpectedMsg *> &expectedMsgsVerif,
    const ExpectedCanFrameMask *expectedCanFrameMask,
    QObject *parent)
    : APCanSeqActionInfo{parent},
    _expectedMsgVerifs{expectedMsgsVerif},
    _expectedCanFrameMask{expectedCanFrameMask}
{
}

CompareCanSequenceActionInfo::~CompareCanSequenceActionInfo()
{
    qDeleteAll(_expectedMsgVerifs);
}

const CompareCanSequenceActionInfo *CompareCanSequenceActionInfo::parsePCanSequenceActionInfo(
    const QString &cmd,
    bool hasBitrateSwitch,
    bool isExtendedFrameFormat,
    const QVector<const CompareExpectedMsg *> &expectedMsgVerifs,
    const Number &msgId)
{
    const ExpectedCanFrameMask *expectedCanFrameMask{nullptr};
    if(msgId.isValid())
    {
        expectedCanFrameMask = CompareExpectedMsg::createPeakCanMessage(
            msgId.toUint32(),
            expectedMsgVerifs);

        if(expectedCanFrameMask == nullptr)
        {
            qWarning() << "A problem occurred when tried to create the peak can expected frame "
                       << "mask, from cmd: " << cmd;
            return nullptr;
        }
    }

    CompareCanSequenceActionInfo *action = new CompareCanSequenceActionInfo(expectedMsgVerifs,
                                                                            expectedCanFrameMask);

    if(!action->fillFromParams(cmd, hasBitrateSwitch, isExtendedFrameFormat, msgId))
    {
        delete action;
        return nullptr;
    }
    
    return action;
}
