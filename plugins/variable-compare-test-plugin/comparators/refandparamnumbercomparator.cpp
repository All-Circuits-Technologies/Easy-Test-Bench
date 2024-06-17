// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "refandparamnumbercomparator.hpp"

#include "testbedcore/parsers/bankjsonformatter.hpp"


RefAndParamNumberComparator::RefAndParamNumberComparator(QObject *parent) :
    ARefAndParamComparator<Number>(staticMetaObject.className(),
                                   parent)
{
}

bool RefAndParamNumberComparator::initParamsFormat(JsonArray &paramsFormat) const
{
    RETURN_IF_FALSE(ARefAndParamComparator<Number>::initParamsFormat(paramsFormat));

    JsonObject valueObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(valueParamName,
                                                                 valueParamDesc,
                                                                 Number::MaxAllowedCommaShift,
                                                                 Number::MaxAllowedCommaShift,
                                                                 valueObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueNumber(Number(1), valueObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, valueObj));

    return true;
}
