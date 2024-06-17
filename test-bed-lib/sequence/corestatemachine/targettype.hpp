// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QtGlobal>


/** @brief Give the target type of a node: if it can target or can be targetted */
namespace TargetType
{
    /** @attention The enum values has to be kept in this order, because we compare them together to
                   know how nodes can be targeted or can target */
    enum Enum : quint8
    {
        None = 0,
        OnlyOne = 1,
        Multiple = 2
    };
}
