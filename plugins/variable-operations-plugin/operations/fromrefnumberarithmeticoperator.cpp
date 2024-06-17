// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "fromrefnumberarithmeticoperator.hpp"

#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"


FromRefNumberArithmeticOperator::FromRefNumberArithmeticOperator(QObject *parent) :
    FromRefNumberAssignmentOperator(staticMetaObject.className(), parent)
{
}

FromRefNumberArithmeticOperator::FromRefNumberArithmeticOperator(const QString &moduleClassName,
                                                                 QObject *parent) :
    FromRefNumberAssignmentOperator(moduleClassName, parent)
{
}

bool FromRefNumberArithmeticOperator::operate(const JsonArray &inputValues, JsonArray &outputValues)
{
    Number inputValue;

    RETURN_IF_FALSE(BankJsonParser::getNumberValue(inputValues,
                                                   inputPinName,
                                                   inputValue));

    return arithmeticOperation(inputValue, _valueToOperateWith, outputValues);
}

bool FromRefNumberArithmeticOperator::arithmeticOperation(const Number &basicNumber,
                                                          const Number &otherNumber,
                                                          JsonArray &outputValues)
{
    Number result;

    if(_arithmOperator == Unknown)
    {
        qWarning() << "The arithm operator is unknown, don't know what to do";
        return false;
    }

    if(!basicNumber.isValid() || !otherNumber.isValid())
    {
        qWarning() << "Can't operate with invalid elements";
        return false;
    }

    if(_arithmOperator == Addition)
    {
        result = basicNumber + otherNumber;
    }
    else if(_arithmOperator == Substraction)
    {
        result = basicNumber - otherNumber;
    }
    else if(_arithmOperator == Multiply)
    {
        result = basicNumber *otherNumber;
    }
    else if(_arithmOperator == Divide)
    {
        result = basicNumber / otherNumber;
    }

    if (!result.isValid())
    {
        qWarning() << "The result of the operation is not valid";
        return false;
    }

    RETURN_IF_FALSE(BankJsonParser::setNumberValue(outputPinName, result, outputValues));

    return true;
}

bool FromRefNumberArithmeticOperator::validateAndSetParameters(const JsonArray &parameters,
                                                               JsonArray &errors)
{
    RETURN_IF_FALSE(sharedValidateAndSetParameters(parameters, errors));

    QString errorStr;

    if(!BankJsonParser::getNumberValue(parameters,
                                       valueParamName,
                                       _valueToOperateWith,
                                       &errorStr))
    {
        fillErrorArray(valueParamName, errorStr, errors);
        return false;
    }

    if(!_valueToOperateWith.isValid())
    {
        fillErrorArray(valueParamName,
                       tr("(TR) The number value given is unknown, key linked: %1")
                                                                        .arg(valueParamName),
                       errors);
        return false;
    }

    return true;
}

bool FromRefNumberArithmeticOperator::sharedValidateAndSetParameters(const JsonArray &parameters,
                                                                     JsonArray &errors)
{
    // We don't use the above class method because it adds a selector which don't need in this class
    //NOLINTNEXTLINE(bugprone-parent-virtual-call)
    RETURN_IF_FALSE(AFromRefOperator::validateAndSetParameters(parameters, errors)); // clazy:exclude=skipped-base-method

    QVector<QString> paramValues;
    QString errorStr;

    if(!BankJsonParser::getArrayStringValue(parameters,
                                            operatorParamName,
                                            paramValues,
                                            &errorStr))
    {
        fillErrorArray(operatorParamName, errorStr, errors);
        return false;
    }

    if(paramValues.length() != 1)
    {
        fillErrorArray(operatorParamName,
                       tr("(TR) One select value has to be chosen (only one)"),
                       errors);
        return false;
    }

    QString paramValue = paramValues.first();

    if(paramValue == optionAddKey)
    {
        _arithmOperator = Addition;
    }
    else if(paramValue == optionSubKey)
    {
        _arithmOperator = Substraction;
    }
    else if(paramValue == optionMultKey)
    {
        _arithmOperator = Multiply;
    }
    else if(paramValue == optionDivKey)
    {
        _arithmOperator = Divide;
    }
    else
    {
        fillErrorArray(operatorParamName,
                       tr("(TR) A select value has to be chosen, unknown key: %1").arg(paramValue),
                       errors);
        return false;
    }

    return true;
}

bool FromRefNumberArithmeticOperator::initParamsFormat(JsonArray &paramsFormat) const
{
    RETURN_IF_FALSE(sharedParamsFormat(paramsFormat));

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

bool FromRefNumberArithmeticOperator::sharedParamsFormat(JsonArray &paramsFormat) const
{
    // We don't use the above class method because it adds a selector which don't need in this class
    //NOLINTNEXTLINE(bugprone-parent-virtual-call)
    RETURN_IF_FALSE(AFromRefOperator::initParamsFormat(paramsFormat));  // clazy:exclude=skipped-base-method

    JsonObject operatorObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamSelectRequired(operatorParamName,
                                                                 operatorParamDesc,
                                                                 operatorObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamAddSelectOption(optionAddKey,
                                                                  optionAddValue,
                                                                  operatorObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamAddSelectOption(optionSubKey,
                                                                  optionSubValue,
                                                                  operatorObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamAddSelectOption(optionMultKey,
                                                                  optionMultValue,
                                                                  operatorObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamAddSelectOption(optionDivKey,
                                                                  optionDivValue,
                                                                  operatorObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultOption({ optionDefault }, operatorObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, operatorObj));

    paramsFormat.append(operatorObj);

    return true;
}
