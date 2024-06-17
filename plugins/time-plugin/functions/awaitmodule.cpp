// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "awaitmodule.hpp"

#include "bankglobalmanager.hpp"
#include "testbedcore/testbedglobal.hpp"
#include "numberutility/number.hpp"
#include "testbedcore/utility/pauseandstophelper.hpp"


AWaitModule::AWaitModule(const QString &moduleClassName,
                         QObject *parent) :
    DefaultSequenceInstanceModule(moduleClassName,
                                  BankGlobalManager::Instance(),
                                  parent)
{
    _timer.setSingleShot(true);
    _timer.setTimerType(Qt::PreciseTimer);

    connect(&_timer, &QTimer::timeout,
            this,    &AWaitModule::onTimeout);

    connect(&getSequenceState(), &SequenceStateHelper::newState,
            this,                &AWaitModule::onNewModuleState);
    connect(&getSequenceState(), &SequenceStateHelper::stopAsked,
            this,                &AWaitModule::onStopAsked);
}

void AWaitModule::processModule(const JsonArray &jsonObjectInputValues)
{
    if(!PauseAndStopHelper::managePauseAndStop(getSequenceState()))
    {
        emit moduleFinished(false, JsonArray());
        return;
    }

    Number timeToWaitInMs;

    if(!processWaiting(jsonObjectInputValues, timeToWaitInMs))
    {
        emit moduleFinished(false, JsonArray());
        return;
    }

    if(!timeToWaitInMs.isValid())
    {
        qWarning() << "The time to wait in ms is invalid, can't proceed";
        emit moduleFinished(false, JsonArray());
        return;
    }

    bool ok = false;

    int integerTimeToWaitInMs = timeToWaitInMs.toInt32(&ok);

    if(!ok)
    {
        qWarning() << "Can't convert to int32 the time to wait";
        emit moduleFinished(false, JsonArray());
    }

    _timer.start(integerTimeToWaitInMs);
}

void AWaitModule::onNewModuleState(SequenceStateType::Enum newState,
                                   SequenceStateType::Enum previousState)
{
    if(previousState == SequenceStateType::Running && newState == SequenceStateType::InPause)
    {
        if(!_timer.isActive())
        {
            // Nothing to do here
            return;
        }

        int remainingTime = _timer.remainingTime();
        _timer.stop();
        _timer.setInterval(remainingTime);
        return;
    }

    if(previousState == SequenceStateType::InPause && newState == SequenceStateType::Running)
    {
        if(_timer.isActive())
        {
            qWarning() << "The timer is already active, can't resume it";
            return;
        }

        _timer.start();
        return;
    }
}

void AWaitModule::onStopAsked()
{
    _timer.stop();

    emit moduleFinished(false, JsonArray());
}

void AWaitModule::onTimeout()
{
    emit moduleFinished(true, JsonArray());
}
