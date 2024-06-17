// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "sequencestatehelper.hpp"


SequenceStateHelper::SequenceStateHelper(QObject *parent) : QObject(parent)
{
}

void SequenceStateHelper::onRunning()
{
    _stopAskedType = StopAskedType::NoStopAsked;
    _pauseAsked = false;
    setCurrentState(SequenceStateType::Running);
}

void SequenceStateHelper::onPauseOrResume(bool setToPause)
{
    if(!setToPause && !_pauseAsked && _currentState != SequenceStateType::InPause)
    {
        // If resume is asking and pause hasn't been previously asked and pause is not effective,
        // there is nothing to be done
        return;
    }

    if(setToPause && (_currentState == SequenceStateType::Stopped || _stopAskedType))
    {
        // If pause is asking and stop hasn't been previously asked or the sequence is stopped,
        // there is bothing to be done. Because the Stop is more important than the pause
        return;
    }

    _pauseAsked = setToPause;

    setCurrentState(setToPause ? SequenceStateType::InPause : SequenceStateType::Running);
}

void SequenceStateHelper::onStopAsked(StopAskedType::Enum stopAskedType)
{
    if(_stopAskedType != stopAskedType)
    {
        _stopAskedType = stopAskedType;
        emit stopAsked(stopAskedType);
    }
}

void SequenceStateHelper::onStop()
{
    _stopAskedType = StopAskedType::NoStopAsked;
    setCurrentState(SequenceStateType::Stopped);
}

void SequenceStateHelper::registerMetaTypes()
{
    SequenceStateType::registerMetaType();
    StopAskedType::registerMetaType();
}

void SequenceStateHelper::setCurrentState(SequenceStateType::Enum state)
{
    if(state != _currentState)
    {
        SequenceStateType::Enum previousState = _currentState;
        _currentState = state;
        emit newState(state, previousState);
    }
}
