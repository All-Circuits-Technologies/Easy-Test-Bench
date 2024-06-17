// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "autoansseriallinkprocess.hpp"

#include <QElapsedTimer>

#include "qtseriallinklib/seriallinkintf.hpp"
#include "qtseriallinklib/seriallinkmanager.hpp"

#include "logsutility/logmsgtype.hpp"
#include "serial-link-lib/processes/messagereceptionprocess.hpp"
#include "testbedcore/logs/sequencelogcategory.hpp"
#include "testbedcore/logs/sequencelogformatter.hpp"
#include "testbedcore/logs/sequencelogdata.hpp"
#include "waitutility/waithelper.hpp"

#include "autoanswer/models/autoansexpectedserialmsg.hpp"
#include "autoanswer/models/autoansmsgtosend.hpp"
#include "autoanswer/models/autoanswerparams.hpp"


AutoAnsSerialLinkProcess::AutoAnsSerialLinkProcess(QObject *parent)
    : QObject{parent}, MixinProcessStats{StatsGlobalInfo}
{
}

AutoAnsSerialLinkProcess::~AutoAnsSerialLinkProcess()
{
    delete _autoAnsParams;
    delete _receptionProcess;
    _autoAnsParams = nullptr;
    _receptionProcess = nullptr;
}

bool AutoAnsSerialLinkProcess::initProcess(const QString &interfaceName,
                                           const AutoAnswerParams *params)
{
    if(!_serialIntf.isNull())
    {
        qInfo() << "The serial echo process has already been initialized";
        return true;
    }

    if(params == nullptr)
    {
        qWarning() << "The auto answer params given is null, we can't initialize the process";
        return false;
    }

    QSharedPointer<SerialLinkIntf> serialIntf = SerialLinkManager::getInstance()
                                                    .getSerialLink(interfaceName);

    if(serialIntf == nullptr)
    {
        qWarning() << "Can't init the serial echo process, we can't get the wanted interface";
        return false;
    }

    _serialIntf = serialIntf;
    delete _autoAnsParams;
    delete _receptionProcess;
    _autoAnsParams = params;
    _receptionProcess = new MessageReceptionProcess(params->getEndOfLine(),
                                                    params->isTrim(),
                                                    params->isMultiLineTest());

    if(params->isDisplayingStats())
    {
        _msgSentStats = &registerCounter(SentStatsCounterKey, SentStatsTitle);
        _msgReceivedStats = &registerCounter(ReceivedStatsCounterKey, ReceivedStatsTitle);
    }
    else
    {
        _msgSentStats = nullptr;
        _msgReceivedStats = nullptr;
    }

    return true;
}

bool AutoAnsSerialLinkProcess::manageEchoing(bool start)
{
    if(start)
    {
        return startEchoing();
    }

    return stopEchoing();
}

bool AutoAnsSerialLinkProcess::startEchoing()
{
    if(_serialIntf.isNull())
    {
        qWarning() << "Can't start echoing, the serial interface is null";
        return false;
    }

    if(_connect)
    {
        qInfo() << "The process is already echoing";
        return true;
    }

    RETURN_IF_FALSE(openSerialPort());

    _pause = false;

    emit newLog(SequenceLogData(LogMsgType::Info,
                                SequenceLogCategory::BusinessScope,
                                QString("Start the auto answer process, for interface: %1")
                                    .arg(_serialIntf->getIntfName())));

    if(_msgSentStats != nullptr)
    {
        _msgSentStats->clear();
        _msgReceivedStats->clear();
    }

    _connect = connect(_serialIntf.data(), &SerialLinkIntf::dataReceived,
                       this,               &AutoAnsSerialLinkProcess::onMessageReceived);

    return true;
}

bool AutoAnsSerialLinkProcess::stopEchoing()
{
    if(_serialIntf.isNull())
    {
        qWarning() << "Can't stop echoing, the serial interface is null";
        return false;
    }

    if(!_connect)
    {
        qInfo() << "The process is already stopped, it isn't echoing";
        return true;
    }

    _pause = false;

    emit newLog(SequenceLogData(LogMsgType::Info,
                                SequenceLogCategory::BusinessScope,
                                QString("Stop the auto answer process, for interface: %1")
                                    .arg(_serialIntf->getIntfName())));

    disconnect(_connect);
    closeSerialPort();

    _pause = false;

    if(_msgSentStats != nullptr)
    {
        emit newLog(SequenceLogData(LogMsgType::Info,
                                    SequenceLogCategory::BusinessScope,
                                    formatStatsToBeDisplayedInLogs()));
    }

    return true;
}

void AutoAnsSerialLinkProcess::onMessageReceived(const QByteArray &message)
{
    if((_serialIntf == nullptr) || (_autoAnsParams == nullptr) || _pause)
    {
        // Useless to try to read PING message, because we can't send the PONG
        // Or we are in pause
        return;
    }

    if(!_receptionProcess->manageReceivedLines(message, _receivedLines, _tmpBuffer))
    {
        // Nothing important has changed, useless to test all
        return;
    }

    QElapsedTimer msgReceptionTimer;
    msgReceptionTimer.start();
    QVector<AutoAnsMsgToSend*> msgsToSend;

    const QVector<AutoAnsExpectedSerialMsg *> &expMsgs = _autoAnsParams->getExpectedMessages();
    for(auto citer = expMsgs.cbegin(); citer != expMsgs.cend(); ++citer)
    {
        AutoAnsExpectedSerialMsg *autoAnsExpSerial = *citer;
        const QRegularExpression &expectedMsgRegExp = *autoAnsExpSerial->getExpectedMsg();

        QVector<QString> captElements;
        SerialLinkReceivedReplyType::Enum replyType =
            _receptionProcess->manageReceivedMessage(
                expectedMsgRegExp,
                _receivedLines,
                _tmpBuffer,
                captElements,
                SerialPluginLibConstants::Defaults::DefaultExpectedFailure,
                &autoAnsExpSerial->accessMultiLineStartIndex());

        if(replyType != SerialLinkReceivedReplyType::ExpectedAnswer)
        {
            // Nothing found, we continue
            continue;
        }

        QString pattern = expectedMsgRegExp.pattern();

        updateMsgStats(pattern, _msgReceivedStats);

        msgsToSend.append(autoAnsExpSerial->getMsgsToSend());
    }

    clearReceivedLinesAfterMessageReception();

    if(!sendMsgsLinkedToExpectedMsgs(msgReceptionTimer, msgsToSend))
    {
        qWarning() << "A problem occurred when tried to send an auto message after having received "
                   << "frames";
    }

}

void AutoAnsSerialLinkProcess::updateMsgStats(const QString &msg, CounterStatsCategory *stats)
{
    if(stats == nullptr)
    {
        // Nothing has to be done
        return;
    }

    CounterStatsInfo &counter = stats->getOrCreateValue(msg, new CounterStatsInfo());
    counter.incrementByOneValue();
}

bool AutoAnsSerialLinkProcess::sendMsgsLinkedToExpectedMsgs(
    const QElapsedTimer &msgReceptElapsTimer,
    QVector<AutoAnsMsgToSend*> &msgsToSend)
{
    // First we sort the time to wait from the less to greather
    std::sort(msgsToSend.begin(), msgsToSend.end(), [](AutoAnsMsgToSend* a, AutoAnsMsgToSend* b) {
        return a->getTimeToWaitBeforeSending() < b->getTimeToWaitBeforeSending();
    });

    for(auto citer = msgsToSend.cbegin(); citer != msgsToSend.cend(); ++citer)
    {
        AutoAnsMsgToSend* msgToSend = (*citer);
        // We add the end of line to the message to send
        QString msgToSendStr = msgToSend->getMsgToSend() + _autoAnsParams->getEndOfLine();

        int timeout = msgToSend->getTimeToWaitBeforeSending() - msgReceptElapsTimer.elapsed();

        if(timeout > 0)
        {
            WaitHelper::pseudoSleep(timeout);
        }

        if(!_serialIntf->send(msgToSendStr.toLatin1(), _autoAnsParams->isForcingFlush()))
        {
            qWarning() << "A problem occured when tried to automatically send the "
                       << "message: " << msgToSend;
            return false;
        }

        updateMsgStats(msgToSendStr, _msgSentStats);
    }

    return true;
}

bool AutoAnsSerialLinkProcess::openSerialPort()
{
    if(!_serialIntf->isOpen())
    {
        if(!_serialIntf->open(QIODevice::ReadWrite))
        {
            const QString linkName = _serialIntf->getIntfName();
            emit newLog(SequenceLogFormatter::formatABusinessProblem(tr("(TR) Serial port %1 is "
                                                                        "not yet set up")
                                                                         .arg(linkName)));
            qWarning() << "A problem occured when tryied to open the port: " << linkName;
            return false;
        }

        _serialPortOpenedByMe = true;
    }
    else
    {
        _serialPortOpenedByMe = false;
    }

    return true;
}

void AutoAnsSerialLinkProcess::closeSerialPort()
{
    if(_serialPortOpenedByMe)
    {
        _serialIntf->close();
    }
}

void AutoAnsSerialLinkProcess::clearReceivedLinesAfterMessageReception()
{
    if(!_autoAnsParams->isMultiLineTest())
    {
        // In the case we aren't managing multi lines we don't need to keep all the received lines
        _receivedLines.clear();
        return;
    }

    // First, we guess the minimum start index for all the expected serial message
    int minStartIdx = -1;
    const QVector<AutoAnsExpectedSerialMsg *> &expMsgs = _autoAnsParams->getExpectedMessages();
    for(auto citer = expMsgs.cbegin(); citer != expMsgs.cend(); ++citer)
    {
        int startIdx = (*citer)->accessMultiLineStartIndex();
        if(minStartIdx == -1 || (*citer)->accessMultiLineStartIndex() < minStartIdx)
        {
            minStartIdx = startIdx;
        }
    }

    if(minStartIdx == 0)
    {
        // Nothing has to be done
        return;
    }

    // Second, we guess the new base index for all the expected serial messages and we guess the
    // lines to remove
    int newBaseIdx = 0;
    auto iter = _receivedLines.begin();
    for(; iter != _receivedLines.end(); ++iter)
    {
        int tmpBaseIdx = newBaseIdx + iter->length();

        if(tmpBaseIdx > minStartIdx)
        {
            // We overflowed the min start index, we stop here
            break;
        }

        newBaseIdx = tmpBaseIdx;
    }

    if(iter == _receivedLines.begin())
    {
        // Nothing more has to be done
        return;
    }

    // Third, we remove the used lines and set the start index for all the expected messages
    _receivedLines.erase(_receivedLines.begin(), iter);

    for(auto citer = expMsgs.cbegin(); citer != expMsgs.cend(); ++citer)
    {
        (*citer)->accessMultiLineStartIndex() = (*citer)->accessMultiLineStartIndex() - newBaseIdx;
    }
}
