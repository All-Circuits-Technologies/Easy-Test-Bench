// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "abstractpluginelements/defaultsequenceinstancemodule.hpp"

#include "logsutility/loggingoption.hpp"
#include "numberutility/number.hpp"
#include "testbedcore/logs/sequencelogformattype.hpp"

class SaveLogInFilesThread;


/** @brief Abstract saving log in file plugin
    @note The module will initiate the logs saving in file, and it's stopped at the sequence end */
class ASavingLogFile : public DefaultSequenceInstanceModule
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param moduleClassName The class name of the module
            @param parent The parent class */
        explicit ASavingLogFile(const QString &moduleClassName, QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~ASavingLogFile() override;

    public:
        /** @see DefaultSequenceInstanceModule::stopModule */
        virtual bool stopModule(StopAskedType::Enum stopAskedType) override;

    protected slots:
        /** @brief DefaultSequenceInstanceModule::processModule */
        virtual void processModule(const JsonArray &jsonObjectInputValues) override;

        /** @brief Try to write the log if it's passing the log filter (log category and level
                   filtering)
            @param log The log to write */
        void writeLogIfNeeded(const SequenceLogData &log);

    protected:
        /** @brief Called before the first log to write, in order to add a specific text before all
            @note This method is called according to the file log creation behavior decided.
            @note By default, no first log is appended to file
            @param firstFileLog The first log to add in file, if invalid, nothing is added */
        virtual void firstLogToSave(SequenceLogData &firstFileLog) const;

        /** @brief Called for every log and before writing, in order to get the arguments to add
                   to the log, if necessary
            @note The arguments depends of the logging strategy chosen
            @param log The log to add arguments with
            @param arguments The arguments to add with the log when writing */
        virtual void getLogArguments(const SequenceLogData &log,
                                     LoggingOptions &arguments);

        /** @brief Start the logging process
            @param logsFolderPath The path of the main logs folder
            @param jsonObjectInputValues Module input values to parse in order to start logging
            @return True if no problem occurs */
        virtual bool startLoggingProcess(const QString &logsFolderPath,
                                         const JsonArray &jsonObjectInputValues) = 0;

        /** @see DefaultSequenceInstanceModule::initInputsFormat */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

        /** @see DefaultSequenceInstanceModule::initParamsFormat
            @note @ref maxFolderSizeParamName : number (the max logs size, if overflow the limit,
                                                        the oldest files will be removed)
                  @ref logsCategoryFilterParamName : vector<string> (the category of logs to save in
                                                                     file)
                  @ref msgTypeLevelParamName : vector<string> (all the logs greater or equal to this
                                                               limit, will be added in file)
                  @ref logsFileSuffixParamName : string (the log file suffix)
                  @ref fileCreationBehaviorName : vector<string> (tells when the log file must be
                                                                  created)
                  @ref enableLogFileStateParamName : bool (If true, it's possible to prepend a log
                                                           to the log file and/or append a suffix
                                                           to the log file name if at least one log
                                                           is above the level given. The same things
                                                           are possible if at log file closing, the
                                                           level was never reached.)
                  @ref logFileStateLevelParamName : vector<string> (Set the log level for the
                                                                    logFileState functionality)
                  @ref logFileStateMsgToPrependIfUnderLevelName : string (If not empty, if the
                                                             functionality is enabled and no log msg
                                                             have never exceeded the level defined,
                                                             this message will be added at the start
                                                             of the log file at it's closing.)
                  @ref logFileStateToAddToFileNameIfUnderLevelName : string (If not empty, if the
                                                             functionality is enabled and no log msg
                                                             have never exceeded the level defined,
                                                             this string will be appended to the log
                                                             file base name at it's closing.)
                  @ref logFileStateMsgToPrependIfAboveLevelNae : string (If not empty, if the
                                                            functionality is enabled and at least
                                                            one log msg has exceeded the level
                                                            defined, this message will be added once
                                                            at the start of the log file, when the
                                                            log has been received.)
                  @ref logFileStateToAddToFileNameIfAboveLevelName : string (If not empty, if the
                                                             functionality is enabled and at least
                                                             one log msg has exceeded the level
                                                             defined, this string will be appended
                                                             once to the log file base name  at the
                                                             start of the log file, when the log has
                                                             been received.) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @see DefaultSequenceInstanceModule::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

    protected:
        /** @brief Describes the file log creation behavior */
        enum FileLogCreationBehavior
        {
            CreateAtFirstLogKey, //!< @brief The file log is created when at the first log receiving
            CreateImmediatelyKey //!< @brief The file log is created immediately on module launching
        };

    protected:
        /** @brief Get max folder size in Mo
            @return The max folder size */
        const int &getMaxFolderSizeInMo() const { return _maxFolderSizeInMo; }

        /** @brief Get the categories filter for logging */
        SequenceLogCategory::Enums getLogsCategory() const { return _logsCats; }

        /** @brief Get the msg type level filter for logging */
        LogMsgType::Enum getMsgTypeLevel() const { return _msgTypeLevel; }

        /** @brief Get log file suffix */
        const QString &getLogFileSuffix() const { return _logsFileSuffix; }

        /** @brief Access logs file thread linked to module */
        SaveLogInFilesThread *accessLogsFileThread() { return _logFileThread; }

    private:
        /** @brief Write the log given to the file
            @warning In most case, it's better to call: @ref ASavingLogFile::writeLogIfNeeded,
                     because this method doesn't filter the log
            @param log The log to write */
        void writeLog(const SequenceLogData &log);

        /** @brief Manage the LogFileState writting
            @param msgToPrepend If not empty, the message will be added to the start of the log file
            @param toAppendToLogFileName If not empty, the string will be appended to the log file
                                         base name
            @param logsArguments The logging options to transfer to the log file strategy
            @return True if no problem occurs */
        bool manageLogFileStateWrittings(const QString &msgToPrepend,
                                         const QString &toAppendToLogFileName,
                                         const LoggingOptions &logsArguments);

        /** @brief Test if the logFileState above conditions are fullfiled and fire the writting
            @param currentLogMsgType The current log msg type
            @param logsArguments The logging options to transfer to the log file strategy
            @return True if no problem occurs */
        bool manageAboveLogFileState(LogMsgType::Enum currentLogMsgType,
                                     const LoggingOptions &logsArguments);

        /** @brief Test if the logFileState under conditions are fullfiled and fire the writting */
        void manageUnderLogFileState();

        /** @brief Manage the connection to get the logs from sequence and get all the sequence
                   logs already added
            @return True if no problem occurs */
        bool manageSequenceLogs();

    private:
        static const constexpr char *InputLogsFolderName = "inputLogsFolder";
        static const constexpr char *InputLogsFolderDesc = "This is the folder where the logs file "
                                                           "will be stored.";

        static const constexpr char *MaxFolderSizeParamName = "maxFolderSize";
        static const constexpr char *MaxFolderSizeParamDesc = "The max folder size in Mo, after "
                                                              "this size, the oldest logs files "
                                                              "will be removed";
        static const constexpr char *LogsCategoryFilterParamName = "logsCategoryFilter";
        static const constexpr char *LogsCategoryFilterParamDesc = "Choose the category of logs to"
                                                                   " save in file";
        static const constexpr char *MsgTypeLevelParamName = "msgTypeLevel";
        static const constexpr char *MsgTypeLevelParamDesc = "Select a msg type level, all the "
                                                             "elements above and equal to this "
                                                             "level will be saved in file";
        static const constexpr LogMsgType::Enum DefaultMsgTypeLevelValue = LogMsgType::Info;

        /******************************************************************************************/
        // Params linked to the logFileState functionality

        static const constexpr char *EnableLogFileStateParamName = "enableLogFileState";
        static const constexpr char *EnableLogFileStateParamDesc = "If true, it's possible to "
                                                                   "prepend a log to the log file "
                                                                   "and/or append a suffix to the "
                                                                   "log file name if at least one "
                                                                   "log is above the level given. "
                                                                   "The same things are possible "
                                                                   "if at log file closing, the "
                                                                   "level was never reached.";
        static const constexpr bool DefaultEnableLogFileStateValue = false;

        static const constexpr char *LogFileStateLevelParamName = "logFileStateLevel";
        static const constexpr char *LogFileStateLevelParamDesc = "Set the log level for the "
                                                                  "logFileState feature";
        static const constexpr LogMsgType::Enum DefaultLogFileStateLevelValue = LogMsgType::Fatal;

        static const constexpr char *LogFileStateMsgToPrependIfUnderLevelName =
                                                            "logFileStateMsgToPrependIfUnderLevel";
        static const constexpr char *LogFileStateMsgToPrependIfUnderLevelDesc =
                                                            "If not empty, if the feature is "
                                                            "enabled and no log msg have never "
                                                            "exceeded the level defined, this "
                                                            "message will be added at the start of "
                                                            "the log file at it's closing.";

        static const constexpr char *LogFileStateToAddToFileNameIfUnderLevelName =
                                                        "logFileStateToAddToFileNameIfUnderLevel";
        static const constexpr char *LogFileStateToAddToFileNameIfUnderLevelDesc =
                                                            "If not empty, if the feature is "
                                                            "enabled and no log msg have never "
                                                            "exceeded the level defined, this "
                                                            "string will be appended to the log "
                                                            "file base name at it's closing.";

        static const constexpr char *LogFileStateMsgToPrependIfAboveLevelName =
                                                            "logFileStateMsgToPrependIfAboveLevel";
        static const constexpr char *LogFileStateMsgToPrependIfAboveLevelDesc =
                                                            "If not empty, if the feature is "
                                                            "enabled and at least one log msg "
                                                            "has exceeded the level defined, this "
                                                            "message will be added once at the "
                                                            "start of the log file, when the log "
                                                            "has been received.";

        static const constexpr char *LogFileStateToAddToFileNameIfAboveLevelName =
                                                        "logFileStateToAddToFileNameIfAboveLevel";
        static const constexpr char *LogFileStateToAddToFileNameIfAboveLevelDesc =
                                                            "If not empty, if the feature is "
                                                            "enabled and at least one log msg "
                                                            "has exceeded the level defined, this "
                                                            "string will be appended once to the "
                                                            "log file base name  at the start of "
                                                            "the log file, when the log has been "
                                                            "received.";

        // End of logFileState params
        /******************************************************************************************/

        static const constexpr char *LogFormatTypeParamName = "logFormatType";
        static const constexpr char *LogFormatTypeParamDesc = "The logs format type to use in the "
                                                              "file";
        static const constexpr SequenceLogFormatType::Enum LogFormatTypeDefaultValue =
                                                                SequenceLogFormatType::DisplayAll;

        static const constexpr char *LogsFileSuffixParamName = "logsFileSuffix";
        static const constexpr char *LogsFileSuffixParamDesc = "Give the logs file suffix";
        static const constexpr char *LogsFileSuffixDefaultValue = ".log";

        static const constexpr char *FileCreationBehaviorName = "fileCreationBehavior";
        static const constexpr char *FileCreationBehaviorDesc = "Select a behavior to manage logs "
                                                                "file creation";
        static const constexpr char *FileCreationAtFirstLogKey = "createAtFirstLogKey";
        static const constexpr char *FileCreationAtFirstLogLabel = "Create the log file when "
                                                                   "writing the first log";
        static const constexpr char *FileCreationImmediatelyKey = "createImmediatelyKey";
        static const constexpr char *FileCreationImmediatelyLabel = "Create the log file "
                                                                    "immediately";

    private:
        int _maxFolderSizeInMo{ -1 };
        SequenceLogCategory::Enums _logsCats;
        LogMsgType::Enum _msgTypeLevel{ DefaultMsgTypeLevelValue };
        QString _logsFileSuffix{ LogsFileSuffixDefaultValue };
        FileLogCreationBehavior _fileLogCreationBehavior { CreateImmediatelyKey };
        SequenceLogFormatType::Enum _logsFormatType { LogFormatTypeDefaultValue };

        // logFileStateParams
        bool _logFileStateEnabled{ DefaultEnableLogFileStateValue };
        LogMsgType::Enum _logFileStateLevel{ DefaultLogFileStateLevelValue };
        QString _logFileStateToAddToFileNameIfAbove;
        QString _logFileStateToAddToFileNameIfUnder;
        QString _logFileStateMsgToPrependIfAbove;
        QString _logFileStateMsgToPrependIfUnder;
        bool _isAboveLimitReached{false};

        bool _isFirstLogFileWritten{false};

        LoggingOptions _defaultLoggingOptions;
        SaveLogInFilesThread *_logFileThread{nullptr};
        bool _loggingStarted{false};
};
