// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "multipleonlyreadcanmsgs.hpp"


MultipleOnlyReadCanMsgs::MultipleOnlyReadCanMsgs(QObject *parent)
    : ACanMsgSequence{staticMetaObject.className(), true, false, parent}
{
}

MultipleOnlyReadCanMsgs::~MultipleOnlyReadCanMsgs()
{
}
