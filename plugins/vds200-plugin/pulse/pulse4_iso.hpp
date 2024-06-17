// SPDX-FileCopyrightText: 2021 - 2022 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "pulse4.hpp"


/** @brief This module is used to generate Pulse 4 with ISO standard*/
class Pulse4_ISO : public Pulse4
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(Pulse4_ISO)

    public:
        /** @brief Class constructor
            @param parent The parent instance class */
        Q_INVOKABLE explicit Pulse4_ISO(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~Pulse4_ISO() override;

    private:
        /** @brief String name of the pulse */
        static const constexpr char *pulseName = "PULSE 4 [ISO]";

    private:
        PulseCmd *_pulseCmd{nullptr};
};
