// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>
#include "testbedcore/testbedglobal.hpp"


/** @brief List of the loading status */
class TESTBEDLIB_DLLSPEC LoadingStatus : public QObject
{
    Q_OBJECT

    public:
        enum Enum:quint16
        {
            Started = 0,    /*beginning of loading (0)*/
            Failed = 1,     /*loading interruption (1)*/
            Finished = 2,   /*successful loading (2)*/
        };
        Q_ENUM(Enum)

    public:
        /** @brief Register meta type  */
        static void RegisterMetaType();
};
