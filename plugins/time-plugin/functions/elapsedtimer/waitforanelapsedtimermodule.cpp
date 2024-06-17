// SPDX-FileCopyrightText: 2020 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "waitforanelapsedtimermodule.hpp"

#include <QElapsedTimer>
#include <QTimer>

#include "bankglobalmanager.hpp"
#include "functions/elapsedtimer/elapsedtimermanager.hpp"
#include "testbedcore/logs/sequencelogformatter.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/utility/pauseandstophelper.hpp"


WaitForAnElapsedTimerModule::WaitForAnElapsedTimerModule(QObject *parent) :
    DefaultSequenceInstanceModule(staticMetaObject.className(),
                                  BankGlobalManager::Instance(),
                                  parent),
    _elapsedManager(BankGlobalManager::Instance()->accessElapsedTimerManager())
{
    _currentTimer.setSingleShot(true);
    _currentTimer.setTimerType(Qt::PreciseTimer);
    connect(&_currentTimer, &QTimer::timeout,
            this,           &WaitForAnElapsedTimerModule::endOfTimer);
}

bool WaitForAnElapsedTimerModule::initParamsFormat(JsonArray &paramsFormat) const
{
    JsonObject timeToWait;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(timeToWaitInMsParamName,
                                                                 timeToWaitInMsParamDesc,
                                                                 0,
                                                                 0,
                                                                 timeToWait));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(timeToWait,
                                                                   Number(0),
                                                                   Number::maxInt32(),
                                                                   Number(1)));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, timeToWait));

    paramsFormat.append(timeToWait);

    JsonObject RemoveElapsedAtEnd;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamBoolRequired(removeElapsedAtEndParamName,
                                                               removeElapsedAtEndParamDesc,
                                                               RemoveElapsedAtEnd));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueBool(true, RemoveElapsedAtEnd));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, RemoveElapsedAtEnd));

    paramsFormat.append(RemoveElapsedAtEnd);

    return true;
}

bool WaitForAnElapsedTimerModule::initInputsFormat(JsonArray &inputsFormat) const
{
    JsonObject elapsedTimerInput;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(inputElapsedTimerTokenName,
                                                         elapsedTimerInput,
                                                         inputElapsedTimerTokenDesc));

    inputsFormat.append(elapsedTimerInput);

    return true;
}

bool WaitForAnElapsedTimerModule::validateAndSetParameters(const JsonArray &parameters,
                                                           JsonArray &errors)
{
    RETURN_IF_FALSE(DefaultSequenceInstanceModule::validateAndSetParameters(parameters, errors));

    QString error;

    Number tmpTimeToWait;
    if(!BankJsonParser::getNumberValue(parameters,
                                       timeToWaitInMsParamName,
                                       tmpTimeToWait,
                                       &error))
    {
        fillErrorArray(timeToWaitInMsParamName, error, errors);
        return false;
    }

    // Because the integer limit are checked in the validator, we can get the value without limits
    // test
    _timeToWait = tmpTimeToWait.toInt32();

    if(!BankJsonParser::getBoolValue(parameters,
                                     removeElapsedAtEndParamName,
                                     _removeElapsedAtEnd,
                                     &error))
    {
        fillErrorArray(removeElapsedAtEndParamName, error, errors);
        return false;
    }

    return true;
}

void WaitForAnElapsedTimerModule::processModule(const JsonArray &jsonObjectInputValues)
{
    if(!PauseAndStopHelper::managePauseAndStop(getSequenceState()))
    {
        emit moduleFinished(false, {});
        return;
    }

    _currentToken.clear();

    if(!BankJsonParser::getStringValue(jsonObjectInputValues,
                                       inputElapsedTimerTokenName,
                                       _currentToken))
    {
        emit moduleFinished(false, {});
        return;
    }

    const QElapsedTimer *elapsedTimer = _elapsedManager.getAnElapsedTimer(_currentToken);

    if(!elapsedTimer)
    {
        emit newLog(SequenceLogFormatter::formatAnApplicationProblem("The elapsed timer: " +
                                                                     _currentToken +
                                                                     "hasn't been found"));
        emit moduleFinished(false, {});
        return;
    }

    if(!elapsedTimer->isValid())
    {
        emit newLog(SequenceLogFormatter::formatAnApplicationProblem("The elapsed timer: " +
                                                                     _currentToken +
                                                                     "is invalid, can't proceed"));
        emit moduleFinished(false, {});
        return;
    }

    qint64 elapsed = elapsedTimer->elapsed();

    if(elapsed >= _timeToWait)
    {
        endOfTimer();
        return;
    }

    int leftTime = _timeToWait - static_cast<int>(elapsed);

    _currentTimer.start(leftTime);
}

void WaitForAnElapsedTimerModule::onNewModuleState(SequenceStateType::Enum newState,
                                                   SequenceStateType::Enum previousState)
{
    if(previousState == SequenceStateType::Running && newState == SequenceStateType::InPause)
    {
        if(!_currentTimer.isActive())
        {
            // Nothing to do here
            return;
        }

        int remainingTime = _currentTimer.remainingTime();
        _currentTimer.stop();
        _currentTimer.setInterval(remainingTime);
        return;
    }

    if(previousState == SequenceStateType::InPause && newState == SequenceStateType::Running)
    {
        if(_currentTimer.isActive())
        {
            qWarning() << "The timer is already active, can't resume it";
            return;
        }

        _currentTimer.start();
        return;
    }
}

void WaitForAnElapsedTimerModule::onStopAsked()
{
    _currentTimer.stop();

    emit moduleFinished(false, {});
}

void WaitForAnElapsedTimerModule::endOfTimer()
{
    if(_removeElapsedAtEnd)
    {
        _elapsedManager.removeElapsedTimer(_currentToken);
    }


    emit newLog(SequenceLogData(LogMsgType::Info,
                                SequenceLogCategory::ApplicationScope,
                                "ElapsedTimer: " + _currentToken + ", stopped"));
    emit moduleFinished(true, {});
}
