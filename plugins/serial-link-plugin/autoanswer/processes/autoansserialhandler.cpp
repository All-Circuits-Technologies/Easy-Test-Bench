// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "autoansserialhandler.hpp"


AutoAnsSerialHandler::AutoAnsSerialHandler(QObject *parent)
    : QObject{parent},
    BaseThreadHandler<AutoAnswerSerialLinkThread>(new AutoAnswerSerialLinkThread())
{
}

AutoAnsSerialHandler::~AutoAnsSerialHandler()
{
}
