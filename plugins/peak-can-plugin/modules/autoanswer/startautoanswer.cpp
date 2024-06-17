// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "startautoanswer.hpp"

#include "qtpeakcanlib/src/pcanapi/pcanbusitf.hpp"

#include "peak-can-lib/canmodulesutility.hpp"
#include "peak-can-lib/peakcanlibconstants.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/utility/pauseandstophelper.hpp"
#include "testbedcore/logs/sequencelogformatter.hpp"

#include "managers/autoanswerpeakcanmanager.hpp"
#include "models/canmsgseqparams.hpp"
#include "process/autoansprocessthread.hpp"
#include "process/autoansthreadhandler.hpp"
#include "peakcanbankglobalmanager.hpp"


StartAutoAnswer::StartAutoAnswer(QObject *parent)
    : DefaultSequenceInstanceModule{staticMetaObject.className(),
                                    PeakCanBankGlobalManager::Instance(),
                                    parent},
    _msgSeqParams{new CanMsgSeqParams(this)}
{
}

StartAutoAnswer::~StartAutoAnswer()
{
}

bool StartAutoAnswer::manageModulePause(bool pauseModule)
{
    if(!_autoAnsHandler.isNull() && _autoAnsHandler->accessThread() != nullptr)
    {
        _autoAnsHandler->accessThread()->manageEchoingPause(pauseModule);
    }

    return DefaultSequenceInstanceModule::manageModulePause(pauseModule);
}

bool StartAutoAnswer::initInputsFormat(JsonArray &inputsFormat) const
{
    JsonObject inputPeakCanIntf;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(
        PeakCanLibConstants::ModulesEntries::Inputs::InputPeakCanIntfName,
        inputPeakCanIntf,
        PeakCanLibConstants::ModulesEntries::Inputs::InputPeakCanIntfDesc));

    inputsFormat.append(inputPeakCanIntf);
    return true;
}

bool StartAutoAnswer::initParamsFormat(JsonArray &paramsFormat) const
{
    JsonObject displayStatsObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamBoolRequired(DisplayStatsParamName,
                                                               DisplayStatsParamDesc,
                                                               displayStatsObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueBool(DisplayStatsDefaultValue,
                                                                   displayStatsObj));

    paramsFormat.append(displayStatsObj);

    JsonObject answerToSendObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringListRequired(AnswersToSendParamName,
                                                                     AnswersToSendParamDesc,
                                                                     answerToSendObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, answerToSendObj));

    paramsFormat.append(answerToSendObj);

    JsonObject expectedMsgsObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringListRequired(ExpectedMsgsVerifsParamName,
                                                                     ExpectedMsgsVerifsParamDesc,
                                                                     expectedMsgsObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, expectedMsgsObj));

    paramsFormat.append(expectedMsgsObj);

    return true;
}

bool StartAutoAnswer::validateAndSetParameters(const JsonArray &parameters, JsonArray &errors)
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

    _displayStats = displayStats;

    QVector<QString> expectedMsgVerifs;
    if(!BankJsonParser::getArrayStringValue(parameters,
                                            ExpectedMsgsVerifsParamName,
                                            expectedMsgVerifs,
                                            &error))
    {
        fillErrorArray(ExpectedMsgsVerifsParamName, error, errors);
        return false;
    }

    _msgSeqParams->setVerifs(expectedMsgVerifs);

    QVector<QString> answersToSend;
    if(!BankJsonParser::getArrayStringValue(parameters,
                                            AnswersToSendParamName,
                                            answersToSend,
                                            &error))
    {
        fillErrorArray(AnswersToSendParamName, error, errors);
        return false;
    }

    _msgSeqParams->setCmds(answersToSend);

    return true;
}

void StartAutoAnswer::processModule(const JsonArray &jsonObjectInputValues)
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

bool StartAutoAnswer::startAutoAnswer(const JsonArray &jsonObjectInputValues)
{
    QString strCanBusIntf;
    PCanBusItf::Enum pCanBusIntf;
    RETURN_IF_FALSE(CanModulesUtility::extractAndParseCanBusIntf(
        jsonObjectInputValues,
        strCanBusIntf,
        pCanBusIntf));

    QSharedPointer<AutoAnsThreadHandler> process = PeakCanBankGlobalManager::Instance()->
                                                accessAutoAnswerManager().createOrGetEchoProcess(
        pCanBusIntf, *_msgSeqParams, *this, _displayStats);

    if(process == nullptr || process->isThreadNull())
    {
        qWarning() << "A problem occurred when tried to create the echo process";
        return false;
    }

    _autoAnsHandler = process;

    return _autoAnsHandler->accessThread()->startEchoing();
}
