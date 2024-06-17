// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "serialmodulesutility.hpp"

#include "testbedcore/parsers/bankjsonparser.hpp"

#include "serialpluginlibconstants.hpp"


SerialModulesUtility::SerialModulesUtility()
{
}

bool SerialModulesUtility::extractSerialLink(const JsonArray &jsonObjectInputValues,
                                             QString &portName)
{
    return BankJsonParser::getStringValue(
        jsonObjectInputValues,
        SerialPluginLibConstants::Json::PortName,
        portName);
}
