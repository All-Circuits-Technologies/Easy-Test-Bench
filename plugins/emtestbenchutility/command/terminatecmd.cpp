// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "terminatecmd.hpp"


TerminateCmd::TerminateCmd(QObject *parent) : ACmd(terminateCmd, parent)
{
}

TerminateCmd::~TerminateCmd()
{
}

bool TerminateCmd::getParameters(QVector<quint16> &params)
{
    Q_UNUSED(params);
    return true;
}
