// SPDX-FileCopyrightText: 2023 Julien Lemasson <julien.lemasson@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "CompareCanMsg.hpp"

#include "peak-can-lib/peakcanlibconstants.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/utility/pauseandstophelper.hpp"
#include "testbedcore/logs/sequencelogformatter.hpp"

#include "peak-can-lib/canmodulesutility.hpp"

#include "models/canmsgseqparams.hpp"
#include "models/comparemsg/comparecansequenceinfo.hpp"
#include "peakcanconstants.hpp"
#include "process/comparemsgprocess.hpp"
#include "peakcanbankglobalmanager.hpp"


CompareCanMsg::CompareCanMsg(QObject *parent)
    : DefaultSequenceInstanceModule{staticMetaObject.className(),
                                    PeakCanBankGlobalManager::Instance(),
                                    parent},
    _msgSeqParams{new CanMsgSeqParams(this)}
{
}

CompareCanMsg::~CompareCanMsg()
{
    delete _sequenceInfo;
}

bool CompareCanMsg::initInputsFormat(JsonArray &inputsFormat) const
{
    JsonObject inputPeakCanIntf;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(
        PeakCanLibConstants::ModulesEntries::Inputs::InputPeakCanIntfName,
        inputPeakCanIntf,
        PeakCanLibConstants::ModulesEntries::Inputs::InputPeakCanIntfDesc));

    inputsFormat.append(inputPeakCanIntf);
    return true;
}

bool CompareCanMsg::initParamsFormat(JsonArray &paramsFormat) const
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

    JsonObject bitrateSwitchObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamBoolRequired(BitrateSwitchParamName,
                                                               BitrateSwitchParamDesc,
                                                               bitrateSwitchObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueBool(
        PeakCanConstants::ModulesEntries::Params::HasBitrateSwitchDefaultValue,
        bitrateSwitchObj));

    paramsFormat.append(bitrateSwitchObj);

    JsonObject extendedFrameFormatObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamBoolRequired(ExtendedFrameFormatParamName,
                                                               ExtendedFrameFormatParamDesc,
                                                               extendedFrameFormatObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueBool(
        PeakCanConstants::ModulesEntries::Params::IsExtendedFrameFormatDefaultValue,
        extendedFrameFormatObj));

    paramsFormat.append(extendedFrameFormatObj);

    return initSingleMsgParamsFormat(paramsFormat);
}

bool CompareCanMsg::validateAndSetParameters(const JsonArray &parameters, JsonArray &errors)
{
    RETURN_IF_FALSE(DefaultSequenceInstanceModule::validateAndSetParameters(parameters, errors));

    CanMsgSeqParams params;

    RETURN_IF_FALSE(parseInfoFromParameters(parameters, params, errors));

    const CompareCanSequenceInfo *sequenceInfo = CompareCanSequenceInfo::parsePCanSequenceInfo(
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

bool CompareCanMsg::initSingleMsgParamsFormat(JsonArray &paramsFormat) const
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

    JsonObject canMsgToSendObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(CanMsgToSendParamName,
                                                                 CanMsgToSendParamDesc,
                                                                 canMsgToSendObj));

    paramsFormat.append(canMsgToSendObj);


    return true;
}

bool CompareCanMsg::parseInfoFromParameters(const JsonArray &parameters,
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

    bool bitrateSwitch = PeakCanConstants::ModulesEntries::Params::HasBitrateSwitchDefaultValue;
    if(!BankJsonParser::getBoolValue(parameters,
                                     BitrateSwitchParamName,
                                     bitrateSwitch,
                                     &error,
                                     true))
    {
        fillErrorArray(BitrateSwitchParamName, error, errors);
        return false;
    }

    canMsgSeqParams.setBitrateSwitch(bitrateSwitch);

    bool extendedFrameFormat =
        PeakCanConstants::ModulesEntries::Params::IsExtendedFrameFormatDefaultValue;
    if(!BankJsonParser::getBoolValue(parameters,
                                     ExtendedFrameFormatParamName,
                                     extendedFrameFormat,
                                     &error,
                                     true))
    {
        fillErrorArray(ExtendedFrameFormatParamName, error, errors);
        return false;
    }

    canMsgSeqParams.setExtendedFrameFormat(extendedFrameFormat);

    return parseSingleMsgInfoFromParameters(parameters, canMsgSeqParams, errors);
}

bool CompareCanMsg::parseSingleMsgInfoFromParameters(const JsonArray &parameters,
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

    QString cmd;
    if(!BankJsonParser::getStringValue(parameters,
                                       CanMsgToSendParamName,
                                       cmd,
                                       &error,
                                       true))
    {
        fillErrorArray(CanMsgToSendParamName, error, errors);
        return false;
    }

    if(!cmd.isEmpty())
    {
        canMsgSeqParams.setCmds({ cmd });
    }

    return true;
}

bool CompareCanMsg::doCompareCanActions(const JsonArray &jsonObjectInputValues, JsonArray &output)
{
    Q_UNUSED(output);

    if(_sequenceInfo == nullptr)
    {
        qWarning() << "We can't compare the can message the writing can module parameters aren't "
                      "been set";
        return false;
    }

    QString strCanBusIntf;
    PCanBusItf::Enum pCanBusIntf;
    RETURN_IF_FALSE(CanModulesUtility::extractAndParseCanBusIntf(
        jsonObjectInputValues,
        strCanBusIntf,
        pCanBusIntf));

    CompareMsgProcess process;

    auto connHandle = connect(&process, &CompareMsgProcess::newLog, this, &CompareCanMsg::newLog);

    QVector<QCanBusFrame> receivedFrames;
    RETURN_IF_FALSE(process.process(pCanBusIntf, *_sequenceInfo, _exitIfFails, receivedFrames));

    disconnect(connHandle);

    return true;
}

void CompareCanMsg::processModule(const JsonArray &jsonObjectInputValues)
{
    if(!PauseAndStopHelper::managePauseAndStop(getSequenceState()))
    {
        emit moduleFinished(false, {});
        return;
    }

    JsonArray output;
    if(!doCompareCanActions(jsonObjectInputValues, output))
    {
        emit newLog(SequenceLogFormatter::formatABusinessProblem(tr("(TR) A problem occurred while "
                                                                    "trying to compare a can "
                                                                    "message")));
        emit moduleFinished(false, {});
        return;
    }

    emit moduleFinished(true, output);
}
