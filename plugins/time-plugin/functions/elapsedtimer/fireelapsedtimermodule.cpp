// SPDX-FileCopyrightText: 2020 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "fireelapsedtimermodule.hpp"

#include "bankglobalmanager.hpp"
#include "functions/elapsedtimer/elapsedtimermanager.hpp"
#include "testbedcore/logs/sequencelogformatter.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/utility/pauseandstophelper.hpp"


FireElapsedTimerModule::FireElapsedTimerModule(QObject *parent) :
    DefaultSequenceInstanceModule(staticMetaObject.className(),
                                  BankGlobalManager::Instance(),
                                  parent)
{
}

bool FireElapsedTimerModule::initOutputsFormat(JsonArray &outputsFormat) const
{
    JsonObject elapsedTimerToken;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(outputElapsedTimerName,
                                                         elapsedTimerToken,
                                                         outputElapsedTimerDesc));

    outputsFormat.append(elapsedTimerToken);

    return true;
}

void FireElapsedTimerModule::processModule(const JsonArray &jsonObjectInputValues)
{
    Q_UNUSED(jsonObjectInputValues)

    if(!PauseAndStopHelper::managePauseAndStop(getSequenceState()))
    {
        emit moduleFinished(false, {});
        return;
    }

    ElapsedTimerManager &elapsedManager =
                                        BankGlobalManager::Instance()->accessElapsedTimerManager();

    QString token = elapsedManager.createAnElapsedTimer();

    if(!elapsedManager.fireAnElapsedTimer(token))
    {
        emit newLog(SequenceLogFormatter::formatAnApplicationProblem("Cannot launch an elapsed "
                                                                     "timer"));
        emit moduleFinished(false, {});
        return;
    }

    emit newLog(SequenceLogData(LogMsgType::Info,
                                SequenceLogCategory::ApplicationScope,
                                "Fire an elapsed timer: " + token));

    JsonArray output;

    if(!BankJsonParser::setStringValue(outputElapsedTimerName, token, output))
    {
        emit moduleFinished(false, {});
        return;
    }

    emit moduleFinished(true, output);
}
