// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "abstractpluginelements/defaultsequenceinstancemodule.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"

class AutoAnsThreadHandler;
class CanMsgSeqParams;


/** @brief This is the start auto answer module. This module allows to start the automatic
           answering process in background.
    @note The process can be stopped by calling the @see StopAutoAnswer module */
class StartAutoAnswer : public DefaultSequenceInstanceModule
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(StartAutoAnswer)

    public:
        /** @brief Class constructor
            @param parent The parent instance class */
        Q_INVOKABLE explicit StartAutoAnswer(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~StartAutoAnswer() override;

    public:
        /** @copydoc DefaultSequenceInstanceModule::manageModulePause */
        virtual bool manageModulePause(bool pauseModule) override;

    protected:
        /** @copydoc DefaultSequenceInstanceModule::initInputsFormat
            @note @ref PeakCanLibConstants::ModulesEntries::Inputs::InputPeakCanIntfName : string
                                            (The input contains the name of the peak CAN
                                             interface) */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

        /** @copydoc DefaultSequenceInstanceModule::initParamsFormat
            @note @ref DisplayStatsParamName : boolean (If the option is active, at the end the
                                                        module will display in logs the number of
                                                        received and sent commands (only the
                                                        expected one).)
            @note @ref ExpectedMsgsVerifsParamName : array<string> (This contains the elements to
                                                                    verify. The info are stored in
                                                                    a string with the following
                                                                    format:
                                                                    \"MSG_ID/MASK_IDX/MASK/
                                                                    MASK_RESULT\",
                                                                    if the payload of the message
                                                                    matches the mask and its
                                                                    result, we send an answer.)
            @note @ref AnswersToSendParamName : array<string> (The CAN messages to send if the
                                                               expected command has been received.
                                                               The messages are stored in a string
                                                               with the following format:
                                                               \"MSG_IDX/CAN_ID/PAYLOAD_LENGTH/
                                                               PAYLOAD\"
                                                               The MSG_IDX indicates the expected
                                                               message linked to this command to
                                                               send.
                                                               The MSG_IDX can't overflow the
                                                               expected messages list length.) */
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

        static const constexpr char *ExpectedMsgsVerifsParamName = "expectedMsgsVerifs";
        static const constexpr char *ExpectedMsgsVerifsParamDesc = "This contains the elements to "
                                                                   "verify. The info are stored "
                                                                   "in a string with the following "
                                                                   "format: "
                                                                   "\"MSG_ID/MASK_IDX/MASK/"
                                                                   "MASK_RESULT\", "
                                                                   "if the payload of the message "
                                                                   "matches the mask and its "
                                                                   "result, we send an answer.";

        static const constexpr char *AnswersToSendParamName = "answersToSend";
        static const constexpr char *AnswersToSendParamDesc = "The CAN messages to send if the "
                                                              "expected command has been received. "
                                                              "The messages are stored in a string "
                                                              "with the following format: "
                                                              "\"MSG_IDX/CAN_ID/PAYLOAD_LENGTH/"
                                                              "PAYLOAD\""
                                                              "The MSG_IDX indicates the expected "
                                                              "message linked to this command to "
                                                              "send. "
                                                              "The MSG_IDX can't overflow the "
                                                              "expected messages list length.";

    private:
        bool _displayStats{DisplayStatsDefaultValue};
        CanMsgSeqParams *_msgSeqParams{nullptr};
        QSharedPointer<AutoAnsThreadHandler> _autoAnsHandler;
};
