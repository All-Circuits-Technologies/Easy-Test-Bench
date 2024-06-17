// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "autoanswerpeakcanmanager.hpp"

#include "bankcoreshared/usebylib/isequenceinstancemodule.hpp"

#include "process/autoansprocessthread.hpp"
#include "process/autoansthreadhandler.hpp"


AutoAnswerPeakCanManager::AutoAnswerPeakCanManager(QObject *parent)
    : QObject{parent}
{
}

AutoAnswerPeakCanManager::~AutoAnswerPeakCanManager()
{
}

QSharedPointer<AutoAnsThreadHandler> AutoAnswerPeakCanManager::createOrGetEchoProcess(
    PCanBusItf::Enum peakCanIntf,
    const CanMsgSeqParams &msgSeqParams,
    ISequenceInstanceModule &caller,
    bool manageStats)
{
    auto createAutoAnswerProcess = [this, &msgSeqParams, &caller, &manageStats]
        (const PCanBusItf::Enum &key)
    {
        AutoAnsThreadHandler *autoAnswerHandler = new AutoAnsThreadHandler();
        AutoAnsProcessThread *autoAnswerProcess = autoAnswerHandler->accessThread();

        if(!autoAnswerProcess->initProcess(key, msgSeqParams, manageStats))
        {
            qWarning() << "A problem occurred when tried to initialize the auto answer process: "
                       << PCanBusItf::toString(key);
            delete autoAnswerHandler;

            return QSharedPointer<AutoAnsThreadHandler>();
        }

        this->connect(autoAnswerProcess, &AutoAnsProcessThread::newLog,
                      &caller,           &ISequenceInstanceModule::newLog);

        return QSharedPointer<AutoAnsThreadHandler>(autoAnswerHandler);
    };

    return createOrGetHandler(peakCanIntf, createAutoAnswerProcess);
}

QSharedPointer<AutoAnsThreadHandler> AutoAnswerPeakCanManager::getEchoProcess(
    PCanBusItf::Enum peakCanIntf)
{
    return getHandler(peakCanIntf);
}
