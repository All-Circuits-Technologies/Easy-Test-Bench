// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "apcanseqactioninfo.hpp"

#include <QCanBusFrame>
#include <QDebug>

#include "qtpeakcanlib/src/models/candeviceconfig.hpp"

#include "canutility/canbusframehelper.hpp"
#include "numberutility/number.hpp"


APCanSeqActionInfo::APCanSeqActionInfo(QObject *parent)
    : QObject{parent}
{
}

APCanSeqActionInfo::~APCanSeqActionInfo()
{
    delete _cmdBus;
}

void APCanSeqActionInfo::getCmdBus(const CanDeviceConfig &config, QCanBusFrame &frame) const
{
    CanBusFrameHelper::copyCanBusFrame(*_cmdBus, frame);

    frame.setFlexibleDataRateFormat(config.isCanFd());
}

bool APCanSeqActionInfo::hasCmdToSend() const
{
    return (_cmdBus != nullptr);
}

bool APCanSeqActionInfo::hasExpectedMessage() const
{
    return _expectedMsgId != std::numeric_limits<quint32>::max();
}

bool APCanSeqActionInfo::fillFromParams(const QString &cmd,
                                        bool hasBitrateSwitch,
                                        bool isExtendedFrameFormat,
                                        const Number &msgId)
{
    QCanBusFrame *frame = nullptr;

    if(!cmd.isEmpty())
    {
        frame = new QCanBusFrame();
        if(!CanBusFrameHelper::parseStrFrame(cmd, *frame))
        {
            delete frame;
            return false;
        }

        frame->setExtendedFrameFormat(isExtendedFrameFormat);
        frame->setBitrateSwitch(hasBitrateSwitch);
    }

    _cmdBus = frame;

    if(msgId.isValid())
    {
        _expectedMsgId = msgId.toUint32();
    }

    return true;
}
