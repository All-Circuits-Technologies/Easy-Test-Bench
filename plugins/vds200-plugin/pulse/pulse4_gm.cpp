// SPDX-FileCopyrightText: 2021 - 2022 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "pulse4_gm.hpp"

#include "vds200-plugin/pulseCmd/pulsecmd.hpp"
#include "vds200-plugin/pulseCmd/pulseenum.hpp"


Pulse4_GM::Pulse4_GM(QObject *parent) :
    Pulse4(staticMetaObject.className(),
           pulseName,
           parent),
    _pulseCmd(new PulseCmd(PulseEnum::DP, this))
{
    setAPulseCmd(_pulseCmd);
}

Pulse4_GM::~Pulse4_GM()
{
}
