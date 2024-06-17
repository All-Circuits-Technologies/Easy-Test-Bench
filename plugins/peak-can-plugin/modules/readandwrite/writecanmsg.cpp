// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "writecanmsg.hpp"


WriteCanMsg::WriteCanMsg(QObject *parent)
    : ACanWriting{staticMetaObject.className(), false, false, parent}
{
}

WriteCanMsg::~WriteCanMsg()
{
}
