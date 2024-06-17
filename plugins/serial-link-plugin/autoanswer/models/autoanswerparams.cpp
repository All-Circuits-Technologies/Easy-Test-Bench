// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "autoanswerparams.hpp"

#include <QDebug>
#include <QHash>
#include <QRegularExpression>

#include "abstractpluginelements/defaultsequenceinstancemodule.hpp"

#include "serial-link-lib/serialpluginlibconstants.hpp"

#include "autoanswer/models/autoansexpectedserialmsg.hpp"
#include "autoanswer/models/autoansmsgtosend.hpp"


AutoAnswerParams::AutoAnswerParams(const QVector<AutoAnsExpectedSerialMsg *> &expectedMessages,
                                   const QString &endOfLine,
                                   bool trim,
                                   bool isMultiLineTest,
                                   bool displayStats,
                                   bool forceFlush,
                                   QObject *parent)
    : QObject{parent},
    _expectedMessages{expectedMessages},
    _endOfLine{endOfLine},
    _trim{trim},
    _isMultiLineTest{isMultiLineTest},
    _displayStats{displayStats},
    _forceFlush{forceFlush}
{
}

AutoAnswerParams::~AutoAnswerParams()
{
    qDeleteAll(_expectedMessages);
}

AutoAnswerParams *AutoAnswerParams::parseAutoAnsParams(const QVector<QString> &expectedMessages,
                                                       const QVector<QString> &msgsToSend,
                                                       const QString &endOfLine,
                                                       bool trim,
                                                       bool isMultiLineTest,
                                                       bool displayStats,
                                                       bool forceFlush,
                                                       JsonArray &errors)
{
    errors = {};

    QHash<int, QVector<AutoAnsMsgToSend *>> tmpMsgsToSend;
    int expectedMsgsLength = expectedMessages.length();

    auto removeMsgsToSend = [&tmpMsgsToSend]() {
        for(auto citer = tmpMsgsToSend.cbegin(); citer != tmpMsgsToSend.cend(); ++citer)
        {
            qDeleteAll(citer.value());
        }
    };

    for(auto citer = msgsToSend.cbegin(); citer != msgsToSend.cend(); ++citer)
    {
        int expectedMsgIdx = -1;
        AutoAnsMsgToSend *msgToSend = AutoAnsMsgToSend::parseAutoAnsMsgToSend(*citer,
                                                                              expectedMsgsLength,
                                                                              expectedMsgIdx,
                                                                              errors);

        if(msgToSend == nullptr)
        {
            // An error occurred in the process
            removeMsgsToSend();
            return nullptr;
        }

        if(!tmpMsgsToSend.contains(expectedMsgIdx))
        {
            tmpMsgsToSend[expectedMsgIdx] = {};
        }

        tmpMsgsToSend[expectedMsgIdx].append(msgToSend);
    }

    QVector<AutoAnsExpectedSerialMsg *> autoAnsExpectedMsgs;

    for(int idx = 0; idx < expectedMessages.length(); ++idx)
    {
        const QString &expectedMessage = expectedMessages.at(idx);
        QRegularExpression *exp = new QRegularExpression(expectedMessage);

        if(!exp->isValid())
        {
            qWarning() << "The expression: " << expectedMessage << ", isn't a valid RegExp";
            DefaultSequenceInstanceModule::fillErrorArray(ExpectedMessagesParamName,
                                                          QString(InvalidExpectedRegExp)
                                                              .arg(expectedMessage),
                                                          errors);
            qDeleteAll(autoAnsExpectedMsgs);
            removeMsgsToSend();
            delete exp;
            return nullptr;
        }

        AutoAnsExpectedSerialMsg *autoAnsExpMsg = new AutoAnsExpectedSerialMsg(
            exp,
            // If the idx is not present in the QHash, the method will create a default value.
            // Here it will be an empty QVector
            tmpMsgsToSend.take(idx));

        autoAnsExpectedMsgs.append(autoAnsExpMsg);
    }

    return new AutoAnswerParams(
        autoAnsExpectedMsgs,
        endOfLine,
        trim,
        isMultiLineTest,
        displayStats,
        forceFlush);
}
