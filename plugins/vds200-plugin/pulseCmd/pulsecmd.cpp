// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "pulsecmd.hpp"


PulseCmd::PulseCmd(PulseEnum::Enum header, QObject *parent) :
    APulseCmd(header, parent),
    _header(header)
{
}

PulseCmd::~PulseCmd()
{
}

bool PulseCmd::getParameters(QVector<quint16> &params)
{
    bool success = true;

    switch (_header) {
        case PulseEnum::DU:
            params = {getVb(),
                      getT6(),
                      getInterruption(),
                      getNbCycles(),
                      getTrigger(),
                      getCurrent()};
            break;

        case PulseEnum::DE:
            params = {getCurrent()};
            break;

        case PulseEnum::DR:
            params = {getVb(),
                      getVs(),
                      getTf(),
                      getCurrent()};
            break;

        case PulseEnum::DP:
        case PulseEnum::DN:
            params = {getVb(),
                      getVs(),
                      getVa(),
                      getT6(),
                      getT7(),
                      getT8(),
                      getTf(),
                      getInterruption(),
                      getNbCycles(),
                      getTrigger(),
                      getCurrent()};
            break;

        default:
            success = false;
            break;
    }

    return success;
}
