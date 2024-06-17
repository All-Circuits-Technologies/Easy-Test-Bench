// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "asavinglogfile.hpp"

#include "abstractpluginelements/defaultbankglobalmanager.hpp"
#include "bankcoreshared/usebyplugin/icommoninterface.hpp"
#include "bankcoreshared/usebyplugin/isequencelogmanager.hpp"
#include "logsutility/saveloginfilesthread.hpp"
#include "testbedcore/logs/sequencelogcategory.hpp"
#include "testbedcore/logs/sequencelogformatter.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/testbedglobal.hpp"
#include "testbedcore/utility/pauseandstophelper.hpp"


ASavingLogFile::ASavingLogFile(const QString &moduleClassName, QObject *parent) :
    DefaultSequenceInstanceModule(moduleClassName,
                                  DefaultBankGlobalManager::Instance(),
                                  parent)
{
    _logFileThread = new SaveLogInFilesThread();
    if(!_logFileThread->startThreadAndWaitToBeReady())
    {
        qWarning() << "Can't start the logging thread for creating a log file";
    }
}

ASavingLogFile::~ASavingLogFile()
{
    if(_logFileThread->isLoggingStarted())
    {
        manageUnderLogFileState();
    }

    _logFileThread->stopAndDeleteThread();
}

bool ASavingLogFile::stopModule(StopAskedType::Enum stopAskedType)
{
    RETURN_IF_FALSE(DefaultSequenceInstanceModule::stopModule(stopAskedType));

    if(!_loggingStarted)
    {
        // Nothing to do here
        return true;
    }

    return _logFileThread->stopLogging();
}

// We use the recursion in purpose, to write a first log, if needed
// NOLINTNEXTLINE(misc-no-recursion)
void ASavingLogFile::writeLog(const SequenceLogData &log)
{
    LoggingOptions logsArguments;
    getLogArguments(log, logsArguments);

    if(!manageAboveLogFileState(log.getType(), logsArguments))
    {
        qWarning() << "A problem occured when tryied to manage the log file state";
    }

    if(!_isFirstLogFileWritten)
    {
        _isFirstLogFileWritten = true;
        SequenceLogData firstLog;

        firstLogToSave(firstLog);

        if(firstLog.isValid())
        {
            emit newLog(firstLog);

            //If log can't be written, cause of its category or msg type level, we force it's
            // writting

            if(!_logsCats.testFlag(firstLog.getCategory()) ||
               !LogMsgType::isEqualOrAboveCriticity(firstLog.getType(), _msgTypeLevel))
            {
                writeLog(firstLog);
            }
        }
    }

    _logFileThread->writeLog(SequenceLogFormatter::toString(log, _logsFormatType), logsArguments);
}

bool ASavingLogFile::manageLogFileStateWrittings(const QString &msgToPrepend,
                                                 const QString &toAppendToLogFileName,
                                                 const LoggingOptions &logsArguments)
{
    if(!msgToPrepend.isEmpty())
    {
        LoggingOptions tmpLoggingOptions(logsArguments);
        if(_isFirstLogFileWritten)
        {
            // Only prepend if there is already date on the file
            tmpLoggingOptions.insert(LoggingOption::AtStart, true);
        }

        _logFileThread->writeLog(msgToPrepend, tmpLoggingOptions);
    }

    if(!toAppendToLogFileName.isEmpty())
    {
        RETURN_IF_FALSE(_logFileThread->appendToBaseName(toAppendToLogFileName, logsArguments));
    }

    return true;
}

bool ASavingLogFile::manageAboveLogFileState(LogMsgType::Enum currentLogMsgType,
                                        const LoggingOptions &logsArguments)
{
    if(!_logFileStateEnabled || _isAboveLimitReached)
    {
        // The functionality is not activated, do nothing
        // Or the file has already been modified
        return true;
    }

    if(!LogMsgType::isEqualOrAboveCriticity(currentLogMsgType, _logFileStateLevel))
    {
        // Do nothing
        return true;
    }

    RETURN_IF_FALSE(manageLogFileStateWrittings(_logFileStateMsgToPrependIfAbove,
                                                _logFileStateToAddToFileNameIfAbove,
                                                logsArguments));

    _isAboveLimitReached = true;

    return true;
}

void ASavingLogFile::manageUnderLogFileState()
{
    if(!_logFileStateEnabled || _isAboveLimitReached)
    {
        // The functionality is not activated, do nothing
        return;
    }

    manageLogFileStateWrittings(_logFileStateMsgToPrependIfUnder,
                                _logFileStateToAddToFileNameIfUnder,
                                _defaultLoggingOptions);
}

bool ASavingLogFile::manageSequenceLogs()
{
    const ISequenceLogManager *logManager =
            DefaultBankGlobalManager::Instance()->accessCommonInterface()
                                                        .getSequenceLogManager(getSequenceToken());

    if(logManager == nullptr)
    {
        qWarning() << "The log manager hasn't been found for the sequence token: "
                   << getSequenceToken();
        return false;
    }

    connect(logManager, &ISequenceLogManager::newLogAdded,
            this,       &ASavingLogFile::writeLogIfNeeded);

    QVector<SequenceLogData> previousLogs;
    RETURN_IF_FALSE(logManager->getAllTheSequenceLogs(previousLogs,
                                                      _logsCats,
                                                      _msgTypeLevel));

    for(const SequenceLogData &log : previousLogs)
    {
        writeLogIfNeeded(log);
    }

    return true;
}

void ASavingLogFile::firstLogToSave(SequenceLogData &firstFileLog) const
{
    firstFileLog.setInvalid();
}

void ASavingLogFile::getLogArguments(const SequenceLogData &log,
                                     LoggingOptions &arguments)
{
    Q_UNUSED(log)
    arguments.clear();
}

bool ASavingLogFile::initInputsFormat(JsonArray &inputsFormat) const
{
    JsonObject logsFolderFormat;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(InputLogsFolderName,
                                                         logsFolderFormat,
                                                         InputLogsFolderDesc));

    inputsFormat.append(logsFolderFormat);

    return true;
}

bool ASavingLogFile::initParamsFormat(JsonArray &paramsFormat) const
{
    JsonObject maxFolderSize;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(MaxFolderSizeParamName,
                                                                 MaxFolderSizeParamDesc,
                                                                 0,
                                                                 0,
                                                                 maxFolderSize));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(maxFolderSize,
                                                                   Number(1, 0, false),
                                                                   Number::maxInt32(),
                                                                   Number(1)));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueNumber(Number(1, 0, false),
                                                                     maxFolderSize));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, maxFolderSize));

    paramsFormat.append(maxFolderSize);

    JsonObject logsCategoryFilter;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamCheckBoxListRequired(LogsCategoryFilterParamName,
                                                                       LogsCategoryFilterParamDesc,
                                                                       logsCategoryFilter));

    QVector<QString> defaultOptions;
    QVector<SequenceLogCategory::Enum> logCats = SequenceLogCategory::getAllEnums();
    for(auto citer = logCats.cbegin(); citer != logCats.cend(); ++citer)
    {
        QString logCatStr = SequenceLogCategory::toString(*citer);

        RETURN_IF_FALSE(BankJsonFormatter::formatParamAddListElem(logCatStr,
                                                                  logsCategoryFilter));
        defaultOptions.append(logCatStr);
    }

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultCheckedList(defaultOptions,
                                                                     logsCategoryFilter));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, logsCategoryFilter));

    paramsFormat.append(logsCategoryFilter);

    JsonObject msgTypeFilter;
    JsonObject logFileStateLevel;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamSelectRequired(MsgTypeLevelParamName,
                                                                 MsgTypeLevelParamDesc,
                                                                 msgTypeFilter));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamSelectRequired(LogFileStateLevelParamName,
                                                                 LogFileStateLevelParamDesc,
                                                                 logFileStateLevel));

    for(LogMsgType::Enum msgType : LogMsgType::getAvailableElements())
    {
        QString msgTypeStr = LogMsgType::toString(msgType);

        RETURN_IF_FALSE(BankJsonFormatter::formatParamAddSelectOption(msgTypeStr,
                                                                      msgTypeStr,
                                                                      msgTypeFilter));

        RETURN_IF_FALSE(BankJsonFormatter::formatParamAddSelectOption(msgTypeStr,
                                                                      msgTypeStr,
                                                                      logFileStateLevel));
    }

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultOption(
                                                { LogMsgType::toString(DefaultMsgTypeLevelValue) },
                                                msgTypeFilter));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultOption(
                                            { LogMsgType::toString(DefaultLogFileStateLevelValue) },
                                            logFileStateLevel));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, msgTypeFilter));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, logFileStateLevel));

    paramsFormat.append(msgTypeFilter);

    paramsFormat.append(logFileStateLevel);

    JsonObject logSuffix;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(LogsFileSuffixParamName,
                                                                 LogsFileSuffixParamDesc,
                                                                 logSuffix));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueString(LogsFileSuffixDefaultValue,
                                                                     logSuffix));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, logSuffix));

    paramsFormat.append(logSuffix);

    JsonObject fileLogCreationBehavior;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamSelectRequired(FileCreationBehaviorName,
                                                                 FileCreationBehaviorDesc,
                                                                 fileLogCreationBehavior));
    
    RETURN_IF_FALSE(BankJsonFormatter::formatParamAddSelectOption(FileCreationAtFirstLogKey,
                                                                  FileCreationAtFirstLogLabel,
                                                                  fileLogCreationBehavior));
    
    RETURN_IF_FALSE(BankJsonFormatter::formatParamAddSelectOption(FileCreationImmediatelyKey,
                                                                  FileCreationImmediatelyLabel,
                                                                  fileLogCreationBehavior));
    
    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultOption({ FileCreationImmediatelyKey },
                                                                fileLogCreationBehavior));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, fileLogCreationBehavior));

    paramsFormat.append(fileLogCreationBehavior);

    JsonObject logFormatType;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamSelectRequired(LogFormatTypeParamName,
                                                                 LogFormatTypeParamDesc,
                                                                 logFormatType));

    for(SequenceLogFormatType::Enum formatType : SequenceLogFormatType::getAllEnums())
    {
        QString formatTypestr = SequenceLogFormatType::toString(formatType);

        RETURN_IF_FALSE(BankJsonFormatter::formatParamAddSelectOption(formatTypestr,
                                                                      formatTypestr,
                                                                      logFormatType));
    }

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultOption(
                                    { SequenceLogFormatType::toString(LogFormatTypeDefaultValue) },
                                    logFormatType));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, logFormatType));

    paramsFormat.append(logFormatType);

    // logFileState parameters

    JsonObject logFileStateEnabled;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamBoolRequired(EnableLogFileStateParamName,
                                                               EnableLogFileStateParamDesc,
                                                               logFileStateEnabled));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueBool(DefaultEnableLogFileStateValue,
                                                                   logFileStateEnabled));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, logFileStateEnabled));

    paramsFormat.append(logFileStateEnabled);

    JsonObject logFileStateToPrependIfUnder;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(
        LogFileStateMsgToPrependIfUnderLevelName,
        LogFileStateMsgToPrependIfUnderLevelDesc,
        logFileStateToPrependIfUnder));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, logFileStateToPrependIfUnder));

    paramsFormat.append(logFileStateToPrependIfUnder);

    JsonObject logFileStateToPrependIfAbove;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(
        LogFileStateMsgToPrependIfAboveLevelName,
        LogFileStateMsgToPrependIfAboveLevelDesc,
        logFileStateToPrependIfAbove));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, logFileStateToPrependIfAbove));

    paramsFormat.append(logFileStateToPrependIfAbove);

    JsonObject logFileStateToFileNameIfUnder;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(
        LogFileStateToAddToFileNameIfUnderLevelName,
        LogFileStateToAddToFileNameIfUnderLevelDesc,
        logFileStateToFileNameIfUnder));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, logFileStateToFileNameIfUnder));

    paramsFormat.append(logFileStateToFileNameIfUnder);

    JsonObject logFileStateToFileNameIfAbove;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(
        LogFileStateToAddToFileNameIfAboveLevelName,
        LogFileStateToAddToFileNameIfAboveLevelDesc,
        logFileStateToFileNameIfAbove));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, logFileStateToFileNameIfAbove));

    paramsFormat.append(logFileStateToFileNameIfAbove);

    return true;
}

bool ASavingLogFile::validateAndSetParameters(const JsonArray &parameters, JsonArray &errors)
{
    RETURN_IF_FALSE(DefaultSequenceInstanceModule::validateAndSetParameters(parameters, errors));

    QString error;
    Number maxSizeNumber;
    if(!BankJsonParser::getNumberValue(parameters, MaxFolderSizeParamName, maxSizeNumber, &error))
    {
        fillErrorArray(MaxFolderSizeParamName, error, errors);
        return false;
    }

    // The conversion from number to integer limits is already tested before
    _maxFolderSizeInMo = maxSizeNumber.toInt32();

    QVector<QString> categories;
    if(!BankJsonParser::getArrayStringValue(parameters,
                                            LogsCategoryFilterParamName,
                                            categories,
                                            &error))
    {
        fillErrorArray(LogsCategoryFilterParamName, error, errors);
        return false;
    }

    _logsCats = SequenceLogCategory::parseFromVector(categories);

    if(!categories.isEmpty() && (_logsCats == SequenceLogCategory::Enums()))
    {
        fillErrorArray(LogsCategoryFilterParamName,
                       "One of the categories selected is unknown",
                       errors);
        return false;
    }

    QVector<QString> msgTypeStr;
    if(!BankJsonParser::getArrayStringValue(parameters, MsgTypeLevelParamName, msgTypeStr, &error))
    {
        fillErrorArray(MsgTypeLevelParamName, error, errors);
        return false;
    }

    if(msgTypeStr.isEmpty())
    {
        fillErrorArray(MsgTypeLevelParamName, "No MsgType selected", errors);
        return false;
    }

    bool ok = false;
    LogMsgType::Enum tmpLogMsg = LogMsgType::parseCriticityFromString(msgTypeStr.first());

    if(tmpLogMsg == LogMsgType::Unknown)
    {
        fillErrorArray(MsgTypeLevelParamName,
                       "The log criticity is unknown: " + msgTypeStr.first(),
                       errors);
        return false;
    }

    _msgTypeLevel = tmpLogMsg;

    if(!BankJsonParser::getStringValue(parameters,
                                        LogsFileSuffixParamName,
                                       _logsFileSuffix,
                                       &error))
    {
        fillErrorArray(LogsFileSuffixParamName, error, errors);
        return false;
    }

    QVector<QString> formatTypesStr;
    if(!BankJsonParser::getArrayStringValue(parameters,
                                            LogFormatTypeParamName,
                                            formatTypesStr,
                                            &error))
    {
        fillErrorArray(LogFormatTypeParamName, error, errors);
        return false;
    }

    if(formatTypesStr.isEmpty())
    {
        qWarning() << "No format type selected";
        fillErrorArray(LogFormatTypeParamName, "No format type selected", errors);
        return false;
    }

    _logsFormatType = SequenceLogFormatType::parseFromString(formatTypesStr.first());

    if(_logsFormatType == SequenceLogFormatType::Unknown)
    {
        error = "A wrong log format has been given " + formatTypesStr.first();
        fillErrorArray(LogFormatTypeParamName, error, errors);
        return false;
    }

    QVector<QString> logFileCreationList;
    if(!BankJsonParser::getArrayStringValue(parameters,
                                            FileCreationBehaviorName,
                                            logFileCreationList,
                                            &error))
    {
        fillErrorArray(FileCreationBehaviorName, error, errors);
        return false;
    }

    if(logFileCreationList.isEmpty())
    {
        fillErrorArray(FileCreationBehaviorName, "No behavior given for file creation", errors);
        return false;
    }

    QString logFileCreation = logFileCreationList.first();
    
    if(logFileCreation == FileCreationAtFirstLogKey)
    {
        _fileLogCreationBehavior = CreateAtFirstLogKey;
    }
    else if(logFileCreation == FileCreationImmediatelyKey)
    {
        _fileLogCreationBehavior = CreateImmediatelyKey;
    }
    else
    {
        fillErrorArray(FileCreationBehaviorName,
                       "The file creation behavior given is not correct: " + logFileCreation,
                       errors);
        return false;
    }

    // Manage logFileState parameters
    if(!BankJsonParser::getBoolValue(parameters,
                                     EnableLogFileStateParamName,
                                     _logFileStateEnabled,
                                     &error,
                                     true))
    {
        fillErrorArray(EnableLogFileStateParamName, error, errors);
        return false;
    }

    if(_logFileStateEnabled)
    {
        // Only this parameter is compulsory if the logFileState functionality is enabled
        QVector<QString> logFileState;
        if(!BankJsonParser::getArrayStringValue(parameters,
                                                LogFileStateLevelParamName,
                                                logFileState,
                                                &error))
        {
            fillErrorArray(LogFileStateLevelParamName, error, errors);
            return false;
        }

        if(logFileState.isEmpty())
        {
            fillErrorArray(LogFileStateLevelParamName, "No LogFileState level selected", errors);
            return false;
        }

        ok = false;
        tmpLogMsg = LogMsgType::parseCriticityFromString(logFileState.first());

        if(tmpLogMsg == LogMsgType::Unknown)
        {
            fillErrorArray(LogFileStateLevelParamName,
                           "The log criticity is unknown: " + logFileState.first(),
                           errors);
            return false;
        }

        _logFileStateLevel = tmpLogMsg;
    }

    if(!BankJsonParser::getStringValue(parameters,
                                       LogFileStateMsgToPrependIfUnderLevelName,
                                       _logFileStateMsgToPrependIfUnder,
                                       &error,
                                       true))
    {
        fillErrorArray(LogFileStateMsgToPrependIfUnderLevelName, error, errors);
        return false;
    }

    if(!BankJsonParser::getStringValue(parameters,
                                       LogFileStateToAddToFileNameIfUnderLevelName,
                                       _logFileStateToAddToFileNameIfUnder,
                                       &error,
                                       true))
    {
        fillErrorArray(LogFileStateToAddToFileNameIfUnderLevelName, error, errors);
        return false;
    }

    if(!BankJsonParser::getStringValue(parameters,
                                        LogFileStateMsgToPrependIfAboveLevelName,
                                       _logFileStateMsgToPrependIfAbove,
                                       &error,
                                       true))
    {
        fillErrorArray(LogFileStateMsgToPrependIfAboveLevelName, error, errors);
        return false;
    }

    if(!BankJsonParser::getStringValue(parameters,
                                       LogFileStateToAddToFileNameIfAboveLevelName,
                                       _logFileStateToAddToFileNameIfAbove,
                                       &error,
                                       true))
    {
        fillErrorArray(LogFileStateToAddToFileNameIfAboveLevelName, error, errors);
        return false;
    }

    return true;
}

void ASavingLogFile::writeLogIfNeeded(const SequenceLogData &log)
{
    if(!_logsCats.testFlag(log.getCategory())                                   ||
       !LogMsgType::isEqualOrAboveCriticity(log.getType(), _msgTypeLevel))
    {
        return;
    }

    writeLog(log);
}

void ASavingLogFile::processModule(const JsonArray &jsonObjectInputValues)
{
    if(!PauseAndStopHelper::managePauseAndStop(getSequenceState()))
    {
        emit moduleFinished(false, {});
        return;
    }

    if(_loggingStarted)
    {
        qInfo() << "Logging already started, do nothing";
        emit moduleFinished(true, {});
        return;
    }

    QString logsFolder;
    if(!BankJsonParser::getStringValue(jsonObjectInputValues, InputLogsFolderName, logsFolder))
    {
        qWarning() << "Can't get the input logs folder";
        emit moduleFinished(false, {});
        return;
    }

    if(!startLoggingProcess(logsFolder, jsonObjectInputValues))
    {
        emit moduleFinished(false, {});
        return;
    }

    _isAboveLimitReached = false;

    // Get the default logging options to be used in class destructor
    getLogArguments(SequenceLogData(), _defaultLoggingOptions);

    if(_fileLogCreationBehavior == CreateImmediatelyKey)
    {
        SequenceLogData log;

        _isFirstLogFileWritten = true;

        firstLogToSave(log);

        if(log.isValid())
        {
            emit newLog(log);

            //If log can't be written, cause of its category or msg type level, we force it's
            // writting

            if(!_logsCats.testFlag(log.getCategory()) ||
               !LogMsgType::isEqualOrAboveCriticity(log.getType(), _msgTypeLevel))
            {
                writeLog(log);
            }
        }
    }

    if(!manageSequenceLogs())
    {
        emit moduleFinished(false, {});
        return;
    }

    _loggingStarted = true;

    emit moduleFinished(true, {});
}
