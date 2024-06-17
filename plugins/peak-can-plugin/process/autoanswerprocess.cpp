// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "autoanswerprocess.hpp"

#include <QDebug>

#include "qtpeakcanlib/src/candevice/candeviceintf.hpp"
#include "qtpeakcanlib/src/canmanager.hpp"

#include "canutility/canbusframehelper.hpp"
#include "collectionutility/qvectorhelper.hpp"
#include "definesutility/definesutility.hpp"
#include "logsutility/logmsgtype.hpp"
#include "testbedcore/logs/sequencelogcategory.hpp"
#include "testbedcore/logs/sequencelogdata.hpp"

#include "models/autoanswer/autoanssequenceinfo.hpp"
#include "models/autoanswer/autoanssequactioninfo.hpp"
#include "models/autoanswer/autoansexpectedmessage.hpp"


AutoAnswerProcess::AutoAnswerProcess(QObject *parent)
    : QObject{parent}, MixinProcessStats{StatsGlobalInfo}
{
}

AutoAnswerProcess::~AutoAnswerProcess()
{
    delete _sequenceInfo;
    _sequenceInfo = nullptr;
}

bool AutoAnswerProcess::initProcess(PCanBusItf::Enum canIntfKey,
                                    const CanMsgSeqParams &msgSeqParams,
                                    bool manageStats)
{
    if(!_canIntf.isNull())
    {
        qInfo() << "The echo process has already been initialized";
        return true;
    }

    QSharedPointer<CanDeviceIntf> deviceIntf = CanManager::getInstance().getCanIntf(canIntfKey);

    if(deviceIntf == nullptr)
    {
        qWarning() << "Can't init the echo process, we can't get the wanted interface";
        return false;
    }

    const AutoAnsSequenceInfo *sequenceInfo = AutoAnsSequenceInfo::parsePCanSequenceInfo(
        msgSeqParams);

    if(sequenceInfo == nullptr)
    {
        return false;
    }

    _canIntf = deviceIntf;
    delete _sequenceInfo;
    _sequenceInfo = sequenceInfo;

    if(manageStats)
    {
        _cmdSentStats = &registerCounter(SentStatsCounterKey, SentStatsTitle);
        _cmdReceivedStats = &registerCounter(ReceivedStatsCounterKey, ReceivedStatsTitle);
    }
    else
    {
        _cmdSentStats = nullptr;
        _cmdReceivedStats = nullptr;
    }

    return true;
}

bool AutoAnswerProcess::manageEchoing(bool start)
{
    if(start)
    {
        return startEchoing();
    }

    return stopEchoing();
}

bool AutoAnswerProcess::startEchoing()
{
    if(_canIntf.isNull())
    {
        qWarning() << "Can't start echoing, the can interface is null";
        return false;
    }

    if(_connect)
    {
        qInfo() << "The process is already echoing";
        return true;
    }

    _pause = false;

    emit newLog(SequenceLogData(LogMsgType::Info,
                                SequenceLogCategory::BusinessScope,
                                QString("Start the auto answer process, for interface: %1")
                                    .arg(PCanBusItf::toString(_canIntf->getCanIntfKey()))));

    if(_cmdSentStats != nullptr)
    {
        _cmdSentStats->clear();
        _cmdReceivedStats->clear();
    }

    _connect = connect(_canIntf.data(), &CanDeviceIntf::framesReceived,
                       this,            &AutoAnswerProcess::onFramesReceived);

    return true;
}

bool AutoAnswerProcess::stopEchoing()
{
    if(_canIntf.isNull())
    {
        qWarning() << "Can't stop echoing, the can interface is null";
        return false;
    }

    if(!_connect)
    {
        qInfo() << "The process is already stopped, it isn't echoing";
        return true;
    }

    emit newLog(SequenceLogData(LogMsgType::Info,
                                SequenceLogCategory::BusinessScope,
                                QString("Stop the auto answer process, for interface: %1")
                                    .arg(PCanBusItf::toString(_canIntf->getCanIntfKey()))));

    disconnect(_connect);

    _pause = false;

    emitStatsLogs();

    return true;
}

void AutoAnswerProcess::onFramesReceived(const QVector<QCanBusFrame> &frames)
{
    if((_canIntf == nullptr) || (_sequenceInfo == nullptr) || _pause)
    {
        // Useless to try to read PING message, because we can't send the PONG
        // Or we are in pause
        return;
    }

    for(auto citer = frames.cbegin(); citer != frames.cend(); ++citer)
    {
        updateCmdStats(*citer, _cmdReceivedStats);

        const QVector<const AutoAnsExpectedMessage*> expectedMsgs =
            _sequenceInfo->getMatchedElements(*citer);

        if(!sendCmdsLinkedToExpectedMsgs(expectedMsgs))
        {
            qWarning() << "A problem occurred when tried to send an auto command after having "
                       << "received the frame: " << citer->toString();
        }
    }
}

void AutoAnswerProcess::updateCmdStats(const QCanBusFrame &cmd, CounterStatsCategory *stats)
{
    if(stats == nullptr)
    {
        // Nothing has to be done
        return;
    }

    QString strCmd = CanBusFrameHelper::stringifyFrame(cmd);
    
    CounterStatsInfo &counter = stats->getOrCreateValue(strCmd, new CounterStatsInfo());
    counter.incrementByOneValue();
}

void AutoAnswerProcess::emitStatsLogs()
{
    if(_cmdSentStats == nullptr)
    {
        // Nothing has to be done
        return;
    }

    emit newLog(SequenceLogData(LogMsgType::Info,
                                SequenceLogCategory::BusinessScope,
                                formatStatsToBeDisplayedInLogs()));
}

bool AutoAnswerProcess::sendCmdsLinkedToExpectedMsgs(
    const QVector<const AutoAnsExpectedMessage *> &expectedMsgs)
{
    const CanDeviceConfig &config = _canIntf->getConfig();
    for(auto citer = expectedMsgs.cbegin(); citer != expectedMsgs.cend(); ++citer)
    {
        const QVector<const AutoAnsSequActionInfo *> &seqActions = (*citer)->getSeqActions();
        for(auto cmdIter = seqActions.cbegin(); cmdIter != seqActions.cend(); ++cmdIter)
        {
            QCanBusFrame frame;
            (*cmdIter)->getCmdBus(config, frame);

            // We have to send a message
            if(!_canIntf->write(frame))
            {
                qWarning() << "A problem occured when tried to automatically send the "
                           << "message: " << frame.toString();
                return false;
            }

            updateCmdStats(frame, _cmdSentStats);
        }
    }

    return true;
}
