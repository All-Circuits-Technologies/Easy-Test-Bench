// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "apulsecmd.hpp"


/** @brief This class is helpful to generate pulse command
    @note This class is used by pulse which only use generic parameters */
class PulseCmd : public APulseCmd
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param header The command header symbolising the type of pulse
            @param parent The parent instance class */
        explicit PulseCmd(PulseEnum::Enum header, QObject *parent);

        /** @brief Class destructor */
        virtual ~PulseCmd() override;

    private:
        /** @see ACmd::getParameters */
        virtual bool getParameters(QVector<quint16> &params) override;

    private:
        QVector<quint16> _parameters;
        PulseEnum::Enum _header;
};
