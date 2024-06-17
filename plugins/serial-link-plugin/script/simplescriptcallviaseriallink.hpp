// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "serial-link-lib/acallscriptviaseriallink.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief This module lets the user calls a script through a serial link */
class SimpleScriptCallViaSerialLink : public ACallScriptViaSerialLink
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(SimpleScriptCallViaSerialLink)

    public:
        /** @brief Class constructor
            @param parent The parent instance */
        Q_INVOKABLE explicit SimpleScriptCallViaSerialLink(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~SimpleScriptCallViaSerialLink() override;
};
