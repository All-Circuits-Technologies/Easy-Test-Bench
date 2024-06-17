// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "onlyreadcanmsg.hpp"


OnlyReadCanMsg::OnlyReadCanMsg(QObject *parent)
    : ACanMsgSequence{staticMetaObject.className(), false, false, parent}
{
}

OnlyReadCanMsg::~OnlyReadCanMsg()
{
}
