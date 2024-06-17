// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "abstractpluginelements/defaultsequenceinstancemodule.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief This module emits the line of a CSV in the sequence logs with the given log category.
    @note It takes a list of string elements (each one is equal to a column value), join them with
          the delimiter given to emit it.
    @note To fill the line to emit, the other modules "CsvAdd*" have to be used. */
class CsvLogLine : public DefaultSequenceInstanceModule
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(CsvLogLine)

    public:
        /** @brief Class constructor
            @param parent The parent instance class */
        Q_INVOKABLE explicit CsvLogLine(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~CsvLogLine() override;

    protected slots:
        /** @brief DefaultSequenceInstanceModule::processModule */
        virtual void processModule(const JsonArray &jsonObjectInputValues) override;

    protected:
    /** @brief Validate and set the parameters
        @param parameters The parameters to parse
        @param errors Contains the errors got when tryied to parse the parameters
        @return True if no problem occurs */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

        /** @see DefaultSequenceInstanceModule::initInputsFormat
            @note @ref inputCsvLineName : array<string> (The list represents the column of the CSV
                                                         line; therefore this is the line to append
                                                         with data) */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

        /** @see DefaultSequenceInstanceModule::initParamsFormat
            @note @ref logCategoryParamName : array<string> (Choose the category to log the CSV
                                                             line with)
            @note @ref msgTypeLevelParamName : array<string> (Select a msg type level, for the CSV
                                                              line.)
            @note @ref delimiterParamName : string (This is the delimiter used in the CSV line.) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

    private:
        /** @brief This method allows to create the CSV line and log it the log category wanted
            @param jsonObjectInputValues The input values got
            @return True if no problem occurs */
        bool operate(const JsonArray &jsonObjectInputValues);

    private:
        static const constexpr char *InputCsvLineName = "csvLine";
        static const constexpr char *InputCsvLineDesc = "CSV line to append with data";

        static const constexpr char *LogCategoryParamName = "logCategory";
        static const constexpr char *LogCategoryParamDesc = "Choose the category to log the CSV "
                                                            "line with";

        static const constexpr char *MsgTypeLevelParamName = "msgTypeLevel";
        static const constexpr char *MsgTypeLevelParamDesc = "Select a msg type level, for the CSV "
                                                             "line.";
        static const constexpr LogMsgType::Enum MsgTypeLevelDefaultValue = LogMsgType::Info;

        static const constexpr char *DelimiterParamName = "delimiter";
        static const constexpr char *DelimiterParamDesc = "This is the delimiter used in the CSV "
                                                          "line.";
        static const constexpr char *DelimiterDefaultValue = ",";

    private:
        LogMsgType::Enum _msgTypeLevel{ MsgTypeLevelDefaultValue };
        SequenceLogCategory::Enum _logCategory{SequenceLogCategory::ApplicationScope};
        QString _delimiter { DelimiterDefaultValue };
};
