// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "autoansexpectedserialmsg.hpp"

#include <QRegularExpression>


AutoAnsExpectedSerialMsg::AutoAnsExpectedSerialMsg(QRegularExpression *expectedMessage,
                                                   const QVector<AutoAnsMsgToSend *> msgsToSend,
                                                   QObject *parent)
    : QObject{parent},
    _expectedMessage{expectedMessage},
    _msgsToSend{msgsToSend}
{
}

AutoAnsExpectedSerialMsg::~AutoAnsExpectedSerialMsg()
{
    delete _expectedMessage;
}
