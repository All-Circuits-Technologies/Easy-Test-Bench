// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "modules/readandwrite/acanmsgsequence.hpp"

#include <QCanBusFrame>

class PCanSequenceInfo;


/** @brief Abstract sequence module class for the CAN writing modules */
class ACanWriting : public ACanMsgSequence
{
    Q_OBJECT

    protected:
        /** @brief Class constructor
            @param moduleClassName The class name of the module
            @param sendMultipleCmd If true, it means that we manage multiple message sending in the
                                   module process
            @param exportReceivedMsgs If true, that means we export the received message(s) to the
                                      module output
            @param parent The parent instance class */
        explicit ACanWriting(const QString &moduleClassName,
                             bool sendMultipleCmd,
                             bool exportReceivedAnswers,
                             QObject *parent = nullptr);

    public:
        /** @brief Class destructor */
        virtual ~ACanWriting() override;

    protected:
        /** @copydoc ACanMsgSequence::initParamsFormat
            @note @ref BitrateSwitchParamName : bool (Activate, or not, the bitrate switch. This is
                                                      only relevant when using CAN FD.)
            @note @ref ExtendedFrameFormatParamName : bool (Activate, or not, the extended frame
                                                            format for the message sent.) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @copydoc ACanMsgSequence::initMultipleMsgsParamsFormat
            @note @ref CanMsgsToSendParamName : List<string> (The CAN messages to send. The messages
                                                              are stored in a string with the
                                                              following format:
                                                              \"CAN_ID/PAYLOAD_LENGTH/PAYLOAD\") */
        virtual bool initMultipleMsgsParamsFormat(JsonArray &paramsFormat) const override;

        /** @copydoc ACanMsgSequence::initSingleMsgParamsFormat
            @note @ref CanMsgToSendParamName : List<string> (The CAN MSG to send. The message is
                                                             stored in a string with the following
                                                             format:
                                                             \"CAN_ID/PAYLOAD_LENGTH/PAYLOAD\) */
        virtual bool initSingleMsgParamsFormat(JsonArray &paramsFormat) const override;

        /** @copydoc ACanMsgSequence::getExpectedMsgsIdsParamDesc */
        virtual QString getExpectedMsgsIdsParamDesc() const override;

        /** @copydoc ACanMsgSequence::parseInfoFromParameters */
        virtual bool parseInfoFromParameters(const JsonArray &parameters,
                                             CanMsgSeqParams &canMsgSeqParams,
                                             JsonArray &errors) override;

        /** @copydoc ACanMsgSequence::parseMultipleMsgsInfoFromParameters */
        virtual bool parseMultipleMsgsInfoFromParameters(const JsonArray &parameters,
                                                         CanMsgSeqParams &canMsgSeqParams,
                                                         JsonArray &errors) override;

        /** @copydoc ACanMsgSequence::parseSingleMsgInfoFromParameters */
        virtual bool parseSingleMsgInfoFromParameters(const JsonArray &parameters,
                                                      CanMsgSeqParams &canMsgSeqParams,
                                                      JsonArray &errors) override;

    private:
        static const constexpr char *BitrateSwitchParamName = "bitrateSwitch";
        static const constexpr char *BitrateSwitchParamDesc = "Activate, or not, the bitrate "
                                                              "switch. This is only relevant when "
                                                              "using CAN FD.";

        static const constexpr char *ExtendedFrameFormatParamName = "extendedFrameFormat";
        static const constexpr char *ExtendedFrameFormatParamDesc = "Activate, or not, the "
                                                                    "extended frame format for the "
                                                                    "message sent.";

        static const constexpr char *CanMsgToSendParamName = "canMsgToSend";
        static const constexpr char *CanMsgToSendParamDesc = "The CAN MSG to send. The message is "
                                                             "stored in a string with the "
                                                             "following format: "
                                                             "\"CAN_ID/PAYLOAD_LENGTH/PAYLOAD\"";

        static const constexpr char *CanMsgsToSendParamName = "canMsgsToSend";
        static const constexpr char *CanMsgsToSendParamDesc = "The CAN messages to send. The "
                                                              "messages are stored in a string "
                                                              "with the following format: "
                                                              "\"CAN_ID/PAYLOAD_LENGTH/PAYLOAD\"";

        static const constexpr char *WriteExpectedMsgsIdsParamDesc = "%1 The element index in this "
                                                                     "list matches the CAN msg "
                                                                     "list index. "
                                                                     "Therefore, to find the "
                                                                     "expected message of a "
                                                                     "sending msg, you have to get "
                                                                     "the answer at the same index "
                                                                     "in this list."
                                                                     "If the list is empty, that "
                                                                     "means that no answers are "
                                                                     "expected at all for all the "
                                                                     "commands."
                                                                     "If there is at least one "
                                                                     "element in this list, the "
                                                                     "size of this list must "
                                                                     "match the list of commands.";

        static const constexpr char *ExpectedMsgIdsLengthError = QT_TR_NOOP("(TR) The list of "
                                                                            "expected message ids "
                                                                            "must have the same "
                                                                            "length as the cmd "
                                                                            "list length");
};
