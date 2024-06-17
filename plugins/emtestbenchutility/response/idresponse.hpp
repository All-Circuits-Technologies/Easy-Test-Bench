// SPDX-FileCopyrightText: 2021 - 2022 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "aresponse.hpp"
#include "instrumentid.h"


/** @brief This class is used to manage idResponse coming from EM_Test instruments*/
class IdResponse : public AResponse
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param response The id response from instruments
            @param parent The parent instance class */
        explicit IdResponse(const QString &response, QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~IdResponse() override;

    public:
        /** @brief Parse response from string received
            @param response The response received to parse
            @return True if no problem occurred */
        virtual bool parseResponse(const QString &response) override;

        /** @brief Get instrument id */
        const InstrumentId::Enum &getIdResponseEnum() const { return _idResponseEnum; };

    private:
        /** @brief Index in a string response of the instrument part */
        static const constexpr int instrumentIdx = 0;

    private:
        InstrumentId::Enum _idResponseEnum;
};
