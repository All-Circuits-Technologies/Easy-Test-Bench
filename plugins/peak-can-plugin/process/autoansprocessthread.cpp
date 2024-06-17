// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "autoansprocessthread.hpp"

#include <QTimer>

#include "definesutility/definesutility.hpp"
#include "threadutility/concurrent/threadconcurrentrun.hpp"

#include "models/canmsgseqparams.hpp"
#include "process/autoanswerprocess.hpp"


AutoAnsProcessThread::AutoAnsProcessThread(QObject *parent)
    : BaseThread{parent}
{
}

AutoAnsProcessThread::~AutoAnsProcessThread()
{
}

bool AutoAnsProcessThread::initProcess(PCanBusItf::Enum canIntfKey,
                                       const CanMsgSeqParams &msgSeqParams,
                                       bool manageStats)
{
    if(_process == nullptr)
    {
        RETURN_IF_FALSE(startThreadAndWaitToBeReady());
    }
    else
    {
        RETURN_IF_FALSE(waitForThread());
    }

    if(_process == nullptr)
    {
        qWarning() << "We can't init the auto answer process, the process is null";
        return false;
    }

    return ThreadConcurrentRun::run(*_process,
                                    &AutoAnswerProcess::initProcess,
                                    canIntfKey,
                                    msgSeqParams,
                                    manageStats);
}

bool AutoAnsProcessThread::manageEchoing(bool start)
{
    RETURN_IF_FALSE(waitForThread());

    if(_process == nullptr)
    {
        qWarning() << "We can't manage the auto answer echoing, the process is null";
        return false;
    }

    return ThreadConcurrentRun::run(*_process, &AutoAnswerProcess::manageEchoing, start);
}

bool AutoAnsProcessThread::startEchoing()
{
    RETURN_IF_FALSE(waitForThread());

    if(_process == nullptr)
    {
        qWarning() << "We can't start the auto answer echoing, the process is null";
        return false;
    }

    return ThreadConcurrentRun::run(*_process, &AutoAnswerProcess::startEchoing);
}

bool AutoAnsProcessThread::stopEchoing()
{
    RETURN_IF_FALSE(waitForThread());

    if(_process == nullptr)
    {
        qWarning() << "We can't stop the auto answer echoing, the process is null";
        return false;
    }

    return ThreadConcurrentRun::run(*_process, &AutoAnswerProcess::stopEchoing);
}

bool AutoAnsProcessThread::manageEchoingPause(bool pause)
{
    RETURN_IF_FALSE(waitForThread());

    if(_process == nullptr)
    {
        qWarning() << "We can't pause the auto answer echoing, the process is null";
        return false;
    }

    ThreadConcurrentRun::run(*_process, &AutoAnswerProcess::manageEchoingPause, pause);
    return true;
}

bool AutoAnsProcessThread::stopThread()
{
    if(_process != nullptr)
    {
        QTimer::singleShot(0, _process, &AutoAnswerProcess::deleteLater);
        _process = nullptr;
    }

    return BaseThread::stopThread();
}

void AutoAnsProcessThread::run()
{
    _process = new AutoAnswerProcess();

    connect(_process, &AutoAnswerProcess::newLog, this, &AutoAnsProcessThread::newLog);

    BaseThread::run();
}
