// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "modules/readandwrite/acanwriting.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief This module sends sequentially multiple CAN messages and may expect answers for each
           one. */
class MultipleWriteCanMsgs : public ACanWriting
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(MultipleWriteCanMsgs)

    public:
        /** @brief Class constructor
            @param parent The parent instance class */
        Q_INVOKABLE explicit MultipleWriteCanMsgs(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~MultipleWriteCanMsgs() override;
};
