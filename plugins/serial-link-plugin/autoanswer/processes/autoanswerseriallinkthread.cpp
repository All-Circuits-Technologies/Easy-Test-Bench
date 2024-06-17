// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "autoanswerseriallinkthread.hpp"

#include <QTimer>

#include "definesutility/definesutility.hpp"
#include "threadutility/concurrent/threadconcurrentrun.hpp"

#include "autoanswer/models/autoanswerparams.hpp"
#include "autoanswer/processes/autoansseriallinkprocess.hpp"


AutoAnswerSerialLinkThread::AutoAnswerSerialLinkThread(QObject *parent)
    : BaseThread{parent}
{
}

AutoAnswerSerialLinkThread::~AutoAnswerSerialLinkThread()
{
}

bool AutoAnswerSerialLinkThread::initProcess(const QString &portName,
                                             const AutoAnswerParams *autoAnsParams)
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
                                    &AutoAnsSerialLinkProcess::initProcess,
                                    portName,
                                    autoAnsParams);
}

bool AutoAnswerSerialLinkThread::manageEchoing(bool start)
{
    RETURN_IF_FALSE(waitForThread());

    if(_process == nullptr)
    {
        qWarning() << "We can't manage the auto answer echoing, the process is null";
        return false;
    }

    return ThreadConcurrentRun::run(*_process, &AutoAnsSerialLinkProcess::manageEchoing, start);
}

bool AutoAnswerSerialLinkThread::startEchoing()
{
    RETURN_IF_FALSE(waitForThread());

    if(_process == nullptr)
    {
        qWarning() << "We can't start the auto answer echoing, the process is null";
        return false;
    }

    return ThreadConcurrentRun::run(*_process, &AutoAnsSerialLinkProcess::startEchoing);
}

bool AutoAnswerSerialLinkThread::stopEchoing()
{
    RETURN_IF_FALSE(waitForThread());

    if(_process == nullptr)
    {
        qWarning() << "We can't stop the auto answer echoing, the process is null";
        return false;
    }

    return ThreadConcurrentRun::run(*_process, &AutoAnsSerialLinkProcess::stopEchoing);
}

bool AutoAnswerSerialLinkThread::manageEchoingPause(bool pause)
{
    RETURN_IF_FALSE(waitForThread());

    if(_process == nullptr)
    {
        qWarning() << "We can't pause the auto answer echoing, the process is null";
        return false;
    }

    ThreadConcurrentRun::run(*_process, &AutoAnsSerialLinkProcess::manageEchoingPause, pause);
    return true;
}

bool AutoAnswerSerialLinkThread::stopThread()
{
    if(_process != nullptr)
    {
        QTimer::singleShot(0, _process, &AutoAnsSerialLinkProcess::deleteLater);
        _process = nullptr;
    }

    return BaseThread::stopThread();
}

void AutoAnswerSerialLinkThread::run()
{
    _process = new AutoAnsSerialLinkProcess();

    connect(_process, &AutoAnsSerialLinkProcess::newLog, this, &AutoAnswerSerialLinkThread::newLog);

    BaseThread::run();
}
