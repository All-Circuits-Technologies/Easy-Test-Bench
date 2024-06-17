// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

/** @brief List of pulse header */
class PulseEnum : public QObject
{
    Q_OBJECT

    public:
        enum Enum
        {
            DN, //Pulse4_Iso
            DP, //Pulse4_GM
            DL, //LoadDump
            DS, //SineWave
            DW, //Sweep
            DJ, //Jaso
            DA, //Pulse2b
            DU, //VoltagInterruption
            DR, //Ramp
            DE, //External
            Unknown
        };

        Q_ENUM(Enum)

    public:
        /** @brief Parse the enum from string
            @param strValue The string to parse
            @return The enum parsed */
        static Enum parseFromString(const QString &strValue);

        /** @brief Return a string representation of the Flow Control
            @param flowControl The type to get the string representation from
            @return The string representation */
        static QString toString(Enum pulseHeader);
};
