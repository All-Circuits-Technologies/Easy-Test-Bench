// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "autoansthreadhandler.hpp"


AutoAnsThreadHandler::AutoAnsThreadHandler(QObject *parent)
    : QObject{parent}, BaseThreadHandler<AutoAnsProcessThread>(new AutoAnsProcessThread())
{
}

AutoAnsThreadHandler::~AutoAnsThreadHandler()
{
}
