// SPDX-FileCopyrightText: 2021 - 2022 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "voltageinterruption.hpp"

#include "vds200-plugin/pulseCmd/pulsecmd.hpp"
#include "vds200-plugin/pulseCmd/pulseenum.hpp"


VoltageInterruption::VoltageInterruption(QObject *parent) :
    APulse(_flags,
           staticMetaObject.className(),
           pulseName,
           parent),
    _pulseCmd(new PulseCmd(PulseEnum::DU, this))
{
    setAPulseCmd(_pulseCmd);
}

VoltageInterruption::~VoltageInterruption()
{
}
