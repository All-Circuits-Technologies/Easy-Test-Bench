// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "abstractpluginelements/defaultsequenceinstancemodule.hpp"

#include <QSharedPointer>

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"

class AutoAnsSerialHandler;
class AutoAnswerParams;


/** @brief The start serial link automatic answer module
    @note This module allows to start an automatic echoing in background. To stop it, you have to
          call the @ref StopSerialAutoAnswer module */
class StartSerialAutoAnswer : public DefaultSequenceInstanceModule
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(StartSerialAutoAnswer)

    public:
        /** @brief Class constructor
            @param parent The parent instance class */
        Q_INVOKABLE explicit StartSerialAutoAnswer(QObject *parent = nullptr);

        /** @brief The class destructor */
        virtual ~StartSerialAutoAnswer() override;

    public:
        /** @copydoc DefaultSequenceInstanceModule::manageModulePause */
        virtual bool manageModulePause(bool pauseModule) override;

    protected:
        /** @copydoc DefaultSequenceInstanceModule::initInputsFormat
            @note @ref SerialPluginLibConstants::Json::PortName : string (Serial port name
                                                                          (ex: COM1)) */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

        /** @copydoc DefaultSequenceInstanceModule::initParamsFormat
            @note @ref DisplayStatsParamName : boolean (If the option is active, at the end the
                                                        module will display in logs the number of
                                                        received and sent commands (only the
                                                        expected one).)
                  @ref SerialPluginLibConstants::Json::EndOfLine : string (End of line string
                                                                           (defaults to LF))
                  @ref SerialPluginLibConstants::Json::Trim : boolean (Should received lines be
                                                                       trimmed before verifications)
                  @ref SerialPluginLibConstants::Json::IsMultiLineTest : boolean (If true, the
                                                                                  expected regexp
                                                                                  pattern will be
                                                                                  tested with all
                                                                                  the received data.
                                                                                  If false, the
                                                                                  regexp pattern
                                                                                  will only be
                                                                                  tested with each
                                                                                  line.)
                  @ref SerialPluginLibConstants::Json::ForceFlush : boolean (If true, we force
                                                                             flushing after having
                                                                             written data to serial
                                                                             port.)
                  @ref AutoAnswerParams::MsgsToSendParamName : list<string> (
                                                            The messages to send if the
                                                            expected message has been received.
                                                            The messages are stored in a string
                                                            with the following format:
                                                            \"MSG_IDX/MSG_CONTENT\",
                                                            It's possible to add a time to wait
                                                            before to send a message, in that
                                                            case, the line has the following
                                                            format:
                                                            \"MSG_IDX/MSG_CONTENT/
                                                            TIME_TO_WAIT_IN_MS\",
                                                            The MSG_IDX indicates the expected
                                                            message linked to this message to
                                                            send.
                                                            The MSG_IDX can't overflow the
                                                            expected messages list length.
                                                            The TIME_TO_WAIT_IN_MS is the time to
                                                            wait in milliseconds before sending
                                                            the message, if the value isn't
                                                            present or negative, we send the
                                                            message without waiting.)
                  @ref AutoAnswerParams::ExpectedMessagesParamName : list<string> (This contains
                                                                                   the patterns of
                                                                                   regular
                                                                                   expression to
                                                                                   test. No need of
                                                                                   captured
                                                                                   groups.) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @copydoc DefaultSequenceInstanceModule::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

    protected slots:
        /** @brief Execute the task
            @copydoc DefaultSequenceInstanceModule::processModule */
        virtual void processModule(const JsonArray &jsonObjectInputValues) override;

    private:
        /** @brief Called to start the auto answer process
            @param jsonObjectInputValues The module input values
            @return True if no problem occurred */
        bool startAutoAnswer(const JsonArray &jsonObjectInputValues);

    private:
        static const constexpr char *DisplayStatsParamName = "displayStats";
        static const constexpr char *DisplayStatsParamDesc = "If the option is active, at the end "
                                                             "the module will display in logs the "
                                                             "number of received and sent "
                                                             "commands (only the expected one).";
        static const constexpr bool DisplayStatsDefaultValue = false;

        static const constexpr bool DefaultFlushSendValue = true;

    private:
        const AutoAnswerParams *_autoAnsParam{nullptr};
        QSharedPointer<AutoAnsSerialHandler> _autoAnsHandler;
};
