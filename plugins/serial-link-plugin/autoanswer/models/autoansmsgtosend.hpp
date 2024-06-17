// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "jsonutility/jsontypedefs.hpp"


/** @brief This represents the messages to send with the auto answer process */
class AutoAnsMsgToSend : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param msgToSend The message to send
            @param timeToWaitBeforeSending This is the time to wait in milliseconds between the
                                           expected message reception and the sending of the message
            @param parent The parent instance class */
        explicit AutoAnsMsgToSend(const QString &msgToSend,
                                  int timeToWaitBeforeSending = DefaultTimeToWaitBeforeSend,
                                  QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~AutoAnsMsgToSend() override;

    public:
        /** @brief Get the message  to send */
        const QString &getMsgToSend() const { return _msgToSend; }

        /** @brief Get the time to wait in milliseconds before sending message */
        int getTimeToWaitBeforeSending() const { return _timeToWaitBeforeSending; }

    public:
        /** @brief Parse the auto answer message
            @param msgFromParam The message received from the module parameter
            @param expectedMsgsLength This is the length of the expected messages list given to the
                                      module
            @param expectedMsgIdx The index of the expected message linked to the message to send
            @param errors If the parameters haven't been correctly set, this array is filled with
                          informations to display to the user; and so he can understand what to
                          correct
            @return The created @ref AutoAnsMsgToSend or nullptr if a problem occurred */
        static AutoAnsMsgToSend *parseAutoAnsMsgToSend(const QString &msgFromParam,
                                                       int expectedMsgsLength,
                                                       int &expectedMsgIdx,
                                                       JsonArray &errors);

    public:
        static const constexpr char *MsgsToSendParamName = "msgsToSend";
        static const constexpr char *MsgsToSendParamDesc = "The messages to send if the "
                                                           "expected message has been received. "
                                                           "The messages are stored in a string "
                                                           "with the following format: "
                                                           "\"MSG_IDX/MSG_CONTENT\","
                                                           "It's possible to add a time to wait "
                                                           "before to send a message, in that "
                                                           "case, the line has the following "
                                                           "format: "
                                                           "\"MSG_IDX/MSG_CONTENT/"
                                                           "TIME_TO_WAIT_IN_MS\","
                                                           "The MSG_IDX indicates the expected "
                                                           "message linked to this message to "
                                                           "send."
                                                           "The MSG_IDX can't overflow the "
                                                           "expected messages list length."
                                                           "The TIME_TO_WAIT_IN_MS is the time to "
                                                           "wait in milliseconds before sending "
                                                           "the message, if the value isn't "
                                                           "present or negative, we send the "
                                                           "message without waiting.";

    private:
        static const constexpr char *MsgToSendIdxParseError = "A problem occurred in the serial "
                                                              "link auto answer sequence when "
                                                              "trying to parse the msgToSend idx "
                                                              "part param: %1";

        static const constexpr char *TimeToWaitParseError = "A problem occurred in the serial link "
                                                            "auto answer sequence when trying to "
                                                            "parse the time to wait part param: %1";

        static const constexpr char *WrongMsgToSendLength = "A problem occurred in the serial link "
                                                            "auto answer sequence when trying to "
                                                            "parse the msgToSend param: %1, the "
                                                            "param has not the right parts length: "
                                                            "%2";

        static const constexpr char *WrongExpectedMsgIdxError = "A problem occurred in the serial "
                                                                "link auto answer sequence when "
                                                                "trying to parse the msgToSend, "
                                                                "the expected message index "
                                                                "doesn't exist: %1";

        static const constexpr int DefaultTimeToWaitBeforeSend = -1;

        static const constexpr int MsgToSendPartNb = 2;
        static const constexpr int ExpectedMsgIdxInMsgToSend = 0;
        static const constexpr int MsgToSendIdxInMsgToSend = 1;

        static const constexpr int MsgToSendPartWithTimerNb = 3;
        static const constexpr int TimeToWaitIdxInMsgToSend = 2;


    private:
        QString _msgToSend;
        int _timeToWaitBeforeSending{DefaultTimeToWaitBeforeSend};
};
