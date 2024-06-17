// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QString>

#include "qtpeakcanlib/src/pcanapi/pcanbusitf.hpp"

#include "jsonutility/jsontypedefs.hpp"


/** @brief Contains useful shared methods for all the CAN modules */
class CanModulesUtility
{
    private:
        /** @brief Class constructor */
        explicit CanModulesUtility();

    public:
        /** @brief Extract and parse the CAN bus interface from the module input
            @param jsonObjectInputValues The module inputs to parse
            @param strCanBusIntf The bus interface name got
            @param pCanBusIntf The parsed bus interface */
        static bool extractAndParseCanBusIntf(const JsonArray &jsonObjectInputValues,
                                              QString &strCanBusIntf,
                                              PCanBusItf::Enum &pCanBusIntf);
};
