// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "apulsecmd.hpp"


APulseCmd::APulseCmd(PulseEnum::Enum header, QObject *parent):
    ACmd(PulseEnum::toString(header), parent)
{
}

APulseCmd::~APulseCmd()
{
}
