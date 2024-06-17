// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "startserialautoanswer.hpp"

#include "serial-link-lib/serialpluginlibconstants.hpp"
#include "serial-link-lib/utilities/serialmodulesutility.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/utility/pauseandstophelper.hpp"
#include "testbedcore/logs/sequencelogformatter.hpp"

#include "autoanswer/models/autoansmsgtosend.hpp"
#include "autoanswer/models/autoanswerparams.hpp"
#include "autoanswer/processes/autoansserialhandler.hpp"
#include "managers/autoanswerserialmanager.hpp"
#include "managers/seriallinkbankglobalmanager.hpp"


StartSerialAutoAnswer::StartSerialAutoAnswer(QObject *parent)
    : DefaultSequenceInstanceModule{staticMetaObject.className(),
                                    SerialLinkBankGlobalManager::Instance(),
                                    parent}
{
}

StartSerialAutoAnswer::~StartSerialAutoAnswer()
{
}

bool StartSerialAutoAnswer::manageModulePause(bool pauseModule)
{
    if(!_autoAnsHandler.isNull() && _autoAnsHandler->accessThread() != nullptr)
    {
        _autoAnsHandler->accessThread()->manageEchoingPause(pauseModule);
    }

    return DefaultSequenceInstanceModule::manageModulePause(pauseModule);
}

bool StartSerialAutoAnswer::initInputsFormat(JsonArray &inputsFormat) const
{
    JsonObject serialPortName;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(
        SerialPluginLibConstants::Json::PortName,
        serialPortName,
        SerialPluginLibConstants::Json::PortNameDesc));

    inputsFormat.append(serialPortName);
    return true;
}

bool StartSerialAutoAnswer::initParamsFormat(JsonArray &paramsFormat) const
{
    JsonObject displayStatsObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamBoolRequired(DisplayStatsParamName,
                                                               DisplayStatsParamDesc,
                                                               displayStatsObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueBool(DisplayStatsDefaultValue,
                                                                   displayStatsObj));

    paramsFormat.append(displayStatsObj);

    JsonObject endOfLineStringParam;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(
        SerialPluginLibConstants::Json::EndOfLine,
        SerialPluginLibConstants::Json::EndOfLineDesc,
        endOfLineStringParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueString(
        SerialPluginLibConstants::Defaults::EndOfLine,
        endOfLineStringParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, endOfLineStringParam));

    paramsFormat.append(endOfLineStringParam);

    JsonObject trimBoolParam;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamBoolRequired(
        SerialPluginLibConstants::Json::Trim,
        SerialPluginLibConstants::Json::TrimDesc,
        trimBoolParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueBool(
        SerialPluginLibConstants::Defaults::Trim,
        trimBoolParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, trimBoolParam));

    paramsFormat.append(trimBoolParam);

    JsonObject multiLineBoolParam;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamBoolRequired(
        SerialPluginLibConstants::Json::IsMultiLineTest,
        SerialPluginLibConstants::Json::IsMultiLineTestDesc,
        multiLineBoolParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueBool(
        SerialPluginLibConstants::Defaults::MultiLine,
        multiLineBoolParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, multiLineBoolParam));

    paramsFormat.append(multiLineBoolParam);

    JsonObject forceFlushBoolParam;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamBoolRequired(
        SerialPluginLibConstants::Json::ForceFlush,
        SerialPluginLibConstants::Json::ForceFlushDesc,
        forceFlushBoolParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueBool(DefaultFlushSendValue,
                                                                   forceFlushBoolParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, forceFlushBoolParam));

    paramsFormat.append(forceFlushBoolParam);

    JsonObject msgsToSendParam;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringListRequired(
        AutoAnsMsgToSend::MsgsToSendParamName,
        AutoAnsMsgToSend::MsgsToSendParamDesc,
        msgsToSendParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, msgsToSendParam));

    paramsFormat.append(msgsToSendParam);

    JsonObject expectedMessagesParam;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringListRequired(
        AutoAnswerParams::ExpectedMessagesParamName,
        AutoAnswerParams::ExpectedMessagesParamDesc,
        expectedMessagesParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, expectedMessagesParam));

    paramsFormat.append(expectedMessagesParam);

    return true;
}

bool StartSerialAutoAnswer::validateAndSetParameters(const JsonArray &parameters, JsonArray &errors)
{
    RETURN_IF_FALSE(DefaultSequenceInstanceModule::validateAndSetParameters(parameters, errors));

    QString error;

    bool displayStats;
    if(!BankJsonParser::getBoolValue(parameters,
                                      DisplayStatsParamName,
                                      displayStats,
                                      &error,
                                      true))
    {
        fillErrorArray(DisplayStatsParamName, error, errors);
        return false;
    }

    QString endOfLine = SerialPluginLibConstants::Defaults::EndOfLine;
    if(!BankJsonParser::getStringValue(parameters,
                                        SerialPluginLibConstants::Json::EndOfLine,
                                        endOfLine,
                                        &error,
                                        true))
    {
        fillErrorArray(SerialPluginLibConstants::Json::EndOfLine, error, errors);
        return false;
    }

    bool trim = SerialPluginLibConstants::Defaults::Trim;
    if(!BankJsonParser::getBoolValue(parameters,
                                      SerialPluginLibConstants::Json::Trim,
                                      trim,
                                      &error,
                                      true))
    {
        fillErrorArray(SerialPluginLibConstants::Json::Trim, error, errors);
        return false;
    }

    bool multiLine = SerialPluginLibConstants::Defaults::MultiLine;
    if(!BankJsonParser::getBoolValue(parameters,
                                      SerialPluginLibConstants::Json::IsMultiLineTest,
                                      multiLine,
                                      &error,
                                      true))
    {
        fillErrorArray(SerialPluginLibConstants::Json::IsMultiLineTest, error, errors);
        return false;
    }

    bool forceFlush = DefaultFlushSendValue;
    if(!BankJsonParser::getBoolValue(parameters,
                                      SerialPluginLibConstants::Json::ForceFlush,
                                      forceFlush,
                                      &error,
                                      true))
    {
        fillErrorArray(SerialPluginLibConstants::Json::ForceFlush, error, errors);
        return false;
    }

    QVector<QString> msgsToSend;
    if(!BankJsonParser::getArrayStringValue(parameters,
                                            AutoAnsMsgToSend::MsgsToSendParamName,
                                            msgsToSend,
                                            &error))
    {
        fillErrorArray(AutoAnsMsgToSend::MsgsToSendParamName, error, errors);
        return false;
    }

    QVector<QString> expectedMsgs;
    if(!BankJsonParser::getArrayStringValue(parameters,
                                            AutoAnswerParams::ExpectedMessagesParamName,
                                            expectedMsgs,
                                            &error))
    {
        fillErrorArray(AutoAnswerParams::ExpectedMessagesParamName, error, errors);
        return false;
    }

    AutoAnswerParams *autoAnsParam = AutoAnswerParams::parseAutoAnsParams(
        expectedMsgs,
        msgsToSend,
        endOfLine,
        trim,
        multiLine,
        displayStats,
        forceFlush,
        errors);

    if(autoAnsParam == nullptr)
    {
        return false;
    }

    delete _autoAnsParam;
    _autoAnsParam = autoAnsParam;

    return true;
}

void StartSerialAutoAnswer::processModule(const JsonArray &jsonObjectInputValues)
{
    if(!PauseAndStopHelper::managePauseAndStop(getSequenceState()))
    {
        emit moduleFinished(false, {});
        return;
    }

    if(!startAutoAnswer(jsonObjectInputValues))
    {
        emit newLog(SequenceLogFormatter::formatABusinessProblem(tr("(TR) A problem occurred while "
                                                                    "trying to start the auto "
                                                                    "answer process")));
        emit moduleFinished(false, {});
        return;
    }

    emit moduleFinished(true, {});
}

bool StartSerialAutoAnswer::startAutoAnswer(const JsonArray &jsonObjectInputValues)
{
    QString portName;
    RETURN_IF_FALSE(SerialModulesUtility::extractSerialLink(jsonObjectInputValues, portName));

    QSharedPointer<AutoAnsSerialHandler> handler = SerialLinkBankGlobalManager::Instance()->
                                                accessAutoAnswerManager().createOrGetEchoProcess(
        portName, *this, _autoAnsParam);

    if(handler == nullptr || handler->isThreadNull())
    {
        qWarning() << "A problem occurred when tried to create the echo process";
        return false;
    }

    _autoAnsHandler = handler;

    return _autoAnsHandler->accessThread()->startEchoing();
}
