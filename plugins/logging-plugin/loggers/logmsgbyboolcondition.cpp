// SPDX-FileCopyrightText: 2020 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "logmsgbyboolcondition.hpp"

#include "abstractpluginelements/defaultbankglobalmanager.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/testbedglobal.hpp"
#include "testbedcore/utility/pauseandstophelper.hpp"


LogMsgByBoolCondition::LogMsgByBoolCondition(QObject *parent) :
    DefaultSequenceInstanceModule(staticMetaObject.className(),
                                  DefaultBankGlobalManager::Instance(),
                                  parent)
{
}

LogMsgByBoolCondition::~LogMsgByBoolCondition()
{
}

bool LogMsgByBoolCondition::initInputsFormat(JsonArray &inputsFormat) const
{
    JsonObject inputCondition;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutBool(InputConditionName,
                                                       inputCondition,
                                                       InputConditionDesc));

    inputsFormat.append(inputCondition);

    return true;
}

bool LogMsgByBoolCondition::initParamsFormat(JsonArray &paramsFormat) const
{
    JsonObject toDisplayIfTrueLogLevel;
    JsonObject toDisplayIfFalseLogLevel;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamSelectRequired(ToDisplayLogLevelIfTrueParamName,
                                                                 ToDisplayLogLevelIfTrueParamDesc,
                                                                 toDisplayIfTrueLogLevel));
    RETURN_IF_FALSE(BankJsonFormatter::formatParamSelectRequired(ToDisplayLogLevelIfFalseParamName,
                                                                 ToDisplayLogLevelIfFalseParamDesc,
                                                                 toDisplayIfFalseLogLevel));

    for(LogMsgType::Enum logType : LogMsgType::getAvailableElements())
    {
        QString logTypeStr = LogMsgType::toString(logType);

        RETURN_IF_FALSE(BankJsonFormatter::formatParamAddSelectOption(logTypeStr,
                                                                      logTypeStr,
                                                                      toDisplayIfTrueLogLevel));
        RETURN_IF_FALSE(BankJsonFormatter::formatParamAddSelectOption(logTypeStr,
                                                                      logTypeStr,
                                                                      toDisplayIfFalseLogLevel));
    }

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultOption(
        { LogMsgType::toString(DefaultToDisplayLogLevelIfTrue) },
        toDisplayIfTrueLogLevel));
    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultOption(
        { LogMsgType::toString(DefaultToDisplayLogLevelIfFalse) },
        toDisplayIfFalseLogLevel));

    paramsFormat.append(toDisplayIfTrueLogLevel);
    paramsFormat.append(toDisplayIfFalseLogLevel);

    JsonObject toDisplayIfTrue;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(ToDisplayIfTrueParamName,
                                                                 ToDisplayIfTrueParamDesc,
                                                                 toDisplayIfTrue));

    paramsFormat.append(toDisplayIfTrue);

    JsonObject toDisplayIfFalse;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(ToDisplayIfFalseParamName,
                                                                 ToDisplayIfFalseParamDesc,
                                                                 toDisplayIfFalse));

    paramsFormat.append(toDisplayIfFalse);

    JsonObject toDisplayCatIfTrue;
    JsonObject toDisplayCatIfFalse;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamSelectRequired(ToDisplayCatIfTrueParamName,
                                                                 ToDisplayCatIfTrueParamDesc,
                                                                 toDisplayCatIfTrue));
    RETURN_IF_FALSE(BankJsonFormatter::formatParamSelectRequired(ToDisplayCatIfFalseParamName,
                                                                 ToDisplayCatIfFalseParamDesc,
                                                                 toDisplayCatIfFalse));

    QVector<SequenceLogCategory::Enum> logcats = SequenceLogCategory::getAllEnums();
    for(auto citer = logcats.cbegin(); citer != logcats.cend(); ++citer)
    {
        QString logCatStr = SequenceLogCategory::toString(*citer);

        RETURN_IF_FALSE(BankJsonFormatter::formatParamAddSelectOption(logCatStr,
                                                                      logCatStr,
                                                                      toDisplayCatIfTrue));
        RETURN_IF_FALSE(BankJsonFormatter::formatParamAddSelectOption(logCatStr,
                                                                      logCatStr,
                                                                      toDisplayCatIfFalse));
    }

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultOption(
        { SequenceLogCategory::toString(DefaultToDisplayCatIfTrue) },
        toDisplayCatIfTrue));
    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultOption(
        { SequenceLogCategory::toString(DefaultToDisplayCatIfFalse) },
        toDisplayCatIfFalse));

    paramsFormat.append(toDisplayCatIfTrue);
    paramsFormat.append(toDisplayCatIfFalse);

    return true;
}

bool LogMsgByBoolCondition::validateAndSetParameters(const JsonArray &parameters, JsonArray &errors)
{
    RETURN_IF_FALSE(DefaultSequenceInstanceModule::validateAndSetParameters(parameters, errors));

    QString error;

    if(!BankJsonParser::getStringValue(parameters,
                                       ToDisplayIfTrueParamName,
                                       _toDisplayIfTrue,
                                       &error,
                                       true))
    {
        fillErrorArray(ToDisplayIfTrueParamName, _toDisplayIfTrue, errors);
        return false;
    }

    if(!BankJsonParser::getStringValue(parameters,
                                        ToDisplayIfFalseParamName,
                                       _toDisplayIfFalse,
                                       &error,
                                       true))
    {
        fillErrorArray(ToDisplayIfFalseParamName, _toDisplayIfFalse, errors);
        return false;
    }

    auto getValues = [&parameters, &errors](const QString &paramName, QString &value)
    {
        QString tmpError;

        QVector<QString> tmpValues;
        if(!BankJsonParser::getArrayStringValue(parameters, paramName, tmpValues, &tmpError))
        {
            fillErrorArray(paramName, tmpError, errors);
            return false;
        }

        if(tmpValues.isEmpty())
        {
            qWarning() << "The param: " << paramName << ", is empty, no choice has been made";
            fillErrorArray(paramName, "A value has to be chosen", errors);
            return false;
        }

        value = tmpValues.first();

        return true;
    };

    auto manageLogLevel = [&errors, &getValues](const QString &paramName,
                                                const QString &toDisplay,
                                                LogMsgType::Enum &logType)
    {
        if(toDisplay.isEmpty())
        {
            // Nothing to do here
            return true;
        }

        QString tmpValue;
        RETURN_IF_FALSE(getValues(paramName, tmpValue));

        LogMsgType::Enum tmpMsgType = LogMsgType::parseCriticityFromString(tmpValue);

        if(tmpMsgType == LogMsgType::Unknown)
        {
            qWarning() << "The param: " << paramName << ", has a value not known LogMsgType: "
                       << tmpValue;
            fillErrorArray(paramName, "The value: " + tmpValue + ", is not known", errors);
            return false;
        }

        logType = tmpMsgType;

        return true;
    };

    auto manageLogCat = [&errors, &getValues](const QString &paramName,
                                              const QString &toDisplay,
                                              SequenceLogCategory::Enum &logCat)
    {
        if(toDisplay.isEmpty())
        {
            // Nothing to do here
            return true;
        }

        QString tmpValue;
        RETURN_IF_FALSE(getValues(paramName, tmpValue));

        bool ok = false;
        SequenceLogCategory::Enum tmpLog = SequenceLogCategory::parseFromString(tmpValue,
                                                                                &ok);

        if(!ok)
        {
            qWarning() << "The param: " << paramName << ", has a value not known "
                       << "SequenceLogCategory: " << tmpValue;
            fillErrorArray(paramName, "The value: " + tmpValue + ", is not known", errors);
            return false;
        }

        logCat = tmpLog;

        return true;
    };

    RETURN_IF_FALSE(manageLogLevel(ToDisplayLogLevelIfTrueParamName,
                                   _toDisplayIfTrue,
                                   _toDisplayIfTrueLogLevel));

    RETURN_IF_FALSE(manageLogLevel(ToDisplayLogLevelIfFalseParamName,
                                   _toDisplayIfFalse,
                                   _toDisplayIfFalseLogLevel));

    RETURN_IF_FALSE(manageLogCat(ToDisplayCatIfTrueParamName,
                                 _toDisplayIfTrue,
                                 _toDisplayCatIfTrue));

    RETURN_IF_FALSE(manageLogCat(ToDisplayCatIfFalseParamName,
                                 _toDisplayIfFalse,
                                 _toDisplayCatIfFalse));

    return true;
}

void LogMsgByBoolCondition::processModule(const JsonArray &jsonObjectInputValues)
{
    if(!PauseAndStopHelper::managePauseAndStop(getSequenceState()))
    {
        emit moduleFinished(false, {});
        return;
    }

    bool value = false;
    if(!BankJsonParser::getBoolValue(jsonObjectInputValues, InputConditionName, value))
    {
        qWarning() << "Can't get the boolean value of the log condition";
        emit moduleFinished(false, {});
        return;
    }

    if(value && !_toDisplayIfTrue.isEmpty())
    {
        emit newLog(SequenceLogData(_toDisplayIfTrueLogLevel,
                                    _toDisplayCatIfTrue,
                                    _toDisplayIfTrue));
    }
    else if(!value && !_toDisplayIfFalse.isEmpty())
    {
        emit newLog(SequenceLogData(_toDisplayIfFalseLogLevel,
                                    _toDisplayCatIfFalse,
                                    _toDisplayIfFalse));
    }

    emit moduleFinished(true, {});
}
