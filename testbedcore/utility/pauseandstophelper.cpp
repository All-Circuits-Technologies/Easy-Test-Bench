// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "pauseandstophelper.hpp"

#include <QDebug>

#include "testbedglobal.hpp"
#include "types/stopaskedtype.hpp"
#include "utility/sequencestatehelper.hpp"
#include "waitutility/waithelper.hpp"


PauseAndStopHelper::PauseAndStopHelper(QObject *parent) :
    QObject(parent)
{
}

bool PauseAndStopHelper::managePauseAndStop(const bool &stop, const bool &pause)
{
    std::function<bool ()> pauseFunc = [&pause](){ return pause; };
    std::function<bool ()> stopFunc = [&stop](){ return stop; };

    return managePauseAndStopProcess(stopFunc, pauseFunc);
}

bool PauseAndStopHelper::managePauseAndStop(const TestFunc &stopFunc, const bool &pause)
{
    std::function<bool ()> pauseFunc = [&pause](){ return pause; };

    return managePauseAndStop(stopFunc, pauseFunc);
}

bool PauseAndStopHelper::managePauseAndStop(const bool &stop, const TestFunc &pauseFunc)
{
    std::function<bool ()> stopFunc = [&stop](){ return stop; };

    return managePauseAndStopProcess(stopFunc, pauseFunc);
}

bool PauseAndStopHelper::managePauseAndStop(const TestFunc &stopFunc,
                                             const TestFunc &pauseFunc)
{
    return managePauseAndStopProcess(stopFunc, pauseFunc);
}

bool PauseAndStopHelper::managePauseAndStop(const SequenceStateHelper &sequenceState)
{
    std::function<bool ()> stopFunc = [&sequenceState]()
        { return sequenceState.getStopAskedType() != StopAskedType::NoStopAsked ||
                 sequenceState.isStopped(); };

    return managePauseAndStopProcess(stopFunc,
                                     castTestCallback(sequenceState,
                                                      &SequenceStateHelper::isPauseAsked));
}

bool PauseAndStopHelper::managePauseAndStopProcess(const TestFunc &stopFunc,
                                                   const TestFunc &pauseFunc)
{
    if(stopFunc())
    {
        qInfo() << "Stop has been asked";
        return false;
    }

    if(!pauseFunc())
    {
        // Stop hasn't been asked and neither pause, therefore we do nothing
        return true;
    }

    std::function<bool ()> exitWait = [&stopFunc, &pauseFunc]()
    {
        return stopFunc() || !pauseFunc();
    };

    RETURN_IF_FALSE(WaitHelper::pseudoWait(exitWait));

    if(stopFunc())
    {
        qInfo() << "Stop has been asked after or while pausing";
        return false;
    }

    return true;
}
