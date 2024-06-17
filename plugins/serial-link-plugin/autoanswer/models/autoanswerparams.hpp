// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "jsonutility/jsontypedefs.hpp"

class AutoAnsExpectedSerialMsg;


/** @brief This contains the module parameters received for the serial auto answer module */
class AutoAnswerParams : public QObject
{
    Q_OBJECT

    private:
        /** @brief Private class constructor
            @param expectedMessages The expected messages with the auto answers to send
            @param endOfLine This is what we considere as end of line when parsing the received data
            @param trim When equals to true, the lines are trimmed before being tested
            @param isMultiLineTest If true, we test the expected pattern on all the lines.
                                   If false, we test the expected pattern line by line
            @param displayStats If true, we display in logs some statisticas about the received and
                                sent messages managed by the auto answer process
            @param forceFlush If true, we force the flushing when writing data in serial link */
        explicit AutoAnswerParams(const QVector<AutoAnsExpectedSerialMsg *> &expectedMessages,
                                  const QString &endOfLine,
                                  bool trim,
                                  bool isMultiLineTest,
                                  bool displayStats,
                                  bool forceFlush,
                                  QObject *parent = nullptr);

    public:
        /** @brief Class destructor */
        virtual ~AutoAnswerParams() override;

    public:
        /** @brief Get the string which represents the end of line */
        const QString &getEndOfLine() const { return _endOfLine; }

        /** @brief Get the trim behavior when receiving new lines
            @return True if we trim the received lines */
        bool isTrim() const { return _trim; }

        /** @brief Say if we test the expected messages line by line or on all lines at once */
        bool isMultiLineTest() const { return _isMultiLineTest; }

        /** @brief Say if we manage the display of statistics in the auto answer process */
        bool isDisplayingStats() const { return _displayStats; }

        /** @brief Say if we force flushing when writing messages */
        bool isForcingFlush() const { return _forceFlush; }

        /** @brief Get the expected messages and message to send */
        const QVector<AutoAnsExpectedSerialMsg *> &getExpectedMessages() const
        { return _expectedMessages; }

    public:
        /** @brief Parse the @ref AutoAnswerParams model from the auto answer module param
            @param expectedMessages The expected messages set by user on the sequence file
            @param msgsToSend The messages to send after having received new messages
            @param endOfLine This is what we considere as end of line when parsing the received data
            @param trim When equals to true, the lines are trimmed before being tested
            @param isMultiLineTest If true, we test the expected pattern on all the lines.
                                   If false, we test the expected pattern line by line
            @param displayStats If true, we display in logs some statisticas about the received and
                                sent messages managed by the auto answer process
            @param forceFlush If true, we force the flushing when writing data in serial link
            @param errors If the parameters haven't been correctly set, this array is filled with
                          informations to display to the user; and so he can understand what to
                          correct
            @return The created @ref AutoAnswerParams or nullptr if a problem occurred */
        static AutoAnswerParams *parseAutoAnsParams(const QVector<QString> &expectedMessages,
                                                    const QVector<QString> &msgsToSend,
                                                    const QString &endOfLine,
                                                    bool trim,
                                                    bool isMultiLineTest,
                                                    bool displayStats,
                                                    bool forceFlush,
                                                    JsonArray &errors);

    public:
        static const constexpr char *ExpectedMessagesParamName = "expectedMessages";
        static const constexpr char *ExpectedMessagesParamDesc = "This contains the patterns of "
                                                                 "regular expression to test. No "
                                                                 "need of captured groups.";

    private:
        static const constexpr char *InvalidExpectedRegExp = "A problem occurred in the serial "
                                                             "link auto answer sequence when "
                                                             "trying to parse the "
                                                             "expectedMessages, it's not a valid "
                                                             "regexp: %1";

    private:
        QVector<AutoAnsExpectedSerialMsg *> _expectedMessages;

        QString _endOfLine;
        bool _trim;
        bool _isMultiLineTest;
        bool _displayStats;
        bool _forceFlush;
};
