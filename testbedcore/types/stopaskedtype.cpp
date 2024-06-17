// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "stopaskedtype.hpp"


void StopAskedType::registerMetaType()
{
    qRegisterMetaType<StopAskedType::Enum>("StopAskedType::Enum");
}
