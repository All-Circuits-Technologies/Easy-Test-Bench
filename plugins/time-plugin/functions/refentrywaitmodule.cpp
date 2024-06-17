// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "refentrywaitmodule.hpp"

#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "numberutility/number.hpp"


RefEntryWaitModule::RefEntryWaitModule(QObject *parent) :
    AWaitModule(staticMetaObject.className(), parent)
{
}

bool RefEntryWaitModule::initInputsFormat(JsonArray &inputsFormat) const
{
    JsonObject input;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutNumber(inputTimeToWaitInMsName,
                                                         input,
                                                         inputTimeToWaitInMsDesc));

    inputsFormat.append(input);

    return true;
}

bool RefEntryWaitModule::processWaiting(const JsonArray &jsonObjectInputValues,
                                        Number &timeToWaitInMs)
{
    return BankJsonParser::getNumberValue(jsonObjectInputValues,
                                          inputTimeToWaitInMsName,
                                          timeToWaitInMs);
}
