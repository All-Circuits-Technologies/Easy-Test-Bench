// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "modules/readandwrite/acanmsgsequence.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief This module only waits for an expected CAN message. */
class OnlyReadCanMsg : public ACanMsgSequence
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(OnlyReadCanMsg)

    public:
        /** @brief Class constructor
            @param parent The parent instance class */
        Q_INVOKABLE explicit OnlyReadCanMsg(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~OnlyReadCanMsg() override;
};
