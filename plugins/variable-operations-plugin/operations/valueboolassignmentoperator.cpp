// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "valueboolassignmentoperator.hpp"

#include "testbedcore/parsers/bankjsonformatter.hpp"


ValueBoolAssignmentOperator::ValueBoolAssignmentOperator(QObject *parent) :
    AValueAssignmentOperator<bool>(staticMetaObject.className(),
                                   parent)
{
}

bool ValueBoolAssignmentOperator::initParamsFormat(JsonArray &paramsFormat) const
{
    RETURN_IF_FALSE(AValueAssignmentOperator<bool>::initParamsFormat(paramsFormat));

    JsonObject valueObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamBoolRequired(valueParamName,
                                                               valueParamDesc,
                                                               valueObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueBool(false, valueObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, valueObj));

    paramsFormat.append(valueObj);

    return true;
}
