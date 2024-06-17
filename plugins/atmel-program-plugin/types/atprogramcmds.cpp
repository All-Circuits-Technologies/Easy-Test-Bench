// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "atprogramcmds.hpp"

#include <QMetaEnum>


QString AtProgramCmds::toString(AtProgramCmds::Enum cmd)
{
    return QString::fromLatin1(QMetaEnum::fromType<Enum>().valueToKey(cmd)).toLower();
}
