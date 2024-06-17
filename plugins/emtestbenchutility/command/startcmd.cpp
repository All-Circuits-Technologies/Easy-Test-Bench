// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "startcmd.hpp"


StartCmd::StartCmd(QObject *parent) : ACmd(startCmd, parent)
{
}

StartCmd::~StartCmd()
{
}

bool StartCmd::getParameters(QVector<quint16> &params)
{
    Q_UNUSED(params);
    return true;
}
