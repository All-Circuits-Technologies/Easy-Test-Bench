// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "valuenumberassignmentoperator.hpp"

#include "testbedcore/parsers/bankjsonformatter.hpp"


ValueNumberAssignmentOperator::ValueNumberAssignmentOperator(QObject *parent) :
    AValueAssignmentOperator<Number>(staticMetaObject.className(),
                                     parent)
{
}

bool ValueNumberAssignmentOperator::initParamsFormat(JsonArray &paramsFormat) const
{
    RETURN_IF_FALSE(AValueAssignmentOperator<Number>::initParamsFormat(paramsFormat));

    JsonObject valueObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(valueParamName,
                                                                 valueParamDesc,
                                                                 Number::MaxAllowedCommaShift,
                                                                 Number::MaxAllowedCommaShift,
                                                                 valueObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueNumber(Number(1), valueObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, valueObj));

    paramsFormat.append(valueObj);

    return true;
}
