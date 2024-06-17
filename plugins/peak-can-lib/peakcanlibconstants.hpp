// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QtGlobal>


/** @brief Contains the constants of the PEAK Can lib */
namespace PeakCanLibConstants
{
    /** @brief This contains shared information between modules for their entries */
    namespace ModulesEntries
    {
        /** @brief This contains shared informations between modules for their inputs */
        namespace Inputs
        {
            /** @brief Defines the name of the PEAK CAN interface input */
            static const constexpr char *InputPeakCanIntfName = "inputPeakCanIntf";

            /** @brief Defines the description of the PEAK CAN interface input */
            static const constexpr char *InputPeakCanIntfDesc = "The input contains the name of "
                                                                "the peak CAN interface";
        }
    }
}
