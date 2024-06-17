// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "csvlogline.hpp"

#include "abstractpluginelements/defaultbankglobalmanager.hpp"
#include "collectionutility/qvectorhelper.hpp"
#include "testbedcore/logs/sequencelogcategory.hpp"
#include "testbedcore/logs/sequencelogformatter.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/testbedglobal.hpp"
#include "testbedcore/utility/pauseandstophelper.hpp"


CsvLogLine::CsvLogLine(QObject *parent) :
    DefaultSequenceInstanceModule(staticMetaObject.className(),
                                  DefaultBankGlobalManager::Instance(),
                                  parent)
{
}

CsvLogLine::~CsvLogLine()
{
}

void CsvLogLine::processModule(const JsonArray &jsonObjectInputValues)
{
    if(!PauseAndStopHelper::managePauseAndStop(getSequenceState()))
    {
        emit moduleFinished(false, {});
        return;
    }

    emit newLog(SequenceLogData(LogMsgType::Info,
                                SequenceLogCategory::BusinessScope,
                                "Try to log the CSV line"));

    if(!operate(jsonObjectInputValues))
    {
        emit newLog(SequenceLogFormatter::formatABusinessProblem(tr("(TR) A problem occured when "
                                                                    "tryied to log the CSV "
                                                                    "line.")));
        emit moduleFinished(false, {});
        return;
    }

    emit newLog(SequenceLogData(LogMsgType::Info,
                                SequenceLogCategory::BusinessScope,
                                "Succeed to write the CSV line"));

    emit moduleFinished(true, {});
}

bool CsvLogLine::validateAndSetParameters(const JsonArray &parameters, JsonArray &errors)
{
    RETURN_IF_FALSE(DefaultSequenceInstanceModule::validateAndSetParameters(parameters, errors));

    QVector<QString> msgTypeStr;
    QString error;
    if(!BankJsonParser::getArrayStringValue(parameters,
                                            MsgTypeLevelParamName,
                                            msgTypeStr,
                                            &error,
                                            true))
    {
        fillErrorArray(MsgTypeLevelParamName, error, errors);
        return false;
    }

    bool ok = false;

    if(!msgTypeStr.isEmpty())
    {
        LogMsgType::Enum tmp = LogMsgType::parseCriticityFromString(msgTypeStr.first());

        if(tmp == LogMsgType::Unknown)
        {
            error = "A wrong msg type has been given " + msgTypeStr.first();
            fillErrorArray(MsgTypeLevelParamName, error, errors);
            return false;
        }

        _msgTypeLevel = tmp;
    }

    QVector<QString> logCatStr;
    if(!BankJsonParser::getArrayStringValue(parameters,
                                            LogCategoryParamName,
                                            logCatStr,
                                            &error))
    {
        fillErrorArray(LogCategoryParamName, error, errors);
        return false;
    }

    if(logCatStr.isEmpty())
    {
        error = "No log category has been given";
        fillErrorArray(LogCategoryParamName, error, errors);
        return false;
    }

    _logCategory = SequenceLogCategory::parseFromString(logCatStr.first(), &ok);

    if(!ok)
    {
        error = "A wrong log category has been given " + logCatStr.first();
        fillErrorArray(LogCategoryParamName, error, errors);
        return false;
    }

    if(!BankJsonParser::getStringValue(parameters,
                                       DelimiterParamName,
                                       _delimiter,
                                       &error,
                                       true))
    {
        fillErrorArray(DelimiterParamName, error, errors);
        return false;
    }

    return true;
}

bool CsvLogLine::initInputsFormat(JsonArray &inputsFormat) const
{
    JsonObject csvLine;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutStringArray(InputCsvLineName,
                                                              csvLine,
                                                              InputCsvLineDesc));

    inputsFormat.append(csvLine);

    return true;
}

bool CsvLogLine::initParamsFormat(JsonArray &paramsFormat) const
{
    JsonObject logCategoryObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamSelectRequired(LogCategoryParamName,
                                                                 LogCategoryParamDesc,
                                                                 logCategoryObj));

    QVector<SequenceLogCategory::Enum> categories = SequenceLogCategory::getAllEnums();
    for(auto citer = categories.cbegin(); citer != categories.cend(); ++citer)
    {
        QString formatTypestr = SequenceLogCategory::toString(*citer);

        RETURN_IF_FALSE(BankJsonFormatter::formatParamAddSelectOption(formatTypestr,
                                                                      formatTypestr,
                                                                      logCategoryObj));
    }

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, logCategoryObj));

    paramsFormat.append(logCategoryObj);

    JsonObject msgTypeObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamSelectRequired(MsgTypeLevelParamName,
                                                                 MsgTypeLevelParamName,
                                                                 msgTypeObj));

    for(LogMsgType::Enum msgType : LogMsgType::getAvailableElements())
    {
        QString msgTypestr = LogMsgType::toString(msgType);

        RETURN_IF_FALSE(BankJsonFormatter::formatParamAddSelectOption(msgTypestr,
                                                                      msgTypestr,
                                                                      msgTypeObj));
    }

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultOption(
        { LogMsgType::toString(MsgTypeLevelDefaultValue) },
                                            msgTypeObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, msgTypeObj));

    paramsFormat.append(msgTypeObj);

    JsonObject delimiterObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(DelimiterParamName,
                                                                 DelimiterParamDesc,
                                                                 delimiterObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueString(DelimiterDefaultValue,
                                                                     delimiterObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, delimiterObj));

    paramsFormat.append(delimiterObj);

    return true;
}

bool CsvLogLine::operate(const JsonArray &jsonObjectInputValues)
{
    QVector<QString> csvLine;
    RETURN_IF_FALSE(BankJsonParser::getArrayStringValue(jsonObjectInputValues,
                                                        InputCsvLineName,
                                                        csvLine));

    QString line = QVectorHelper::join(csvLine, _delimiter);

    emit newLog(SequenceLogData(_msgTypeLevel, _logCategory, line));

    return true;
}
