// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QtGlobal>


/** @brief Contains the constants of the plugin */
namespace PeakCanConstants
{
    /** @brief This contains shared information between modules for their entries */
    namespace ModulesEntries
    {
        /** @brief This contains shared informations between modules for their params */
        namespace Params
        {
            /** @brief Defines the default value for the module param bitrate switch state */
            static const constexpr bool HasBitrateSwitchDefaultValue = true;

            /** @brief Defines the default value for the module param extended frame format state */
            static const constexpr bool IsExtendedFrameFormatDefaultValue = false;

            /** @brief Defines the default value for the module param flexible data rate state */
            static const constexpr bool HasFlexibleDataRateDefaultValue = false;
        }
    }

    /** @brief This defines limits applied to particular values */
    namespace Limits
    {
        /** @brief Defines the value max a command id may have */
        static const constexpr quint32 MaxCmdIdValue = 0x3FFFFFFF;
    }

    /** @brief This defines constants for parsing process */
    namespace Parser
    {
        /** @brief The character used to separate the different parts in the expected messages */
        static const constexpr char StrSeparator = '/';
    }
}
