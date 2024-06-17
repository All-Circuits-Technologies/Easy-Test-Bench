// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "fromrefstringassignmentoperator.hpp"

#include <QDebug>

#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/types/typemanaged.hpp"


FromRefStringAssignmentOperator::FromRefStringAssignmentOperator(QObject *parent) :
    AFromRefOperator(staticMetaObject.className(), TypeManaged::String, parent)
{
}

bool FromRefStringAssignmentOperator::operate(const JsonArray &inputValues, JsonArray &outputValues)
{
    QString inputValue;
    RETURN_IF_FALSE(BankJsonParser::getStringValue(inputValues, inputPinName, inputValue));

    RETURN_IF_FALSE(BankJsonParser::setStringValue(outputPinName, inputValue, outputValues));

    return true;
}
