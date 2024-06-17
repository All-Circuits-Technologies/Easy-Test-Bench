// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "loggers/asavinglogfile.hpp"

#include <QHash>

#include "logsutility/loggingstrategyoption.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief This module creates a file and save all the logs linked to a specific board in it.
    @note The file is closed and no more used at the sequence end. */
class OneBoardOneFile : public ASavingLogFile
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(OneBoardOneFile)

    public:
        /** @brief Class constructor
            @param parent The class parent */
        Q_INVOKABLE explicit OneBoardOneFile(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~OneBoardOneFile() override;

    protected:
        /** @see ASavingLogFile::initInputsFormat
            @note @ref inputSerialNumberName : string (This is the serial number to use for creating
                                                       the file) */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

        /** @see ASavingLogFile::firstLogToSave
            @note The plugin add a first log with the board serial number */
        virtual void firstLogToSave(SequenceLogData &firstFileLog) const override;

        /** @see ASavingLogFile::getLogArguments
            @note This add the serial number in the arguments list */
        virtual void getLogArguments(const SequenceLogData &log,
                                     LoggingOptions &arguments) override;

        /** @brief ASavingLogFile::startLoggingProcess */
        virtual bool startLoggingProcess(const QString &logsFolderPath,
                                         const JsonArray &jsonObjectInputValues) override;

    private:
        static const constexpr char *InputSerialNumberName = "inputSerialNumber";
        static const constexpr char *InputSerialNumberDesc = "This is the serial number to use for "
                                                             "creating the file";

        inline static const LoggingStrategyOption::Enums DefaultStrategies = {
            LoggingStrategyOption::File_StoreInDayFolder    |
            LoggingStrategyOption::File_StoreInMonthFolder  |
            LoggingStrategyOption::File_StoreInYearFolder
        };

        static const QHash<char, QString> DefaultReplacementStrings;

    private:
        QString _serialNumber;
};
