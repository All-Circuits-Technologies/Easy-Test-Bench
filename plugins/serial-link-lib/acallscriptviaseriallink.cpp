// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "acallscriptviaseriallink.hpp"

#include "qtseriallinklib/seriallinkintf.hpp"

#include "stringutility/stringhelper.hpp"
#include "testbedcore/logs/sequencelogformatter.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"

#include "serialpluginlibconstants.hpp"


ACallScriptViaSerialLink::ACallScriptViaSerialLink(
    const QString &moduleClassName,
    bool isMultiLine,
    const QSharedPointer<DefaultBankInterface> &bankLinked,
    QObject *parent)
    : ASerialLinkWriteRead{moduleClassName,
                           bankLinked,
                           isMultiLine,
                           QRegularExpression(ScriptCallExpectedEndScriptPattern),
                           parent}
{
}

ACallScriptViaSerialLink::ACallScriptViaSerialLink(
    const QString &moduleClassName,
    const QSharedPointer<DefaultBankInterface> &bankLinked,
    QObject *parent)
    : ASerialLinkWriteRead{moduleClassName,
                           bankLinked,
                           SerialPluginLibConstants::Defaults::MultiLine,
                           QRegularExpression(ScriptCallExpectedEndScriptPattern),
                           parent}
{
}

ACallScriptViaSerialLink::~ACallScriptViaSerialLink()
{
}

void ACallScriptViaSerialLink::setVerboseLogs(bool verboseValue)
{
    _verboseLogs = verboseValue;
}

bool ACallScriptViaSerialLink::initParamsFormat(JsonArray &paramsFormat) const
{
    RETURN_IF_FALSE(ASerialLinkWriteRead::initParamsFormat(paramsFormat));

    RETURN_IF_FALSE(addParametersFormat(ParamsManagedBySubModule::EndOfLine, paramsFormat));

    JsonObject errorsList;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringListRequired(ErrorCodesMeaningParamName,
                                                                     ErrorCodesMeaningParamDesc,
                                                                     errorsList));

    paramsFormat.append(errorsList);

    JsonObject testDescriptionObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(TestDescriptionParamName,
                                                                 TestDescriptionParamDesc,
                                                                 testDescriptionObj));

    paramsFormat.append(testDescriptionObj);

    JsonObject testedElementObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(TestedElementParamName,
                                                                 TestedElementParamDesc,
                                                                 testedElementObj));

    paramsFormat.append(testedElementObj);

    JsonObject scriptToCallObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(ScriptToCallParamName,
                                                                 ScriptToCallParamDesc,
                                                                 scriptToCallObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, scriptToCallObj));

    paramsFormat.append(scriptToCallObj);

    JsonObject emitLogsObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamBoolRequired(VerboseLogsParamName,
                                                               VerboseLogsParamDesc,
                                                               emitLogsObj));

    paramsFormat.append(emitLogsObj);

    return true;
}

bool ACallScriptViaSerialLink::validateAndSetParameters(const JsonArray &parameters,
                                                        JsonArray &errors)
{
    RETURN_IF_FALSE(ASerialLinkWriteRead::validateAndSetParameters(parameters, errors));

    QString error;

    QVector<QString> errorCodesStr;
    if(!BankJsonParser::getArrayStringValue(parameters,
                                            ErrorCodesMeaningParamName,
                                            errorCodesStr,
                                            &error,
                                            true))
    {
        fillErrorArray(ErrorCodesMeaningParamName, error, errors);
        return false;
    }

    QHash<quint8, QString> tmpErrorCodesMeaning;
    if(!parseErrorCodes(errorCodesStr, tmpErrorCodesMeaning))
    {
        fillErrorArray(ErrorCodesMeaningParamName,
                       "A problem occurred when tried to parse the error codes list",
                       errors);
        return false;
    }

    _errorCodesMeaning = tmpErrorCodesMeaning;

    if(!BankJsonParser::getStringValue(parameters,
                                       TestDescriptionParamName,
                                       _testDescription,
                                       &error,
                                       true))
    {
        fillErrorArray(TestDescriptionParamName, error, errors);
        return false;
    }

    if(!BankJsonParser::getStringValue(parameters,
                                       ScriptToCallParamName,
                                       _scriptToCall,
                                       &error))
    {
        fillErrorArray(ScriptToCallParamName, error, errors);
        return false;
    }

    QString testedElement = QString(TestedElementDefaultValue).arg(_scriptToCall);

    if(!BankJsonParser::getStringValue(parameters,
                                        TestedElementParamName,
                                        testedElement,
                                        &error,
                                        true))
    {
        fillErrorArray(TestedElementParamName, error, errors);
        return false;
    }

    _testedElement = testedElement;

    if(!BankJsonParser::getBoolValue(parameters,
                                      VerboseLogsParamName,
                                      _verboseLogs,
                                      &error,
                                      true))
    {
        fillErrorArray(VerboseLogsParamName, error, errors);
        return false;
    }

    return true;
}

bool ACallScriptViaSerialLink::manageWriteReadProcess(
    const JsonArray &jsonObjectInputValues,
    const QSharedPointer<SerialLinkIntf> &serialIntf,
    SerialLinkReceivedReplyType::Enum &/*receivedReply*/,
    QVector<QString> &capturedElements)
{
    QString tmpScriptToCall;
    RETURN_IF_FALSE(manageScriptToCall(jsonObjectInputValues, _scriptToCall, tmpScriptToCall));

    RETURN_IF_FALSE(manageCommandSending(serialIntf,
                                         QString(GlobalFormatScriptCall).arg(tmpScriptToCall),
                                         capturedElements));

    if(capturedElements.length() < MinCapturedGroupsNb)
    {
        qWarning() << "A problem occurred for the module call script via serial link, the captured "
                   << "groups list hasn't the right length, we expect: " << MinCapturedGroupsNb;
        return false;
    }

    // The last captured group is the error code returned by script
    bool ok = false;
    quint8 errorCode = StringHelper::toQuint8(capturedElements.last(),
                                              &ok);

    if(!ok)
    {
        qWarning() << "A problem occurred for the module call script via serial link, we failed to "
                   << "get the error code, element captured" << capturedElements.last();
        return false;
    }

    if(errorCode != 0)
    {
        if(_errorCodesMeaning.contains(errorCode))
        {
            emit newLog(SequenceLogFormatter::formatTest(false,
                                                         _testedElement,
                                                         tr("(TR) An error occurred when calling "
                                                            "the linked script. The error code is "
                                                            "%1, details of the error: %2.")
                                                             .arg(errorCode)
                                                             .arg(_errorCodesMeaning[errorCode])));
        }
        else
        {
            emit newLog(SequenceLogFormatter::formatTest(false,
                                                         _testedElement,
                                                         tr("(TR) An error occurred when calling "
                                                            "the linked script. The error code is "
                                                            "%1.")
                                                             .arg(errorCode)));
        }
    }

    return manageCapturedElements(errorCode, capturedElements);
}

bool ACallScriptViaSerialLink::manageCapturedElements(
    quint8 errorCode,
    const QVector<QString> &/*capturedElements*/)
{
    if(errorCode != 0)
    {
        // A problem occurred in the script call
        // If the user has asked to emit an error if failed, we return false; if the user has asked
        // to not emit an error, we return true as if no problem occurred
        return isEmittingErrorIfFailed() ? false : true;
    }

    if(_verboseLogs == true)
    {
        emit newLog(SequenceLogFormatter::formatTest(true, _testedElement, _testDescription));
    }
    return true;
}

bool ACallScriptViaSerialLink::manageScriptToCall(const JsonArray &/*jsonObjectInputValues*/,
                                                  const QString &scriptToCall,
                                                  QString &managedScriptToCall)
{
    managedScriptToCall = scriptToCall;
    return true;
}

bool ACallScriptViaSerialLink::setExpectedPattern(const QString &pattern)
{
    QString mergedPattern;

    if(pattern.isEmpty())
    {
        mergedPattern = ScriptCallExpectedEndScriptPattern;
    }
    else
    {
        mergedPattern = QString(MergePatterns).arg(pattern)
                                              .arg(ScriptCallExpectedEndScriptPattern);
    }

    return ASerialLinkWriteRead::setExpectedPattern(mergedPattern);
}

bool ACallScriptViaSerialLink::parseErrorCodes(const QVector<QString> &errorCodes,
                                               QHash<quint8, QString> &parsedErrorCodes)
{
    for(auto citer = errorCodes.cbegin(); citer != errorCodes.cend(); ++citer)
    {
        QStringList errorSplitted = citer->split(ErrorCodeSeparator);
        if(errorSplitted.length() != ErrorCodePartsLength)
        {
            qWarning() << "The error code element got from parameter isn't well formatted: "
                       << *citer << "; its size isn't equal to " << ErrorCodePartsLength;
            return false;
        }

        bool ok = false;
        quint8 errorCode = StringHelper::toQuint8(errorSplitted.at(ErrorCodePartIdx), &ok);

        if(!ok)
        {
            qWarning() << "The error code part got from parameter isn't a quint8: " << *citer
                       << ", we can't proceed";
            return false;
        }

        parsedErrorCodes.insert(errorCode, errorSplitted.at(ErrorCodeMeaningPartIdx));
    }

    return true;
}
