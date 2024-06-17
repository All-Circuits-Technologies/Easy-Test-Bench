// SPDX-FileCopyrightText: 2023 Julien Lemasson <julien.lemasson@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "detectfromscriptlogs.hpp"

#include "serial-link-lib/serialpluginlibconstants.hpp"
#include "testbedcore/logs/sequencelogformatter.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"

#include "managers/seriallinkbankglobalmanager.hpp"


DetectFromScriptLogs::DetectFromScriptLogs(QObject *parent)
    : ACallScriptViaSerialLink{staticMetaObject.className(),
                               true,
                               SerialLinkBankGlobalManager::Instance(),
                               parent},
    MixinBoardDetection<SerialDetectSharedData>{}
{
    setVerboseLogs(false);
}

DetectFromScriptLogs::~DetectFromScriptLogs()
{
}

bool DetectFromScriptLogs::initParamsFormat(JsonArray &paramsFormat) const
{
    RETURN_IF_FALSE(ACallScriptViaSerialLink::initParamsFormat(paramsFormat));
    RETURN_IF_FALSE(initDetectParamsFormat(paramsFormat));

    JsonObject expectedValuePatternObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(
        ExpectedValuePatternParamName,
        SerialPluginLibConstants::Json::OneCapturingRegExpDesc,
        expectedValuePatternObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamPatternRegex(
        SerialPluginLibConstants::Defaults::OneCapturingParamRegExp,
        expectedValuePatternObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, expectedValuePatternObj));

    paramsFormat.append(expectedValuePatternObj);

    JsonObject expectedValueObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(ExpectedValueParamName,
                                                                 ExpectedValueParamDesc,
                                                                 expectedValueObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, expectedValueObj));

    paramsFormat.append(expectedValueObj);

    return true;
}

bool DetectFromScriptLogs::validateAndSetParameters(const JsonArray &parameters,
                                                        JsonArray &errors)
{
    RETURN_IF_FALSE(ACallScriptViaSerialLink::validateAndSetParameters(parameters, errors));
    RETURN_IF_FALSE(validateAndSetDetectParameters(parameters, errors));

    QString error;

    QString expectedValuePattern;
    if(!BankJsonParser::getStringValue(parameters,
                                        ExpectedValuePatternParamName,
                                        expectedValuePattern,
                                        &error))
    {
        fillErrorArray(ExpectedValuePatternParamName, error, errors);
        return false;
    }

    if(!setExpectedPattern(expectedValuePattern))
    {
        fillErrorArray(ExpectedValuePatternParamName,
                       QString("A problem occurred when tried to set the expected param: %1")
                           .arg(expectedValuePattern),
                       errors);
        return false;
    }

    if(!BankJsonParser::getStringValue(parameters,
                                       ExpectedValueParamName,
                                       _expectedValue,
                                       &error))
    {
        fillErrorArray(ExpectedValueParamName, error, errors);
        return false;
    }

    return true;
}

bool DetectFromScriptLogs::manageWriteReadProcess(const JsonArray &jsonObjectInputValues,
                                                  const QSharedPointer<SerialLinkIntf> &serialIntf,
                                                  SerialLinkReceivedReplyType::Enum &receivedReply,
                                                  QVector<QString> &capturedElements)
{
    SerialDetectSharedData sharedData{jsonObjectInputValues,
                                      serialIntf,
                                      receivedReply,
                                      capturedElements};
    return detectBoard(sharedData, *this);
}

bool DetectFromScriptLogs::boardDetectionProcess(const SerialDetectSharedData &sharedData,
                                                 bool &isDetected)
{
    QVector<QString> &capturedElements = sharedData.getCapturedElements();
    if(!ACallScriptViaSerialLink::manageWriteReadProcess(sharedData.getJsonObjectInputValues(),
                                                         sharedData.getSerialIntf(),
                                                         sharedData.getReceivedReply(),
                                                         capturedElements))
    {
        // Useless to go forward if a problem occurred, but we still want to continue in the board
        // detection process
        capturedElements.clear();
        return true;
    }

    manageDetectElements(sharedData.getCapturedElements(), &isDetected);
    capturedElements.clear();

    return true;
}

bool DetectFromScriptLogs::manageDetectElements(const QVector<QString> &capturedElements,
                                                  bool *detectionStatus)
{

    const bool hasCapturedScriptValue = capturedElements.length() == ExpectedCapturedListLength;
    const QString catchValue = hasCapturedScriptValue ?
                                   capturedElements.at(ExpectedValueIdxInCapturedList) :
                                   QString();

    if(catchValue.isEmpty())
    {
        qWarning() << "A problem occurred in the compare value from script logs, there is "
                   << "no captured value or it can't be parsed: " << capturedElements;
        emit newLog(SequenceLogFormatter::formatAnApplicationProblem("The script hasn't returned "
                                                                     "any value. Check the "
                                                                     "RegExp given to the module "
                                                                     "or the script."));
        return false;
    }

    if(catchValue == _expectedValue)
    {
        *detectionStatus = true;
    }
    else
    {
        *detectionStatus = false;
    }

    return true;
}
