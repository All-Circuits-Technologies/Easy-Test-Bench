// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "gpibvisasetup.hpp"


/** @brief This module is used to setup a VISA communication using a GPIB 488.1 interface */
class Gpib4881VisaSetup : public GpibVisaSetup
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(Gpib4881VisaSetup)

    public:
        /** @brief Class constructor
            @param parent The parent instance class */
        Q_INVOKABLE explicit Gpib4881VisaSetup(QObject *parent = nullptr);

    protected:
        /** @see AVisaSetup::createAndSetupItf */
        virtual QSharedPointer<AVisacom> createAndSetupItf() override;
};
