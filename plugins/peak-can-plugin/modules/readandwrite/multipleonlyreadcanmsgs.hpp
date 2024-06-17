// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "modules/readandwrite/acanmsgsequence.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief This module waits for expected CAN messages. */
class MultipleOnlyReadCanMsgs : public ACanMsgSequence
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(MultipleOnlyReadCanMsgs)

    public:
    /** @brief Class constructor
            @param parent The parent instance class */
        Q_INVOKABLE explicit MultipleOnlyReadCanMsgs(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~MultipleOnlyReadCanMsgs() override;
};
