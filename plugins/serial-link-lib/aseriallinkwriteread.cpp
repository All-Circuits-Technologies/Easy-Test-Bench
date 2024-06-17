// SPDX-FileCopyrightText: 2020 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "aseriallinkwriteread.hpp"

#include <QDebug>
#include <QSerialPort>

#include "qtseriallinklib/seriallinkintf.hpp"
#include "qtseriallinklib/seriallinkmanager.hpp"

#include "testbedcore/logs/sequencelogformatter.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/utility/pauseandstophelper.hpp"
#include "waitutility/waithelper.hpp"

#include "processes/messagereceptionprocess.hpp"

const QRegularExpression ASerialLinkWriteRead::DefaultExpectedReply = QRegularExpression("..*");
const QRegularExpression ASerialLinkWriteRead::DefaultPossibleFailure = QRegularExpression(
    QString());


ASerialLinkWriteRead::ASerialLinkWriteRead(const QString &moduleClassName,
                                           const QSharedPointer<DefaultBankInterface> &bankLinked,
                                           QObject *parent) :
    DefaultSequenceInstanceModule(moduleClassName, bankLinked, parent),
    _expectedReply{DefaultExpectedReply},
    _isMultiLineTest{SerialPluginLibConstants::Defaults::MultiLine}
{
}

ASerialLinkWriteRead::ASerialLinkWriteRead(const QString &moduleClassName,
                                           const QSharedPointer<DefaultBankInterface> &bankLinked,
                                           bool isMultiLine,
                                           const QRegularExpression &expectedReply,
                                           QObject *parent) :
    DefaultSequenceInstanceModule(moduleClassName, bankLinked, parent),
    _expectedReply{expectedReply},
    _isMultiLineTest{isMultiLine}
{
}

bool ASerialLinkWriteRead::initInputsFormat(JsonArray &inputsFormat) const
{
    JsonObject portStringInVar;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(
        SerialPluginLibConstants::Json::PortName,
        portStringInVar,
        SerialPluginLibConstants::Json::PortNameDesc));
    inputsFormat.append(portStringInVar);

    return true;
}

bool ASerialLinkWriteRead::initParamsFormat(JsonArray &paramsFormat) const
{
    JsonObject TimeoutMsUIntParam;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(
        SerialPluginLibConstants::Json::Timeout,
        TimeoutParamDesc,
        0,
        0,
        TimeoutMsUIntParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(
        TimeoutMsUIntParam,
        Number(1, 0, false),
        Number::maxInt32(),
        Number(1)));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueNumber(
        Number::fromInt32(DefaultTimeoutInMs),
        TimeoutMsUIntParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, TimeoutMsUIntParam));

    paramsFormat.append(TimeoutMsUIntParam);


    JsonObject emitErrorIfFailParam;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamBoolRequired(
        SerialPluginLibConstants::Json::EmitErrorIfFail,
        EmitErrorIfFailParamDesc,
        emitErrorIfFailParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueBool(DefaultEmitErrorIfFail,
                                                                   emitErrorIfFailParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, emitErrorIfFailParam));

    paramsFormat.append(emitErrorIfFailParam);

    return true;
}

bool ASerialLinkWriteRead::validateAndSetParameters(const JsonArray &parameters, JsonArray &errors)
{
    RETURN_IF_FALSE(DefaultSequenceInstanceModule::validateAndSetParameters(parameters, errors));

    QString parserError;

    /* Timeout */
    Number timeoutNumber = Number::fromInt32(_timeoutMs);
    if(!BankJsonParser::getNumberValue(parameters,
                                        SerialPluginLibConstants::Json::Timeout,
                                       timeoutNumber,
                                       &parserError,
                                       true))
    {
        fillErrorArray(SerialPluginLibConstants::Json::Timeout, parserError, errors);
        return false;
    }

    _timeoutMs = timeoutNumber.toInt32();

    /* ExitIfError */
    if(!BankJsonParser::getBoolValue(parameters,
                                      SerialPluginLibConstants::Json::EmitErrorIfFail,
                                     _emitErrorIfFail,
                                     &parserError,
                                     true))
    {
        fillErrorArray(SerialPluginLibConstants::Json::EmitErrorIfFail, parserError, errors);
        return false;
    }

    /* End of line */
    if(!BankJsonParser::getStringValue(parameters,
                                        SerialPluginLibConstants::Json::EndOfLine,
                                       _eol,
                                       &parserError,
                                       true))
    {
        fillErrorArray(SerialPluginLibConstants::Json::EndOfLine, parserError, errors);
        return false;
    }

    /* Trim */
    if(!BankJsonParser::getBoolValue(parameters,
                                     SerialPluginLibConstants::Json::Trim,
                                     _trim,
                                     &parserError,
                                     true))
    {
        fillErrorArray(SerialPluginLibConstants::Json::Trim, parserError, errors);
        return false;
    }

    /* Multiline test */
    if(!BankJsonParser::getBoolValue(parameters,
                                     SerialPluginLibConstants::Json::IsMultiLineTest,
                                     _isMultiLineTest,
                                     &parserError,
                                     true))
    {
        fillErrorArray(SerialPluginLibConstants::Json::IsMultiLineTest, parserError, errors);
        return false;
    }

    /* Expected reply pattern */
    QString expectedPattern = _expectedReply.pattern();
    if(!BankJsonParser::getStringValue(parameters,
                                        SerialPluginLibConstants::Json::ExpectedPattern,
                                       expectedPattern,
                                       &parserError,
                                       true))
    {
        fillErrorArray(SerialPluginLibConstants::Json::ExpectedPattern, parserError, errors);
        return false;
    }

    _expectedReply.setPattern(expectedPattern);

    if(!_expectedReply.isValid())
    {
        fillErrorArray(SerialPluginLibConstants::Json::ExpectedPattern,
                       tr("(TR) Given expected pattern is invalid"),
                       errors);
        return false;
    }

    /* Known possible failure replies */
    QString failurePattern = _possibleFailure.pattern();
    if(!BankJsonParser::getStringValue(parameters,
                                       SerialPluginLibConstants::Json::FailurePattern,
                                       failurePattern,
                                       &parserError,
                                       true))
    {
        fillErrorArray(SerialPluginLibConstants::Json::FailurePattern, parserError, errors);
        return false;
    }

    _possibleFailure.setPattern(failurePattern);
    if(!failurePattern.isEmpty() && !_possibleFailure.isValid())
    {
        fillErrorArray(SerialPluginLibConstants::Json::FailurePattern,
                       tr("(TR) Failure pattern must be either empty or valid"),
                       errors);
        return false;
    }

    /* Open mode coherency */
    _openMode = ReadWrite;
    if(expectedPattern.isEmpty())
    {
        _openMode = WriteOnly;
    }

    if(_openMode != WriteOnly)
    {
        if(_timeoutMs == 0)
        {
            fillErrorArray(SerialPluginLibConstants::Json::Timeout,
                           tr("(TR) Timeout can not be zero when expecting a reply"),
                           errors);
            return false;
        }
    }

    return true;
}

void ASerialLinkWriteRead::processModule(const JsonArray &jsonObjectInputValues)
{
    if(!PauseAndStopHelper::managePauseAndStop(getSequenceState()))
    {
        emit moduleFinished(false, {});
        return;
    }

    JsonArray jsonOutputValues;
    bool success = manageModuleProcess(jsonObjectInputValues, jsonOutputValues);

    emit moduleFinished(success, jsonOutputValues);
}

bool ASerialLinkWriteRead::manageModuleProcess(const JsonArray &jsonObjectInputValues,
                                               JsonArray &jsonOutputValues)
{
    /* How this method works:
     *  - It registers to serial port received data first,
     *  - Then it sends message to it
     *  - And wait for any of success or failure pattern or timeout
     */

    /* Fetch inputs */
    QSharedPointer<SerialLinkIntf> serialIntf = getSerialIntfFromInput(jsonObjectInputValues);

    if(serialIntf == nullptr)
    {
        return false;
    }

    RETURN_IF_FALSE(PauseAndStopHelper::managePauseAndStop(getSequenceState()));

    SerialLinkReceivedReplyType::Enum receivedReply = SerialLinkReceivedReplyType::NoAnswer;
    QVector<QString> capturedElements;
    RETURN_IF_FALSE(manageWriteReadProcess(jsonObjectInputValues,
                                           serialIntf,
                                           receivedReply,
                                           capturedElements));

    return manageModuleOutput(receivedReply, capturedElements, jsonOutputValues);
}

bool ASerialLinkWriteRead::manageModuleOutput(
    SerialLinkReceivedReplyType::Enum /*receivedReply*/,
    const QVector<QString> &/*capturedElements*/,
    JsonArray &/*jsonOutputValues*/)
{
    return true;
}

bool ASerialLinkWriteRead::manageCommandSending(const QSharedPointer<SerialLinkIntf> &serialIntf,
                                                const QString &messageToSend,
                                                QVector<QString> &capturedElements,
                                                bool ignoreStoppingInProcess)
{
    RETURN_IF_FALSE(PauseAndStopHelper::managePauseAndStop(getSequenceState()));

    bool serialPortOpenedByMe = false;
    RETURN_IF_FALSE(manageIntfOpen(serialIntf, serialPortOpenedByMe));

    bool success = false;

    if(_openMode == WriteOnly)
    {
        if(messageToSend.isNull())
        {
            qWarning() << "The message to send can't be null (it may be empty, but not null)";
            return false;
        }

        success = sendCommand(serialIntf, messageToSend);
    }
    else
    {
        SerialLinkReceivedReplyType::Enum receivedReply;
        success = manageReadAndOrWriteCmd(serialIntf,
                                          messageToSend,
                                          ignoreStoppingInProcess,
                                          receivedReply,
                                          capturedElements);
    }

    /* Close port if opened on purpose */
    if(serialPortOpenedByMe)
    {
        serialIntf->close();
    }

    return success;
}

bool ASerialLinkWriteRead::setExpectedPattern(const QString &expectedPattern)
{
    _expectedReply.setPattern(expectedPattern);

    if(!_expectedReply.isValid())
    {
        qWarning() << "The expected pattern: " << expectedPattern << ", is not a valid Regex";
        return false;
    }

    return true;
}

bool ASerialLinkWriteRead::setFailurePattern(const QString &failurePattern)
{
    _possibleFailure.setPattern(failurePattern);

    if(!_possibleFailure.isValid())
    {
        qWarning() << "The failure pattern: " << failurePattern << ", is not a valid Regex";
        return false;
    }

    return true;
}

bool ASerialLinkWriteRead::addParametersFormat(int paramsManaged, JsonArray &parametersFormat)
{
    /* expectedPattern:string */
    if((paramsManaged & ExpectedPattern) != 0)
    {
        JsonObject expectedPatternStringParam;
        RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(
            SerialPluginLibConstants::Json::ExpectedPattern,
                                    ExpectedPatternParamDesc,
                                    expectedPatternStringParam));

        RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueString(
            DefaultExpectedReply.pattern(),
                                                                    expectedPatternStringParam));

        RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, expectedPatternStringParam));

        parametersFormat.append(expectedPatternStringParam);
    }

    /* failurePattern:string */
    if((paramsManaged & FailurePattern) != 0)
    {
        JsonObject failurePatternStringParam;
        RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(
                                   SerialPluginLibConstants::Json::FailurePattern,
                                   FailurePatternParamDesc,
                                   failurePatternStringParam));

        RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueString(
                                                                DefaultPossibleFailure.pattern(),
                                                                failurePatternStringParam));

        RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, failurePatternStringParam));

        parametersFormat.append(failurePatternStringParam);
    }

    /* end-of-line:string */
    if((paramsManaged & EndOfLine) != 0)
    {
        JsonObject endOfLineStringParam;
        RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(
            SerialPluginLibConstants::Json::EndOfLine,
            SerialPluginLibConstants::Json::EndOfLineDesc,
            endOfLineStringParam));

        RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueString(
            SerialPluginLibConstants::Defaults::EndOfLine,
            endOfLineStringParam));

        RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, endOfLineStringParam));

        parametersFormat.append(endOfLineStringParam);
    }

    /* trim:bool */
    if((paramsManaged & Trim) != 0)
    {
        JsonObject trimBoolParam;
        RETURN_IF_FALSE(BankJsonFormatter::formatParamBoolRequired(
            SerialPluginLibConstants::Json::Trim,
            SerialPluginLibConstants::Json::TrimDesc,
            trimBoolParam));

        RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueBool(
            SerialPluginLibConstants::Defaults::Trim,
            trimBoolParam));

        RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, trimBoolParam));

        parametersFormat.append(trimBoolParam);
    }

    /* multiline:bool */
    if((paramsManaged & IsMultiLineTest) != 0)
    {
        JsonObject multiLineBoolParam;
        RETURN_IF_FALSE(BankJsonFormatter::formatParamBoolRequired(
            SerialPluginLibConstants::Json::IsMultiLineTest,
            SerialPluginLibConstants::Json::IsMultiLineTestDesc,
            multiLineBoolParam));

        RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueBool(
            SerialPluginLibConstants::Defaults::MultiLine,
            multiLineBoolParam));

        RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, multiLineBoolParam));

        parametersFormat.append(multiLineBoolParam);
    }

    return true;
}

bool ASerialLinkWriteRead::addInputsFormat(int inputsToGet,
                                           JsonArray &inputsFormat)
{
    /* message:string */
    if((inputsToGet & Message) != 0)
    {
        JsonObject messageStringInVar;
        RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(
                                                            SerialPluginLibConstants::Json::Message,
                                                            messageStringInVar,
                                                            InputMessageDesc));
        inputsFormat.append(messageStringInVar);
    }

    return true;
}

bool ASerialLinkWriteRead::addOutputsFormat(int outputsToGet, JsonArray &outputsFormat)
{
    /* found:bool */
    if((outputsToGet & Found) != 0)
    {
        JsonObject foundBoolOutVar;
        RETURN_IF_FALSE(BankJsonFormatter::formatInOutBool(SerialPluginLibConstants::Json::Found,
                                                           foundBoolOutVar,
                                                           OutputFoundDesc));
        outputsFormat.append(foundBoolOutVar);
    }

    /* captured groups:List<string> */
    if((outputsToGet & CaptureGroups) != 0)
    {
        JsonObject captureGroupsOutVar;
        RETURN_IF_FALSE(BankJsonFormatter::formatInOutStringArray(
            SerialPluginLibConstants::Json::CapturedGroup,
            captureGroupsOutVar,
            OutputCaptureGroupsDesc));
        outputsFormat.append(captureGroupsOutVar);
    }

    return true;
}

bool ASerialLinkWriteRead::getMessageFromInput(const JsonArray &jsonObjectInputValues,
                                               QString &message)
{
    if(!BankJsonParser::getStringValue(jsonObjectInputValues,
                                        SerialPluginLibConstants::Json::Message,
                                        message))
    {
        emit newLog(SequenceLogFormatter::formatAnApplicationProblem(tr("(TR) Missing required "
                                                                        "message input")));
        return false;
    }

    return true;
}

bool ASerialLinkWriteRead::setInfoToOutput(int outputsToAdd,
                                           SerialLinkReceivedReplyType::Enum receivedReply,
                                           const QVector<QString> &capturedElements,
                                           JsonArray &jsonOutputValues)
{
    if((outputsToAdd & Found) != 0)
    {
        RETURN_IF_FALSE(BankJsonParser::setBoolValue(
            SerialPluginLibConstants::Json::Found,
            receivedReply == SerialLinkReceivedReplyType::ExpectedAnswer,
            jsonOutputValues));
    }

    if((outputsToAdd & CaptureGroups) != 0)
    {
        RETURN_IF_FALSE(BankJsonParser::setArrayStringValue(
            SerialPluginLibConstants::Json::CapturedGroup,
            capturedElements,
            jsonOutputValues));
    }

    return true;
}

QSharedPointer<SerialLinkIntf> ASerialLinkWriteRead::getSerialIntfFromInput(
    const JsonArray &jsonObjectInputValues)
{
    QString linkName;
    if(!BankJsonParser::getStringValue(jsonObjectInputValues,
                                        SerialPluginLibConstants::Json::PortName,
                                        linkName))
    {
        emit newLog(SequenceLogFormatter::formatAnApplicationProblem(tr("(TR) Missing required "
                                                                        "serial port name input")));
        return nullptr;
    }

    /* Parse inputs */
    const QSharedPointer<SerialLinkIntf> serialItf = SerialLinkManager::getInstance()
                                                         .getSerialLink(linkName);
    if(serialItf.isNull())
    {
        emit newLog(SequenceLogFormatter::formatAnApplicationProblem(tr("(TR) Serial port %1 is "
                                                                        "not yet set up")
                                                                         .arg(linkName)));
        qWarning() << "No link found with the name given: " << linkName;
        return nullptr;
    }

    return serialItf;
}

bool ASerialLinkWriteRead::manageIntfOpen(const QSharedPointer<SerialLinkIntf> &serialIntf,
                                          bool &serialPortOpenedByMe)
{
    if(!serialIntf->isOpen())
    {
        RETURN_IF_FALSE(PauseAndStopHelper::managePauseAndStop(getSequenceState()));

        if(!serialIntf->open(static_cast<QIODevice::OpenMode>(_openMode)))
        {
            const QString linkName = serialIntf->getIntfName();
            emit newLog(SequenceLogFormatter::formatABusinessProblem(tr("(TR) Serial port %1 is "
                                                                        "not yet set up")
                                                                         .arg(linkName)));
            qWarning() << "A problem occured when tryied to open the port: " << linkName;
            return false;
        }

        serialPortOpenedByMe = true;
    }
    else
    {
        serialPortOpenedByMe = false;
    }

    if(!PauseAndStopHelper::managePauseAndStop(getSequenceState()))
    {
        if(serialPortOpenedByMe)
        {
            serialIntf->close();
        }

        return false;
    }

    return true;
}

bool ASerialLinkWriteRead::sendCommand(const QSharedPointer<SerialLinkIntf> &serialIntf,
                                       const QString &messageToSend)
{
    if(!serialIntf->send((messageToSend + _eol).toLatin1()))
    {
        const QString linkName = serialIntf->getIntfName();
        emit newLog(SequenceLogFormatter::formatABusinessProblem(tr("(TR) A problem occurred when "
                                                                    "tried to send message to "
                                                                    "serial port: %1")
                                                                     .arg(linkName)));
        qWarning() << "A problem occured when tryied to send message to serial port: "
                   << linkName;

        return false;
    }

    return true;
}

bool ASerialLinkWriteRead::manageReadAndOrWriteCmd(const QSharedPointer<SerialLinkIntf> &serialIntf,
                                                   const QString &messageToSend,
                                                   bool ignoreStoppingInProcess,
                                                   SerialLinkReceivedReplyType::Enum &receivedReply,
                                                   QVector<QString> &capturedElements)
{
    receivedReply = SerialLinkReceivedReplyType::NoAnswer;

    // Trash old RX data to not mistakenly see it as a reply to the command we will send
    serialIntf->flushRx();

    RETURN_IF_FALSE(PauseAndStopHelper::managePauseAndStop(getSequenceState()));

    QStringList receivedLines;
    QString tmpBuffer;
    MessageReceptionProcess receptionProcess{_eol, _trim, _isMultiLineTest};

    auto testIfAnswerWaitingIsEnded = [&receivedReply, &ignoreStoppingInProcess, this]()
    {
        return (receivedReply != SerialLinkReceivedReplyType::NoAnswer ||
                (!ignoreStoppingInProcess &&
                 !PauseAndStopHelper::managePauseAndStop(getSequenceState())));
    };

    auto onReceivedData = [this,
                           &receivedLines,
                           &receptionProcess,
                           &tmpBuffer,
                           &capturedElements,
                           &receivedReply,
                           &testIfAnswerWaitingIsEnded]
        (const QByteArray &data)
    {
        if(testIfAnswerWaitingIsEnded())
        {
            // No more need to manage data
            return;
        }

        receivedReply = receptionProcess.parseAndManageReceivedMessage(
            data,
            _expectedReply,
            receivedLines,
            tmpBuffer,
            capturedElements,
            _possibleFailure);
    };

    // Already start listening for future replies (before sending request)
    QMetaObject::Connection dataReceivedConn = connect(serialIntf.data(),
                                                       &SerialLinkIntf::dataReceived,
                                                       this,
                                                       onReceivedData);

    if(!messageToSend.isNull())
    {
        if(!sendCommand(serialIntf, messageToSend))
        {
            disconnect(dataReceivedConn);
            return false;
        }
    }

    if(!WaitHelper::pseudoWait(testIfAnswerWaitingIsEnded, _timeoutMs))
    {
        disconnect(dataReceivedConn);
        return false;
    }

    disconnect(dataReceivedConn);

    RETURN_IF_FALSE(PauseAndStopHelper::managePauseAndStop(getSequenceState()));

    return true;
}
