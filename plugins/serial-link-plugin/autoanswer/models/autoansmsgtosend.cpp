// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "autoansmsgtosend.hpp"

#include "abstractpluginelements/defaultsequenceinstancemodule.hpp"
#include "serial-link-lib/serialpluginlibconstants.hpp"


AutoAnsMsgToSend::AutoAnsMsgToSend(const QString &msgToSend,
                                   int timeToWaitBeforeSending,
                                   QObject *parent)
    : QObject{parent},
    _msgToSend{msgToSend},
    _timeToWaitBeforeSending{timeToWaitBeforeSending}
{
}

AutoAnsMsgToSend::~AutoAnsMsgToSend()
{
}

AutoAnsMsgToSend *AutoAnsMsgToSend::parseAutoAnsMsgToSend(const QString &msgFromParam,
                                                          int expectedMsgsLength,
                                                          int &expectedMsgIdx,
                                                          JsonArray &errors)
{
    QStringList parsedMsgToSend = msgFromParam.split(
        SerialPluginLibConstants::Parser::StrSeparator);

    const int parsedLength = parsedMsgToSend.length();
    if(parsedLength < MsgToSendPartNb)
    {
        DefaultSequenceInstanceModule::fillErrorArray(MsgsToSendParamName,
                                                      QString(WrongMsgToSendLength)
                                                          .arg(msgFromParam, MsgToSendPartNb),
                                                      errors);
        return nullptr;
    }

    bool ok = false;
    expectedMsgIdx = parsedMsgToSend[ExpectedMsgIdxInMsgToSend].toInt(&ok);

    if(!ok)
    {
        DefaultSequenceInstanceModule::fillErrorArray(MsgsToSendParamName,
                                                      QString(MsgToSendIdxParseError)
                                                          .arg(msgFromParam),
                                                      errors);
        return nullptr;
    }

    if(expectedMsgIdx < 0 || expectedMsgIdx >= expectedMsgsLength)
    {
        DefaultSequenceInstanceModule::fillErrorArray(MsgsToSendParamName,
                                                      QString(WrongExpectedMsgIdxError)
                                                          .arg(msgFromParam),
                                                      errors);
        return nullptr;
    }

    const QString msgToSend = parsedMsgToSend[MsgToSendIdxInMsgToSend];
    int timeToWaitBeforeSending = DefaultTimeToWaitBeforeSend;

    if(parsedLength >= MsgToSendPartWithTimerNb)
    {
        timeToWaitBeforeSending = parsedMsgToSend[TimeToWaitIdxInMsgToSend].toInt(&ok);

        if(!ok)
        {
            DefaultSequenceInstanceModule::fillErrorArray(MsgsToSendParamName,
                                                          QString(TimeToWaitParseError)
                                                              .arg(msgFromParam),
                                                          errors);
            return nullptr;
        }
    }

    return new AutoAnsMsgToSend(msgToSend, timeToWaitBeforeSending);
}
