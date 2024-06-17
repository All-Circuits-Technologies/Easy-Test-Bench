// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


/** @brief Represents the type of stop asked by user or application */
class StopAskedType : public QObject
{
    Q_OBJECT

    public:
        /** @brief Type of the stop asked */
        enum Enum {
            AskedByUser     = 1,
            EndOfProcess    = 2,
            NoStopAsked     = 0
        };
        Q_ENUM(Enum)

    public:
        /** @brief Register meta type  */
        static void registerMetaType();
};
