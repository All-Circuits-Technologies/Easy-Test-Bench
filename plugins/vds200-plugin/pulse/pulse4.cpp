// SPDX-FileCopyrightText: 2021 - 2022 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "pulse4.hpp"


Pulse4::Pulse4(const QString &moduleClassName,
               const QString &pulseName,
               QObject *parent) :
    APulse(_flag,
           moduleClassName,
           pulseName,
           parent)
{
}

Pulse4::~Pulse4()
{
}
