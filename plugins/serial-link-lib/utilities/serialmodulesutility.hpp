// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QString>

#include "jsonutility/jsontypedefs.hpp"


/** @brief Contains useful methods for modules using Serial link */
class SerialModulesUtility
{
    private:
        /** @brief Private class constructor */
        explicit SerialModulesUtility();

    public:
        /** @brief Extract the port name from the module inputs
            @param jsonObjectInputValue The module inputs
            @param portName The port name extracted from the inputs
            @return True if no problem occurred */
        static bool extractSerialLink(const JsonArray &jsonObjectInputValues, QString &portName);
};

