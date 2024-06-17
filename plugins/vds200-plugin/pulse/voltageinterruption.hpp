// SPDX-FileCopyrightText: 2021 - 2022 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "apulse.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"
#include "pulse/pulseflag.hpp"

class PulseCmd;
class PulseEnum;


/** @brief This module is used to generate Voltage Interruption pulse*/
class VoltageInterruption : public APulse
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(VoltageInterruption)

    public:
        /** @brief Class constructor
            @param parent The parent instance class */
        Q_INVOKABLE explicit VoltageInterruption(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~VoltageInterruption() override;

    private:
        /** @brief flag variable used to define the generic parameters to use in  APulse class */
        inline static QVector<PulseFlag::Flag> _flags = { PulseFlag::VbFlag,
                                                          PulseFlag::T6Flag,
                                                          PulseFlag::InterFlag,
                                                          PulseFlag::NbCyclesFlag,
                                                          PulseFlag::TriggerFlag,
                                                          PulseFlag::CurrentFlag };

    private:
        /** @brief String name of the pulse */
        static const constexpr char *pulseName = "VOLTAGE INTERRUPTION";

    private:
        PulseCmd *_pulseCmd{nullptr};
};
