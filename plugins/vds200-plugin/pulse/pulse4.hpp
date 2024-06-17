// SPDX-FileCopyrightText: 2021 - 2022 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "apulse.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"
#include "pulseflag.hpp"

class PulseCmd;
class PulseEnum;


/** @brief This module is used to generate Pulse 4 (ISO)/(GM)*/
class Pulse4 : public APulse
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param moduleClassName The module class name
            @param parent The parent instance class */
        explicit Pulse4(const QString &moduleClassName,
                        const QString &pulseName,
                        QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~Pulse4() override;

    private:
        /** @brief flag variable used to define the generic parameters to use in  APulse class */
        inline static QVector<PulseFlag::Flag> _flag = { PulseFlag::VbFlag,
                                                         PulseFlag::VsFlag,
                                                         PulseFlag::VaFlag,
                                                         PulseFlag::T6Flag,
                                                         PulseFlag::T7Flag,
                                                         PulseFlag::TfFlag,
                                                         PulseFlag::T8Flag,
                                                         PulseFlag::InterFlag,
                                                         PulseFlag::NbCyclesFlag,
                                                         PulseFlag::TriggerFlag,
                                                         PulseFlag::CurrentFlag};

    private:
        PulseCmd *_pulseCmd{nullptr};
};
