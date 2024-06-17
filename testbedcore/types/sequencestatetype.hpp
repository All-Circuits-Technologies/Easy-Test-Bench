// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


/** @brief The state of the sequence module */
class SequenceStateType : public QObject
{
    Q_OBJECT

    public:
        enum Enum
        {
            Running = 1,
            InPause = 2,
            Stopped = 3,
            Unknown = 0
        };
        Q_ENUM(Enum)

    public:
        /** @brief Register meta type  */
        static void registerMetaType();
};
