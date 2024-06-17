// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "sequencestatetype.hpp"


void SequenceStateType::registerMetaType()
{
    qRegisterMetaType<SequenceStateType::Enum>("SequenceStateType::Enum");
}
