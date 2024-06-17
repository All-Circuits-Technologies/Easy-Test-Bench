// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "fromrefnumberassignmentoperator.hpp"

#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/types/typemanaged.hpp"


FromRefNumberAssignmentOperator::FromRefNumberAssignmentOperator(QObject *parent) :
    AFromRefOperator(staticMetaObject.className(), TypeManaged::Number, parent)
{
}

FromRefNumberAssignmentOperator::FromRefNumberAssignmentOperator(const QString &moduleClassName,
                                                                 QObject *parent) :
    AFromRefOperator(moduleClassName, TypeManaged::Number, parent)
{
}

bool FromRefNumberAssignmentOperator::initParamsFormat(JsonArray &paramsFormat) const
{
    RETURN_IF_FALSE(AFromRefOperator::initParamsFormat(paramsFormat));

    JsonObject assignObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamSelectRequired(operatorParamName,
                                                                 operatorParamDesc,
                                                                 assignObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamAddSelectOption(optionOppKey,
                                                                  optionOppValue,
                                                                  assignObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamAddSelectOption(optionAbsKey,
                                                                  optionAbsValue,
                                                                  assignObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamAddSelectOption(optionSameKey,
                                                                  optionSameValue,
                                                                  assignObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultOption({ optionDefault }, assignObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, assignObj));

    return true;
}

bool FromRefNumberAssignmentOperator::operate(const JsonArray &inputValues, JsonArray &outputValues)
{
    Number inputValue;
    Number result;

    RETURN_IF_FALSE(BankJsonParser::getNumberValue(inputValues,
                                                   inputPinName,
                                                   inputValue));

    if(!inputValue.isValid())
    {
        qWarning() << "Can't operate with invalid elements";
        return false;
    }

    if(_numberOperation == Opposite                                ||
            (_numberOperation == Absolute && inputValue.isNegative()))
    {
        result = -inputValue;
    }
    else if ((_numberOperation == Absolute && inputValue.isPositive()) ||
             _numberOperation == Same)
    {
        result = inputValue;
    }

    RETURN_IF_FALSE(BankJsonParser::setNumberValue(outputPinName, result, outputValues));

    return true;
}

bool FromRefNumberAssignmentOperator::validateAndSetParameters(const JsonArray &parameters,
                                                               JsonArray &errors)
{
    RETURN_IF_FALSE(AFromRefOperator::validateAndSetParameters(parameters, errors));

    QString errorStr;

    QVector<QString> paramValues;
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

    if(paramValue == optionOppKey)
    {
        _numberOperation = Opposite;
    }
    else if(paramValue == optionAbsKey)
    {
        _numberOperation = Absolute;
    }
    else if(paramValue == optionSameKey)
    {
        _numberOperation = Same;
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
