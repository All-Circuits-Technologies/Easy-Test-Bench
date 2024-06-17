// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "aprojectcachemodule.hpp"

#include "abstractpluginelements/defaultbankglobalmanager.hpp"
#include "bankcoreshared/usebyplugin/icommoninterface.hpp"
#include "testbedcore/logs/sequencelogformatter.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/testbedglobal.hpp"
#include "testbedcore/utility/pauseandstophelper.hpp"


AProjectCacheModule::AProjectCacheModule(const QString &moduleClassName, QObject *parent) :
    DefaultSequenceInstanceModule(moduleClassName,
                                  DefaultBankGlobalManager::Instance(),
                                  parent),
    _commonItf(DefaultBankGlobalManager::Instance()->accessCommonInterface())
{
}

bool AProjectCacheModule::initInputsFormat(JsonArray &inputsFormat) const
{
    JsonObject projectCacheKey;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(InputProjectCacheKeyName,
                                                         projectCacheKey,
                                                         InputProjectCacheKeyDesc));

    inputsFormat.append(projectCacheKey);

    return true;
}

void AProjectCacheModule::processModule(const JsonArray &jsonObjectInputValues)
{
    if(!PauseAndStopHelper::managePauseAndStop(getSequenceState()))
    {
        emit moduleFinished(false, {});
        return;
    }

    QString cacheKey;
    if(!BankJsonParser::getStringValue(jsonObjectInputValues, InputProjectCacheKeyName, cacheKey))
    {
        emit newLog(SequenceLogFormatter::formatABusinessProblem(tr("(TR) A problem occured while "
                                                                    "trying to get the project "
                                                                    "cache key")));
        emit moduleFinished(false, {});
        return;
    }

    JsonArray output;
    if(!actOnCache(jsonObjectInputValues, cacheKey, output))
    {
        emit newLog(SequenceLogFormatter::formatABusinessProblem(tr("(TR) A problem occured while "
                                                                    "trying to operate on the "
                                                                    "project cache")));
        emit moduleFinished(false, {});
        return;
    }


    emit newLog(SequenceLogData(LogMsgType::Info,
                                SequenceLogCategory::BusinessScope,
                                QString("The operation on project cache was successful")));

    emit moduleFinished(true, output);
}
