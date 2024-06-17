// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "acanmsgsequence.hpp"

#include "qtpeakcanlib/src/pcanapi/pcanbusitf.hpp"

#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/utility/pauseandstophelper.hpp"
#include "testbedcore/logs/sequencelogformatter.hpp"

#include "peak-can-lib/canmodulesutility.hpp"
#include "peak-can-lib/peakcanlibconstants.hpp"

#include "models/canmsgseqparams.hpp"
#include "models/readandwrite/pcansequenceinfo.hpp"
#include "peakcanbankglobalmanager.hpp"
#include "peakcanconstants.hpp"
#include "process/canmsgprocess.hpp"


ACanMsgSequence::ACanMsgSequence(const QString &moduleClassName,
                                 bool manageMultipleMsgs,
                                 bool exportReceivedMsgs,
                                 QObject *parent)
    : DefaultSequenceInstanceModule{moduleClassName, PeakCanBankGlobalManager::Instance(), parent},
    _manageMultipleMsgs{manageMultipleMsgs},
    _exportReceivedMsgs{exportReceivedMsgs}
{
}

ACanMsgSequence::~ACanMsgSequence()
{
    delete _sequenceInfo;
}

bool ACanMsgSequence::initInputsFormat(JsonArray &inputsFormat) const
{
    JsonObject inputPeakCanIntf;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(
        PeakCanLibConstants::ModulesEntries::Inputs::InputPeakCanIntfName,
        inputPeakCanIntf,
        PeakCanLibConstants::ModulesEntries::Inputs::InputPeakCanIntfDesc));

    inputsFormat.append(inputPeakCanIntf);
    return true;
}

bool ACanMsgSequence::initParamsFormat(JsonArray &paramsFormat) const
{
    JsonObject testGroupNameObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(TestGroupNameParamName,
                                                                 TestGroupNameParamDesc,
                                                                 testGroupNameObj));

    paramsFormat.append(testGroupNameObj);

    JsonObject timeoutObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(ExpectingMsgTimeoutParamName,
                                                                 ExpectingMsgTimeoutParamDesc,
                                                                 0,
                                                                 0,
                                                                 timeoutObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamUnitNumber(Unit(UnitType::Second, -3),
                                                             timeoutObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(timeoutObj,
                                                                   Number(1, 0, false),
                                                                   Number::maxInt32(),
                                                                   Number(1)));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueNumber(
        Number::fromInt32(ExpectingMsgTimeoutDefaultValue),
        timeoutObj));

    paramsFormat.append(timeoutObj);

    JsonObject exitIfFailObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamBoolRequired(EmitErrorIfFailParamName,
                                                               EmitErrorIfFailParamDesc,
                                                               exitIfFailObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueBool(EmitErrorIfFailDefaultValue,
                                                                   exitIfFailObj));

    paramsFormat.append(exitIfFailObj);


    if(_manageMultipleMsgs)
    {
        return initMultipleMsgsParamsFormat(paramsFormat);
    }

    return initSingleMsgParamsFormat(paramsFormat);
}

bool ACanMsgSequence::initMultipleMsgsParamsFormat(JsonArray &paramsFormat) const
{
    JsonObject expectedMsgsIds;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(ExpectedMsgsIdsParamName,
                                                                 getExpectedMsgsIdsParamDesc(),
                                                                 0,
                                                                 0,
                                                                 expectedMsgsIds,
                                                                 true));

    const Number minValue = Number::fromInt32(ExpectedMsgIdDefaultValue);

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(
                                    expectedMsgsIds,
                                    minValue,
                                    Number::fromUInt32(PeakCanConstants::Limits::MaxCmdIdValue)));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueNumber(minValue, expectedMsgsIds));

    paramsFormat.append(expectedMsgsIds);

    JsonObject expectedMsgVerifsObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringListRequired(ExpectedMsgsVerifsParamName,
                                                                     ExpectedMsgsVerifsParamDesc,
                                                                     expectedMsgVerifsObj));

    paramsFormat.append(expectedMsgVerifsObj);

    return true;
}

bool ACanMsgSequence::initSingleMsgParamsFormat(JsonArray &paramsFormat) const
{
    JsonObject expectedMsgId;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(ExpectedMsgIdParamName,
                                                                 ExpectedMsgIdParamDesc,
                                                                 0,
                                                                 0,
                                                                 expectedMsgId));

    const Number minValue = Number::fromInt32(ExpectedMsgIdDefaultValue);

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(
        expectedMsgId,
        minValue,
        Number::fromUInt32(PeakCanConstants::Limits::MaxCmdIdValue)));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueNumber(minValue, expectedMsgId));

    paramsFormat.append(expectedMsgId);

    JsonObject expectedMsgVerifsObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(ExpectedMsgVerifsParamName,
                                                                 ExpectedMsgVerifsParamDesc,
                                                                 expectedMsgVerifsObj));

    paramsFormat.append(expectedMsgVerifsObj);

    return true;
}

bool ACanMsgSequence::initOutputsFormat(JsonArray &outputsFormat) const
{
    if(_exportReceivedMsgs)
    {
        JsonObject outputReceivedAnswers;
        RETURN_IF_FALSE(BankJsonFormatter::formatInOutStringArray(OutputReceivedAnswersName,
                                                                  outputReceivedAnswers,
                                                                  OutputReceivedAnswersDesc));

        outputsFormat.append(outputReceivedAnswers);
    }

    return true;
}

QString ACanMsgSequence::getExpectedMsgsIdsParamDesc() const
{
    return ExpectedMsgsIdsParamDesc;
}

bool ACanMsgSequence::validateAndSetParameters(const JsonArray &parameters, JsonArray &errors)
{
    RETURN_IF_FALSE(DefaultSequenceInstanceModule::validateAndSetParameters(parameters, errors));

    CanMsgSeqParams params;

    RETURN_IF_FALSE(parseInfoFromParameters(parameters, params, errors));

    const PCanSequenceInfo *sequenceInfo = PCanSequenceInfo::parsePCanSequenceInfo(
        params,
        _manageMultipleMsgs);

    if(sequenceInfo == nullptr)
    {
        return false;
    }

    delete _sequenceInfo;
    _sequenceInfo = sequenceInfo;
    return true;
}

bool ACanMsgSequence::parseInfoFromParameters(const JsonArray &parameters,
                                              CanMsgSeqParams &canMsgSeqParams,
                                              JsonArray &errors)
{
    QString error;

    if(!BankJsonParser::getBoolValue(parameters,
                                      EmitErrorIfFailParamName,
                                      _exitIfFails,
                                      &error,
                                      true))
    {
        fillErrorArray(EmitErrorIfFailParamName, error, errors);
        return false;
    }

    QString testGroupName;
    if(!BankJsonParser::getStringValue(parameters,
                                       TestGroupNameParamName,
                                       testGroupName,
                                       &error,
                                       true))
    {
        fillErrorArray(TestGroupNameParamName, error, errors);
        return false;
    }

    canMsgSeqParams.setTestGroupName(testGroupName);

    Number tmpTimeout;
    if(!BankJsonParser::getNumberValue(parameters,
                                        ExpectingMsgTimeoutParamName,
                                        tmpTimeout,
                                        &error,
                                        true))
    {
        fillErrorArray(ExpectingMsgTimeoutParamName, error, errors);
        return false;
    }

    if(tmpTimeout.isValid())
    {
        canMsgSeqParams.setTimeoutInMs(tmpTimeout.toInt32());
    }
    else
    {
        canMsgSeqParams.setTimeoutInMs(ExpectingMsgTimeoutDefaultValue);
    }

    if(_manageMultipleMsgs)
    {
        return parseMultipleMsgsInfoFromParameters(parameters, canMsgSeqParams, errors);
    }

    return parseSingleMsgInfoFromParameters(parameters, canMsgSeqParams, errors);
}

bool ACanMsgSequence::parseMultipleMsgsInfoFromParameters(const JsonArray &parameters,
                                                          CanMsgSeqParams &canMsgSeqParams,
                                                          JsonArray &errors)
{
    Q_UNUSED(canMsgSeqParams)

    QString error;

    QVector<Number> expectedMsgsIds;
    if(!BankJsonParser::getArrayNumberValue(parameters,
                                             ExpectedMsgsIdsParamName,
                                             expectedMsgsIds,
                                             &error,
                                             true))
    {
        fillErrorArray(ExpectedMsgsIdsParamName, error, errors);
        return false;
    }

    canMsgSeqParams.setExpectedMsgsIds(expectedMsgsIds);

    QVector<QString> verifs;
    if(!BankJsonParser::getArrayStringValue(parameters,
                                             ExpectedMsgsVerifsParamName,
                                             verifs,
                                             &error,
                                             true))
    {
        fillErrorArray(ExpectedMsgsVerifsParamName, error, errors);
        return false;
    }

    canMsgSeqParams.setVerifs(verifs);

    return true;
}

bool ACanMsgSequence::parseSingleMsgInfoFromParameters(const JsonArray &parameters,
                                                       CanMsgSeqParams &canMsgSeqParams,
                                                       JsonArray &errors)
{
    Q_UNUSED(canMsgSeqParams)

    QString error;

    Number expectedMsgId;
    if(!BankJsonParser::getNumberValue(parameters,
                                        ExpectedMsgIdParamName,
                                        expectedMsgId,
                                        &error,
                                        true))
    {
        fillErrorArray(ExpectedMsgIdParamName, error, errors);
        return false;
    }

    QString verifObj;
    if(!BankJsonParser::getStringValue(parameters,
                                        ExpectedMsgVerifsParamName,
                                        verifObj,
                                        &error,
                                        true))
    {
        fillErrorArray(ExpectedMsgVerifsParamName, error, errors);
        return false;
    }

    if(expectedMsgId.isValid())
    {
        canMsgSeqParams.setExpectedMsgsIds({ expectedMsgId });
    }

    if(!verifObj.isEmpty())
    {
        canMsgSeqParams.setVerifs({ verifObj });
    }

    return true;
}

bool ACanMsgSequence::manageOutputFilling(const QVector<QCanBusFrame> &receivedFrames,
                                            JsonArray &output)
{
    if(!_exportReceivedMsgs)
    {
        // Nothing to export
        return true;
    }

    QVector<QString> answsers;
    for(auto citer = receivedFrames.cbegin(); citer != receivedFrames.cend(); ++citer)
    {
        answsers.append(citer->toString());
    }

    return BankJsonParser::setArrayStringValue(OutputReceivedAnswersName, answsers, output);
}

void ACanMsgSequence::processModule(const JsonArray &jsonObjectInputValues)
{
    if(!PauseAndStopHelper::managePauseAndStop(getSequenceState()))
    {
        emit moduleFinished(false, {});
        return;
    }

    JsonArray output;
    if(!doCanActions(jsonObjectInputValues, output))
    {
        emit newLog(SequenceLogFormatter::formatABusinessProblem(tr("(TR) A problem occurred while "
                                                                    "trying to write a can "
                                                                    "message")));
        emit moduleFinished(false, {});
        return;
    }

    emit moduleFinished(true, output);
}

bool ACanMsgSequence::doCanActions(const JsonArray &jsonObjectInputValues, JsonArray &output)
{
    if(_sequenceInfo == nullptr)
    {
        qWarning() << "We can't write the can message(s) the writing can module parameters aren't "
                      "been set";
        return false;
    }

    QString strCanBusIntf;
    PCanBusItf::Enum pCanBusIntf;
    RETURN_IF_FALSE(CanModulesUtility::extractAndParseCanBusIntf(
        jsonObjectInputValues,
        strCanBusIntf,
        pCanBusIntf));

    CanMsgProcess process;

    auto connHandle = connect(&process, &CanMsgProcess::newLog, this, &ACanMsgSequence::newLog);

    QVector<QCanBusFrame> receivedFrames;
    RETURN_IF_FALSE(process.process(pCanBusIntf, *_sequenceInfo, _exitIfFails, receivedFrames));

    disconnect(connHandle);

    return manageOutputFilling(receivedFrames, output);
}
