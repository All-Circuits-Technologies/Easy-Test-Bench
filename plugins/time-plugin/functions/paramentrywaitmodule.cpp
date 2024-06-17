// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "paramentrywaitmodule.hpp"

#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/types/unit.hpp"


ParamEntryWaitModule::ParamEntryWaitModule(QObject *parent) :
    AWaitModule(staticMetaObject.className(),
                parent)
{
}

bool ParamEntryWaitModule::initParamsFormat(JsonArray &paramsFormat) const
{
    JsonObject timeToWait;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(timeToWaitParamName,
                                                                 timeToWaitParamDesc,
                                                                 Number::MaxAllowedCommaShift,
                                                                 Number::MaxAllowedCommaShift,
                                                                 timeToWait));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamUnitNumber(Unit("s", -3), timeToWait));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueNumber(Number(0), timeToWait));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, timeToWait));

    paramsFormat.append(timeToWait);

    return true;
}

bool ParamEntryWaitModule::validateAndSetParameters(const JsonArray &parameters, JsonArray &errors)
{
    RETURN_IF_FALSE(AWaitModule::validateAndSetParameters(parameters, errors));

    QString error;
    if(!BankJsonParser::getNumberValue(parameters, timeToWaitParamName, _timeToWait, &error))
    {
        fillErrorArray(timeToWaitParamName, error, errors);
        return false;
    }

    return true;
}

bool ParamEntryWaitModule::processWaiting(const JsonArray &jsonObjectInputValues,
                                          Number &timeToWaitInMs)
{
    Q_UNUSED(jsonObjectInputValues)
    timeToWaitInMs = _timeToWait;

    return true;
}
