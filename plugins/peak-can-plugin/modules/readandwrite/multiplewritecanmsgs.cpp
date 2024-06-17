// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "multiplewritecanmsgs.hpp"


MultipleWriteCanMsgs::MultipleWriteCanMsgs(QObject *parent)
    : ACanWriting{staticMetaObject.className(), true, false, parent}
{
}

MultipleWriteCanMsgs::~MultipleWriteCanMsgs()
{
}
