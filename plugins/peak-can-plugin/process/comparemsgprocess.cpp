// SPDX-FileCopyrightText: 2023 Julien Lemasson <julien.lemasson@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "CompareMsgProcess.hpp"

#include "qtpeakcanlib/src/canmanager.hpp"

#include "testbedcore/logs/sequencelogformatter.hpp"

#include "models/comparemsg/comparecansequenceactioninfo.hpp"
#include "models/comparemsg/comparecansequenceinfo.hpp"

CompareMsgProcess::CompareMsgProcess(QObject *parent)
    : QObject{parent}
{
}

CompareMsgProcess::~CompareMsgProcess()
{
}

bool CompareMsgProcess::process(PCanBusItf::Enum pCanBusIntf,
                                const CompareCanSequenceInfo &seqActionInfo,
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

    const QVector<const CompareCanSequenceActionInfo *> &seqActions =
        seqActionInfo.getSeqActionsInfo();

    for(auto citer = seqActions.cbegin(); citer != seqActions.cend(); ++citer)
    {
        const CompareCanSequenceActionInfo *seqAction = *citer;

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
            receivedFrames.append(receivedFrame);
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
                receivedFrames.append(receivedFrame);
            }
            else
            {
                RETURN_IF_FALSE(deviceIntf->write(frame));
            }
        }
    }

    return true;
}

bool CompareMsgProcess::processWriteAndWaitAnswer(const QSharedPointer<CanDeviceIntf> &deviceIntf,
                                                  const QCanBusFrame &toSend,
                                                  const CompareCanSequenceActionInfo &seqActionInfo,
                                                  bool exitIfVerifFails,
                                                  int timeoutInMs,
                                                  QCanBusFrame &receivedFrame)
{
    const QVector<QCanBusFrame> expectedAnswers =  deviceIntf->writeAndWaitAnswer(
        toSend,
        *seqActionInfo.getExpectedCanFrameMask(),
        timeoutInMs);


    return parseReceivedMsg(expectedAnswers, seqActionInfo, exitIfVerifFails, receivedFrame);
}

bool CompareMsgProcess::processWaitExpectedMsg(const QSharedPointer<CanDeviceIntf> &deviceIntf,
                                               const CompareCanSequenceActionInfo &seqActionInfo,
                                               bool exitIfVerifFails,
                                               int timeoutInMs,
                                               QCanBusFrame &receivedFrame)
{
    const QVector<QCanBusFrame> expectedMsgs =  deviceIntf->waitCanMsg(
        *seqActionInfo.getExpectedCanFrameMask(),
        timeoutInMs);


    return parseReceivedMsg(expectedMsgs, seqActionInfo, exitIfVerifFails, receivedFrame);
}

bool CompareMsgProcess::parseReceivedMsg(const QVector<QCanBusFrame> &expectedMsgs,
                                         const CompareCanSequenceActionInfo &seqActionInfo,
                                         bool exitIfVerifFails,
                                         QCanBusFrame &receivedFrame)
{
    if(expectedMsgs.isEmpty())
    {
        // A problem occurred
        return false;
    }

    const QCanBusFrame &answer = expectedMsgs.constFirst();
    receivedFrame = answer;

    auto citer = seqActionInfo.getExpectedMsgData().cbegin();
    for(; citer != seqActionInfo.getExpectedMsgData().cend(); ++citer)
    {
        const CompareExpectedMsg * expectedAnswer = *citer;
        const QString &testNameBusinessLog = expectedAnswer->getTestNameBusinessLog();

        bool resultAnswer = expectedAnswer->checkIfMessageReceivedIsValid(answer);

        if(!testNameBusinessLog.isEmpty())
        {
            emit newLog(SequenceLogFormatter::formatTest(resultAnswer,
                                              testNameBusinessLog,
                                              expectedAnswer->getCompareValue(),
                                              expectedAnswer->getInfTolerance(),
                                              expectedAnswer->getSupTolerance(),
                                              ""));
        }

        if(exitIfVerifFails && !resultAnswer)
        {
            return false;
        }
    }

    return true;
}
