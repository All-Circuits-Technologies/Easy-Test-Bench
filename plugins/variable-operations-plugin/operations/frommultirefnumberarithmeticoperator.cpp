// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "frommultirefnumberarithmeticoperator.hpp"

#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/types/typemanaged.hpp"


FromMultiRefNumberArithmeticOperator::FromMultiRefNumberArithmeticOperator(QObject *parent) :
    FromRefNumberArithmeticOperator(staticMetaObject.className(),
                                    parent)
{
}

bool FromMultiRefNumberArithmeticOperator::operate(const JsonArray &inputValues,
                                                   JsonArray &outputValues)
{
    Number inputValue;

    RETURN_IF_FALSE(BankJsonParser::getNumberValue(inputValues,
                                                   inputPinName,
                                                   inputValue));

    Number inputSecValue;

    RETURN_IF_FALSE(BankJsonParser::getNumberValue(inputValues,
                                                   inputSecPinName,
                                                   inputSecValue));

    return arithmeticOperation(inputValue, inputSecValue, outputValues);
}

bool FromMultiRefNumberArithmeticOperator::initParamsFormat(JsonArray &paramsFormat) const
{
    return sharedParamsFormat(paramsFormat);
}

bool FromMultiRefNumberArithmeticOperator::validateAndSetParameters(const JsonArray &parameters,
                                                                    JsonArray &errors)
{
    return sharedValidateAndSetParameters(parameters, errors);
}

bool FromMultiRefNumberArithmeticOperator::initInputsFormat(JsonArray &inputsFormat) const
{
    RETURN_IF_FALSE(FromRefNumberArithmeticOperator::initInputsFormat(inputsFormat));

    return addInOutFormat(inputSecPinName, inputSecPinDesc, TypeManaged::Number, inputsFormat);
}
