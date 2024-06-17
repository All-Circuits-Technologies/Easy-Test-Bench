// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "modules/readandwrite/acanwriting.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief This module sends a CAN message and may expect an answer. */
class WriteCanMsg : public ACanWriting
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(WriteCanMsg)

    public:
        /** @brief Class constructor
            @param parent The parent instance class */
        Q_INVOKABLE explicit WriteCanMsg(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~WriteCanMsg() override;
};
