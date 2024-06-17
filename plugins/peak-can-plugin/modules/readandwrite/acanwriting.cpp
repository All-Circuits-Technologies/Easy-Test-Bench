// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "acanwriting.hpp"

#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"

#include "models/canmsgseqparams.hpp"
#include "peakcanconstants.hpp"


ACanWriting::ACanWriting(const QString &moduleClassName,
                         bool sendMultipleCmd,
                         bool exportReceivedAnswers,
                         QObject *parent)
    : ACanMsgSequence{moduleClassName, sendMultipleCmd, exportReceivedAnswers, parent}
{
}

ACanWriting::~ACanWriting()
{
}

bool ACanWriting::initParamsFormat(JsonArray &paramsFormat) const
{
    RETURN_IF_FALSE(ACanMsgSequence::initParamsFormat(paramsFormat));

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

    return true;
}

bool ACanWriting::initMultipleMsgsParamsFormat(JsonArray &paramsFormat) const
{
    RETURN_IF_FALSE(ACanMsgSequence::initMultipleMsgsParamsFormat(paramsFormat));

    JsonObject canMsgsToSend;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringListRequired(CanMsgsToSendParamName,
                                                                     CanMsgsToSendParamDesc,
                                                                     canMsgsToSend));

    paramsFormat.append(canMsgsToSend);

    return true;
}

bool ACanWriting::initSingleMsgParamsFormat(JsonArray &paramsFormat) const
{
    RETURN_IF_FALSE(ACanMsgSequence::initSingleMsgParamsFormat(paramsFormat));

    JsonObject canMsgToSendObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(CanMsgToSendParamName,
                                                                 CanMsgToSendParamDesc,
                                                                 canMsgToSendObj));

    paramsFormat.append(canMsgToSendObj);


    return true;
}

QString ACanWriting::getExpectedMsgsIdsParamDesc() const
{
    return QString(WriteExpectedMsgsIdsParamDesc)
        .arg(ACanMsgSequence::getExpectedMsgsIdsParamDesc());
}

bool ACanWriting::parseInfoFromParameters(const JsonArray &parameters,
                                          CanMsgSeqParams &canMsgSeqParams,
                                          JsonArray &errors)
{
    RETURN_IF_FALSE(ACanMsgSequence::parseInfoFromParameters(parameters, canMsgSeqParams, errors));

    QString error;

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

    return true;
}

bool ACanWriting::parseMultipleMsgsInfoFromParameters(const JsonArray &parameters,
                                                      CanMsgSeqParams &canMsgSeqParams,
                                                      JsonArray &errors)
{
    RETURN_IF_FALSE(ACanMsgSequence::parseMultipleMsgsInfoFromParameters(parameters,
                                                                         canMsgSeqParams,
                                                                         errors));

    QString error;

    QVector<QString> cmdsToSend;
    if(!BankJsonParser::getArrayStringValue(parameters,
                                             CanMsgsToSendParamName,
                                             cmdsToSend,
                                             &error,
                                             true))
    {
        fillErrorArray(CanMsgsToSendParamName, error, errors);
        return false;
    }

    canMsgSeqParams.setCmds(cmdsToSend);

    const int expectedMsgsLength = canMsgSeqParams.getExpectedMsgsIds().length();
    const int cmdsLength = cmdsToSend.length();
    if(cmdsLength > 0 && expectedMsgsLength > 0 && cmdsLength != expectedMsgsLength)
    {
        fillErrorArray(CanMsgsToSendParamName, QObject::tr(ExpectedMsgIdsLengthError), errors);
        return false;
    }

    return true;
}

bool ACanWriting::parseSingleMsgInfoFromParameters(const JsonArray &parameters,
                                                   CanMsgSeqParams &canMsgSeqParams,
                                                   JsonArray &errors)
{
    RETURN_IF_FALSE(ACanMsgSequence::parseSingleMsgInfoFromParameters(parameters,
                                                                      canMsgSeqParams,
                                                                      errors));

    QString error;

    QString cmd;
    if(!BankJsonParser::getStringValue(parameters,
                                        CanMsgToSendParamName,
                                        cmd,
                                        &error))
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
