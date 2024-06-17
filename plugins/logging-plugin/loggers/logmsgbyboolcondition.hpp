// SPDX-FileCopyrightText: 2020 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "abstractpluginelements/defaultsequenceinstancemodule.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief This module creates and emits a specific log message on a boolean condition.
    @note You can specify a different message if the boolean condition is equals to true or
          false. */
class LogMsgByBoolCondition : public DefaultSequenceInstanceModule
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(LogMsgByBoolCondition)

    public:
        /** @brief Class constructor
            @param parent The parent class */
        Q_INVOKABLE explicit LogMsgByBoolCondition(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~LogMsgByBoolCondition() override;

    public:
        /** @see DefaultSequenceInstanceModule::initInputsFormat
            @note @ref inputConditionName : boolean (Boolean condition to display logs) */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

        /** @see DefaultSequenceInstanceModule::initParamsFormat
            @note @ref ToDisplayIfTrueParamName : string (The message to display if the condition
                                                          is equals to true. If the message is
                                                          empty, nothing is logged.)
            @note @ref ToDisplayLogLevelIfTrueParamName : vector<string> (Select a log level for
                                                                          the log to display if
                                                                          true)
            @note @ref ToDisplayCatIfTrueParamName : vector<string> (Select the category of the
                                                                     log to display if true)
            @note @ref ToDisplayIfFalseParamName : string (The message to display if the condition
                                                           is equals to false. If the message is
                                                           empty, nothing is logged.)
            @note @ref ToDisplayLogLevelIfFalseParamName : vector<string> (Select a log level for
                                                                           the log to display if
                                                                           false)
            @note @ref ToDisplayCatIfFalseParamName : vector<string> (Select the category of the
                                                                      log to display if false) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @see DefaultSequenceInstanceModule::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

    protected slots:
        /** @see DefaultSequenceInstanceModule::processModule */
        virtual void processModule(const JsonArray &jsonObjectInputValues) override;

    private:
        static const constexpr char *InputConditionName = "inputCondition";
        static const constexpr char *InputConditionDesc = "Boolean condition to display logs";

        static const constexpr char *ToDisplayIfTrueParamName = "toDisplayIfTrue";
        static const constexpr char *ToDisplayIfTrueParamDesc = "The message to display if the "
                                                                "condition is equals to true. If "
                                                                "the message is empty, nothing is "
                                                                "logged.";

        static const constexpr char *ToDisplayLogLevelIfTrueParamName = "toDisplayLogLevelIfTrue";
        static const constexpr char *ToDisplayLogLevelIfTrueParamDesc = "Select a log level for "
                                                                        "the log to display if "
                                                                        "true";
        static const constexpr LogMsgType::Enum DefaultToDisplayLogLevelIfTrue = LogMsgType::Info;

        static const constexpr char *ToDisplayCatIfTrueParamName = "toDisplayCatIfTrue";
        static const constexpr char *ToDisplayCatIfTrueParamDesc = "Select the category of the log "
                                                                   "to display if true";
        static const constexpr SequenceLogCategory::Enum DefaultToDisplayCatIfTrue =
            SequenceLogCategory::BusinessScope;

        static const constexpr char *ToDisplayIfFalseParamName = "toDisplayIfFalse";
        static const constexpr char *ToDisplayIfFalseParamDesc = "The message to display if the "
                                                                 "condition is equals to false. If "
                                                                 "the message is empty, nothing is "
                                                                 "logged.";

        static const constexpr char *ToDisplayLogLevelIfFalseParamName = "toDisplayLogLevelIfFalse";
        static const constexpr char *ToDisplayLogLevelIfFalseParamDesc = "Select a log level for "
                                                                         "the log to display if "
                                                                         "false";
        static const constexpr LogMsgType::Enum DefaultToDisplayLogLevelIfFalse = LogMsgType::Info;

        static const constexpr char *ToDisplayCatIfFalseParamName = "toDisplayCatIfFalse";
        static const constexpr char *ToDisplayCatIfFalseParamDesc = "Select the category of the "
                                                                    "log to display if false";
        static const constexpr SequenceLogCategory::Enum DefaultToDisplayCatIfFalse =
            SequenceLogCategory::BusinessScope;

    private:
        LogMsgType::Enum _toDisplayIfTrueLogLevel{ DefaultToDisplayLogLevelIfTrue };
        SequenceLogCategory::Enum _toDisplayCatIfTrue { DefaultToDisplayCatIfTrue };
        QString _toDisplayIfTrue{""};

        LogMsgType::Enum _toDisplayIfFalseLogLevel{ DefaultToDisplayLogLevelIfFalse };
        SequenceLogCategory::Enum _toDisplayCatIfFalse { DefaultToDisplayCatIfFalse };
        QString _toDisplayIfFalse{""};
};
