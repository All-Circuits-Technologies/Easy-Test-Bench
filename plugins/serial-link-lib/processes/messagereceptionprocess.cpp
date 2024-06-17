// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "messagereceptionprocess.hpp"

#include <QDebug>

#include "stringutility/stringhelper.hpp"


MessageReceptionProcess::MessageReceptionProcess(
    const QString &endOfLine,
    bool trim,
    bool isMultiLineTest,
    QObject *parent) :
    QObject{parent},
    _endOfLine{endOfLine},
    _trim{trim},
    _isMultiLineTest{isMultiLineTest}
{
}

MessageReceptionProcess::~MessageReceptionProcess()
{
}

SerialLinkReceivedReplyType::Enum MessageReceptionProcess::parseAndManageReceivedMessage(
    const QByteArray &data,
    const QRegularExpression &expectedReply,
    QStringList &receivedLines,
    QString &tmpBuffer,
    QVector<QString> &capturedElements,
    const QRegularExpression &possibleFailure) const
{
    if(!manageReceivedLines(data, receivedLines, tmpBuffer))
    {
        // Nothing important has changed, useless to go forward
        return SerialLinkReceivedReplyType::NoAnswer;
    }

    return manageReceivedMessage(expectedReply,
                                         receivedLines,
                                         tmpBuffer,
                                         capturedElements,
                                         possibleFailure);
}

SerialLinkReceivedReplyType::Enum MessageReceptionProcess::manageReceivedMessage(
    const QRegularExpression &expectedReply,
    const QStringList &receivedLines,
    const QString &tmpBuffer,
    QVector<QString> &capturedElements,
    const QRegularExpression &possibleFailure,
    int *multiLineStartIndex) const
{
    SerialLinkReceivedReplyType::Enum receivedReply = SerialLinkReceivedReplyType::NoAnswer;

    if(_isMultiLineTest)
    {
        QString tmpLine = receivedLines.join(_endOfLine) + tmpBuffer;
        int startIndex = (multiLineStartIndex != nullptr) ? *multiLineStartIndex : 0;
        receivedReply = manageParsingRegExpTxt(tmpLine,
                                               expectedReply,
                                               possibleFailure,
                                               capturedElements,
                                               startIndex);

        if(multiLineStartIndex != nullptr)
        {
            *multiLineStartIndex = guessNewStartIndex(tmpLine,
                                                      capturedElements,
                                                      startIndex);
        }
    }
    else
    {
        for(auto citer = receivedLines.cbegin(); citer != receivedLines.cend(); ++citer)
        {
            receivedReply = manageParsingRegExpTxt(*citer,
                                                   expectedReply,
                                                   possibleFailure,
                                                   capturedElements);

            if(receivedReply != SerialLinkReceivedReplyType::NoAnswer)
            {
                break;
            }
        }

        if(receivedReply == SerialLinkReceivedReplyType::NoAnswer && !tmpBuffer.isEmpty())
        {
            receivedReply = manageParsingRegExpTxt(tmpBuffer,
                                                   expectedReply,
                                                   possibleFailure,
                                                   capturedElements);
        }
    }

    return receivedReply;

}

bool MessageReceptionProcess::manageReceivedLines(const QByteArray &data,
                                                  QStringList &receivedLines,
                                                  QString &tmpBuffer) const
{
    int receivedLinesLength = receivedLines.length();
    int tmpBufferLength = tmpBuffer.length();

    const QString receivedData = QString::fromLatin1(data.data());
    const QString textBuffer = (tmpBuffer + receivedData);

    QStringList newLines = StringHelper::splitAndKeepSepOnEnd(textBuffer,
                                                              _endOfLine,
                                                              Qt::SkipEmptyParts);
    const bool isLastLineEntirelyReceived = textBuffer.endsWith(_endOfLine);
    tmpBuffer.clear();

    /* Postpone last line if not fully received yet */
    if(!isLastLineEntirelyReceived)
    {
        tmpBuffer = newLines.last();
        newLines.removeLast();
    }

    receivedLines.append(newLines);

    return receivedLinesLength != receivedLines.length() || tmpBufferLength != tmpBuffer.length();
}

int MessageReceptionProcess::guessNewStartIndex(const QString &line,
                                                const QVector<QString> &capturedElements,
                                                int startIndex) const
{
    int newStartIndex = startIndex;

    for(auto citer = capturedElements.cbegin(); citer != capturedElements.cend(); ++citer)
    {
        int tmpStartIndex = line.indexOf(*citer, startIndex);

        if(tmpStartIndex == -1)
        {
            // This case can't happen
            continue;
        }

        int tmpEnd = tmpStartIndex + citer->length();

        if(tmpEnd > newStartIndex)
        {
            newStartIndex = tmpEnd;
        }
    }

    return newStartIndex;
}

SerialLinkReceivedReplyType::Enum MessageReceptionProcess::manageParsingRegExpTxt(
    const QString &line,
    const QRegularExpression &expectedReply,
    const QRegularExpression &possibleFailure,
    QVector<QString> &capturedElements,
    int startIndex) const
{
    QString tmpLine = line.mid(startIndex);

    if(_trim)
    {
        tmpLine = line.trimmed();
    }

    QRegularExpressionMatch match = expectedReply.match(tmpLine);

    if(match.hasMatch())
    {
        for(int i = 0; i <= match.lastCapturedIndex(); ++i)
        {
            capturedElements.append(match.captured(i));
        }

        return SerialLinkReceivedReplyType::ExpectedAnswer;
    }

    if(possibleFailure.isValid() && !possibleFailure.pattern().isNull())
    {
        QRegularExpressionMatch matchFailure = possibleFailure.match(tmpLine);
        if(matchFailure.hasMatch())
        {
            return SerialLinkReceivedReplyType::FailureAnswer;
        }
    }

    return SerialLinkReceivedReplyType::NoAnswer;
}
