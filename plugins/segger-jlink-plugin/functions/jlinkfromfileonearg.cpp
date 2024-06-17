// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "jlinkfromfileonearg.hpp"


JLinkFromFileOneArg::JLinkFromFileOneArg(QObject *parent)
    : AJLinkIntfFromFile{1, staticMetaObject.className(), parent}
{
}

JLinkFromFileOneArg::~JLinkFromFileOneArg()
{
}
