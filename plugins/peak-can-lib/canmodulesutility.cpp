// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "canmodulesutility.hpp"

#include <QDebug>

#include "testbedcore/parsers/bankjsonparser.hpp"

#include "peakcanlibconstants.hpp"


CanModulesUtility::CanModulesUtility()
{
}

bool CanModulesUtility::extractAndParseCanBusIntf(const JsonArray &jsonObjectInputValues,
                                                  QString &strCanBusIntf,
                                                  PCanBusItf::Enum &pCanBusIntf)
{
    RETURN_IF_FALSE(BankJsonParser::getStringValue(
        jsonObjectInputValues,
        PeakCanLibConstants::ModulesEntries::Inputs::InputPeakCanIntfName,
        strCanBusIntf));

    pCanBusIntf = PCanBusItf::parseFromString(strCanBusIntf);
    if(pCanBusIntf == PCanBusItf::Unknown)
    {
        qWarning() << "The input: "
                   << PeakCanLibConstants::ModulesEntries::Inputs::InputPeakCanIntfName
                   << ", doesn't contain a PCanBusItf: " << strCanBusIntf;
        return false;
    }

    return true;
}
