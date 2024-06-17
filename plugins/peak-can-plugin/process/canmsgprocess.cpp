// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "canmsgprocess.hpp"

#include "qtpeakcanlib/src/canmanager.hpp"
#include "qtpeakcanlib/src/models/expectedcanframemask.hpp"

#include "testbedcore/logs/sequencelogformatter.hpp"

#include "models/readandwrite/pcansequenceactioninfo.hpp"
#include "models/readandwrite/pcansequenceinfo.hpp"


CanMsgProcess::CanMsgProcess(QObject *parent)
    : QObject{parent}
{
}

CanMsgProcess::~CanMsgProcess()
{
}

bool CanMsgProcess::process(PCanBusItf::Enum pCanBusIntf,
                            const PCanSequenceInfo &seqActionInfo,
                            bool exitIfVerifFails,
                            QVector<QCanBusFrame> &receivedFrames)
{
    const QSharedPointer<CanDeviceIntf> &deviceIntf = CanManager::getInstance().getCanIntf(
        pCanBusIntf);

    if(deviceIntf == nullptr)
    {
        qWarning() << "Can't process the message sending: there is no CanDeviceIntf linked";
        return false;
    }

    const QString &testGroupName = seqActionInfo.getTestGroupName();
    if(!testGroupName.isEmpty())
    {
        emit newLog(SequenceLogData(LogMsgType::Info,
                                    SequenceLogCategory::BusinessScope,
                                    testGroupName));
    }

    const QVector<const PCanSequenceActionInfo *> &seqActions = seqActionInfo.getSeqActionsInfo();

    for(auto citer = seqActions.cbegin(); citer != seqActions.cend(); ++citer)
    {
        const PCanSequenceActionInfo *seqAction = *citer;

        if(!seqAction->hasCmdToSend() && !seqAction->hasExpectedMessage())
        {
            qInfo() << "In CAN plugin, you don't send any CAN msg and you don't wait for any msg, "
                    << "this is useless and shouldn't exist. May be it's an error?";
        }
        else if(!seqAction->hasCmdToSend())
        {
            QCanBusFrame receivedFrame;
            RETURN_IF_FALSE(processWaitExpectedMsg(deviceIntf,
                                                   *seqAction,
                                                   exitIfVerifFails,
                                                   seqActionInfo.getTimeoutInMs(),
                                                   receivedFrame));

            if(receivedFrame.isValid())
            {
                receivedFrames.append(receivedFrame);
            }
        }
        else
        {
            QCanBusFrame frame;
            seqAction->getCmdBus(deviceIntf->getConfig(), frame);

            if(seqAction->hasExpectedMessage())
            {
                QCanBusFrame receivedFrame;
                RETURN_IF_FALSE(processWriteAndWaitAnswer(deviceIntf,
                                                          frame,
                                                          *seqAction,
                                                          exitIfVerifFails,
                                                          seqActionInfo.getTimeoutInMs(),
                                                          receivedFrame));

                if(receivedFrame.isValid())
                {
                    receivedFrames.append(receivedFrame);
                }
            }
            else
            {
                RETURN_IF_FALSE(deviceIntf->write(frame));
            }
        }
    }

    return true;
}

bool CanMsgProcess::processWriteAndWaitAnswer(const QSharedPointer<CanDeviceIntf> &deviceIntf,
                                              const QCanBusFrame &toSend,
                                              const PCanSequenceActionInfo &seqActionInfo,
                                              bool exitIfVerifFails,
                                              int timeoutInMs,
                                              QCanBusFrame &receivedFrame)
{
    const QVector<QCanBusFrame> expectedAnswers = deviceIntf->writeAndWaitAnswer(
        toSend,
        *seqActionInfo.getExpectedCanFrameMask(),
        timeoutInMs);

    return parseReceivedMsg(expectedAnswers, seqActionInfo, exitIfVerifFails, receivedFrame);
}

bool CanMsgProcess::processWaitExpectedMsg(const QSharedPointer<CanDeviceIntf> &deviceIntf,
                                           const PCanSequenceActionInfo &seqActionInfo,
                                           bool exitIfVerifFails,
                                           int timeoutInMs,
                                           QCanBusFrame &receivedFrame)
{
    const QVector<QCanBusFrame> expectedMsgs = deviceIntf->waitCanMsg(
        *seqActionInfo.getExpectedCanFrameMask(),
        timeoutInMs);

    return parseReceivedMsg(expectedMsgs, seqActionInfo, exitIfVerifFails, receivedFrame);
}

bool CanMsgProcess::parseReceivedMsg(const QVector<QCanBusFrame> &expectedMsgs,
                                     const PCanSequenceActionInfo &seqActionInfo,
                                     bool exitIfVerifFails,
                                     QCanBusFrame &receivedFrame)
{
    if(expectedMsgs.isEmpty() &&
       !seqActionInfo.getExpectedCanFrameMask()->hasToWaitUntilReceivedExpected())
    {
        // A problem occurred
        return false;
    }

    QCanBusFrame answer;

    if(!expectedMsgs.isEmpty())
    {
        answer = expectedMsgs.constFirst();
        receivedFrame = answer;
    }

    auto citer = seqActionInfo.getExpectedMsgData().cbegin();
    for(; citer != seqActionInfo.getExpectedMsgData().cend(); ++citer)
    {
        const PCanExpectedMessage * expectedAnswer = *citer;
        const QString &testNameBusinessLog = expectedAnswer->getTestNameBusinessLog();
        const QString &failBusinessLog = expectedAnswer->getFailBusinessLog();
        const QString &successBusinessLog = expectedAnswer->getSuccessBusinessLog();

        if(!answer.isValid() || !expectedAnswer->checkIfMessageReceivedIsValid(answer))
        {
            if(!testNameBusinessLog.isEmpty() && !failBusinessLog.isEmpty())
            {
                emit newLog(SequenceLogFormatter::formatTestProblem(testNameBusinessLog,
                                                                    failBusinessLog));
            }

            if(exitIfVerifFails)
            {
                return false;
            }
        }
        else if(!testNameBusinessLog.isEmpty() && !successBusinessLog.isEmpty())
        {
            emit newLog(SequenceLogFormatter::formatTest(true,
                                                         testNameBusinessLog,
                                                         successBusinessLog));
        }
    }

    return true;
}
