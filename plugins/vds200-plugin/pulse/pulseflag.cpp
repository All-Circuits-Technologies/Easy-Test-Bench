// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "pulseflag.hpp"


PulseFlag::Flags PulseFlag::initParametersFlag(const QVector<PulseFlag::Flag> &flagList)
{
    Flags tmpFlag;

    for(auto citer = flagList.cbegin(); citer != flagList.cend(); ++citer)
    {
        tmpFlag.setFlag(*citer, true);
    }

    return tmpFlag;
}
