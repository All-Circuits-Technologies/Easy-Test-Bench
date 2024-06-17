// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "valuestringassignmentoperator.hpp"

#include "testbedcore/parsers/bankjsonformatter.hpp"


ValueStringAssignmentOperator::ValueStringAssignmentOperator(QObject *parent) :
    AValueAssignmentOperator<QString>(staticMetaObject.className(),
                                      parent)
{
}

bool ValueStringAssignmentOperator::initParamsFormat(JsonArray &paramsFormat) const
{
    RETURN_IF_FALSE(AValueAssignmentOperator<QString>::initParamsFormat(paramsFormat));

    JsonObject valueObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(valueParamName,
                                                                 valueParamDesc,
                                                                 valueObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, valueObj));

    paramsFormat.append(valueObj);

    return true;
}
