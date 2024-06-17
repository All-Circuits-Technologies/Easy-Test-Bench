// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "idncmd.hpp"


IdnCmd::IdnCmd(QObject *parent) : ACmd(vdsIdnCmd, parent)
{
}

IdnCmd::~IdnCmd()
{
}

bool IdnCmd::getParameters(QVector<quint16> &params)
{
    Q_UNUSED(params);
    return true;
}
