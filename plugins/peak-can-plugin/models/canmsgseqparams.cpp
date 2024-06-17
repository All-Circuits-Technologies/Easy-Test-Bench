// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "canmsgseqparams.hpp"


CanMsgSeqParams::CanMsgSeqParams(QObject *parent)
    : QObject{parent}
{
}

CanMsgSeqParams::CanMsgSeqParams(const CanMsgSeqParams &otherElement) :
    _testGroupName{otherElement._testGroupName},
    _timeoutInMs{otherElement._timeoutInMs},
    _cmds{otherElement._cmds},
    _hasBitrateSwitch{otherElement._hasBitrateSwitch},
    _isExtendedFrameFormat{otherElement._isExtendedFrameFormat},
    _expectedMsgsIds{otherElement._expectedMsgsIds},
    _verifs{otherElement._verifs}
{
}

CanMsgSeqParams::~CanMsgSeqParams()
{
}

CanMsgSeqParams &CanMsgSeqParams::operator=(const CanMsgSeqParams &otherElement)
{
    _testGroupName = otherElement._testGroupName;
    _timeoutInMs = otherElement._timeoutInMs;
    _cmds = otherElement._cmds;
    _hasBitrateSwitch = otherElement._hasBitrateSwitch;
    _isExtendedFrameFormat = otherElement._isExtendedFrameFormat;
    _expectedMsgsIds = otherElement._expectedMsgsIds;
    _verifs = otherElement._verifs;

    return *this;
}
