// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


/** @brief The pulse flag */
class PulseFlag : public QObject
{
    Q_OBJECT

    public:
        /** @brief Enum use to define parameters flag
            @note Used to know which parameters must be initialized in initParamsFormat()
            @note Used to know which parameters must be set in validateAndSetParameters() */
        enum Flag: quint16
        {
            VbFlag = 1,         // 0000 0000 0000 0001
            VsFlag = 2,         // 0000 0000 0000 0010
            VaFlag = 4,         // 0000 0000 0000 0100
            VpFlag = 8,         // 0000 0000 0000 1000
            T6Flag = 16,        // 0000 0000 0001 0000
            T7Flag = 32,        // 0000 0000 0010 0000
            TfFlag = 64,        // 0000 0000 0100 0000
            T8Flag = 128,       // 0000 0000 1000 0000
            InterFlag = 256,    // 0000 0001 0000 0000
            NbCyclesFlag = 512, // 0000 0010 0000 0000
            TriggerFlag = 1024, // 0000 0100 0000 0000
            CurrentFlag = 2048, // 0000 1000 0000 0000
        };
        Q_DECLARE_FLAGS(Flags, Flag)
        Q_FLAG(Flags)

    public:
        /** @brief Generate a flags according to the flagList
            @note The flagList is filled by specific pulses (ex: pulse4) & transmit as parameter
            @param flagList The list of all the sequence parameters to use in the APulse Class
            @return The flags */
        static PulseFlag::Flags initParametersFlag(const QVector<PulseFlag::Flag> &flagList);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(PulseFlag::Flags)
