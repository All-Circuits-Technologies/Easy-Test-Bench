// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "serialpluginlibconstants.hpp"
#include "types/seriallinkreceivedreplytype.hpp"


/** @brief This class contains useful methods to parse and manage the serial link message
           received */
class MessageReceptionProcess : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param endOfLine This is the expected string to signify the end of a line
            @param trim True to trim the end of each line
            @param isMultiLineTest True if the expected regexp is tested on several lines or not
            @param parent The parent object instance */
        explicit MessageReceptionProcess(
            const QString &endOfLine,
            bool trim,
            bool isMultiLineTest,
            QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~MessageReceptionProcess() override;

    public:
        /** @brief Parse and manage the received data to test if the reply is an expected one,
                   a failure or not awaited.
            @note Because the serial message data arrives by packet and not by line, the method
                  parses the @ref data param, and with the @ref tmpBuffer, tries to recreate the
                  lines.
            @note If we are testing on multiline, the method will use @ref receivedLines regexp on
                  all the received lines at once.
                  If we are not, the method will test the lines one by one
            @note The method also tests not yet ended line (because, sometimes, what we expect is
                  sent without an end of line).
            @param data The data received from the serial link
            @param expectedReply This is the expected reply to search in the message received
            @param receivedLines This contains the received and ended lines
            @param tmpBuffer This is the buffer which contains the begin of the last line which has
                             not been ended for now
            @param capturedElements This contains the captured elements in the lines. The captured
                                    elements are linked to the @ref expectedReply regexp.
                                    If the regexp captures elemnents, the elements will be in this
                                    list.
            @param possibleFailure If given, the method tests if the received lines matches the
                                   failure regexp, if it does, that means an error occurred.
            @return The result of parsing: have we received the expected or failure answer, or
                    nothing has been found yet */
        SerialLinkReceivedReplyType::Enum parseAndManageReceivedMessage(
            const QByteArray &data,
            const QRegularExpression &expectedReply,
            QStringList &receivedLines,
            QString &tmpBuffer,
            QVector<QString> &capturedElements,
            const QRegularExpression &possibleFailure =
                SerialPluginLibConstants::Defaults::DefaultExpectedFailure) const;

        /** @brief Manages the received lines to test if the reply is an expected one, a failure or
                   not awaited.
            @note If we are testing on multiline, the method will use @ref receivedLines regexp on
                  all the received lines at once.
                  If we are not, the method will test the lines one by one
            @note The method also tests not yet ended line (because, sometimes, what we expect is
                  sent without an end of line).
            @param expectedReply This is the expected reply to search in the message received
            @param receivedLines This contains the received and ended lines
            @param tmpBuffer This is the buffer which contains the begin of the last line which has
                             not been ended for now
            @param capturedElements This contains the captured elements in the lines. The captured
                                    elements are linked to the @ref expectedReply regexp.
                                    If the regexp captures elemnents, the elements will be in this
                                    list.
            @param possibleFailure If given, the method tests if the received lines matches the
                                   failure regexp, if it does, that means an error occurred.
            @param multiLineStartIndex This param is only used if we are testing on multiline.
                                       If not null, the test of expected answer or possible failure
                                       will be done from this index.
                                       The index is global at the @ref receivedLines and
                                       @ref tmpBuffer concatenated.
                                       At the end of the process, this value will be updated with
                                       the new multiline start index (from which we can start the
                                       test)
            @return The result of parsing: have we received the expected or failure answer, or
                    nothing has been found yet */
        SerialLinkReceivedReplyType::Enum manageReceivedMessage(
            const QRegularExpression &expectedReply,
            const QStringList &receivedLines,
            const QString &tmpBuffer,
            QVector<QString> &capturedElements,
            const QRegularExpression &possibleFailure =
                SerialPluginLibConstants::Defaults::DefaultExpectedFailure,
            int *multiLineStartIndex = nullptr) const;

        /** @brief Parse and create the received lines from the received data
            @param data The data newly received from serial interface
            @param receivedLines The complete lines already received
            @param tmpBuffer At the method starts, this is the partial line lastly received, before
                             the @ref data parsing (we wait an end of line to add its content to
                             @ref receivedLines).
                             At the method ends, this contains the partial line after having parsed
                             the @ref data param
            @return True if we have detected new received lines or new relevant elements have been
                    added to buffer
                    False if no new relevant elements have been received */
        bool manageReceivedLines(const QByteArray &data,
                                 QStringList &receivedLines,
                                 QString &tmpBuffer) const;

        /** @brief The method guesses the new start index after the captured elements, to be sure
                   next time to not include the same captured elements again
            @param line The line which contains all the elements tested
            @param capturedElements The captured elements from the given @ref line
            @param startIndex The current start index in the @ref line from which we got the
                              captured elements
            @return The new start index to use, in order to not include the captured elements
                    again */
        int guessNewStartIndex(const QString &line,
                               const QVector<QString> &capturedElements,
                               int startIndex) const;

    private:

        /** @brief The method tests if the @ref text contains the @ref expectedReply information.
                   If it is, the method captured the elements and add them in @ref capturedElements
            @param line The line to parse
            @param expectedReply The expected reply regexp to test
            @param possibleFailure The failure regexp to test (if the pattern is null or invalid,
                                   nothing is done)
            @param capturedElements Those are the captured elements by the expected reply regex
                                    Attention: the first element is the result of whole match;
                                    therefore you have to add one to the captured element index you
                                    expect.
            @param startIndex The index from which we test the regular expressions in the given
                              @ref line
            @return The received reply status */
        SerialLinkReceivedReplyType::Enum manageParsingRegExpTxt(
            const QString &line,
            const QRegularExpression &expectedReply,
            const QRegularExpression &possibleFailure,
            QVector<QString> &capturedElements,
            int startIndex = 0) const;

    private:
        QString _endOfLine;
        bool _trim;
        bool _isMultiLineTest;
};
