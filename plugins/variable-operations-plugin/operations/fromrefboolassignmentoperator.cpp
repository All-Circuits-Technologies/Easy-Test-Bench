// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "fromrefboolassignmentoperator.hpp"

#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/types/typemanaged.hpp"


FromRefBoolAssignmentOperator::FromRefBoolAssignmentOperator(QObject *parent) :
    AFromRefOperator(staticMetaObject.className(), TypeManaged::Boolean, parent)
{
}

bool FromRefBoolAssignmentOperator::initParamsFormat(JsonArray &paramsFormat) const
{
    RETURN_IF_FALSE(AFromRefOperator::initParamsFormat(paramsFormat));

    JsonObject assignObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamSelectRequired(assingParamName,
                                                                 assignParamDesc,
                                                                 assignObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamAddSelectOption(optionSameKey,
                                                                  optionSameValue,
                                                                  assignObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamAddSelectOption(optionNotKey,
                                                                  optionNotValue,
                                                                  assignObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultOption({ optionDefault }, assignObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, assignObj));

    return true;
}

bool FromRefBoolAssignmentOperator::operate(const JsonArray &inputValues, JsonArray &outputValues)
{
    bool value;

    RETURN_IF_FALSE(BankJsonParser::getBoolValue(inputValues, inputPinName, value));

    value = _sameValue ? value : !value;

    RETURN_IF_FALSE(BankJsonParser::setBoolValue(outputPinName, value, outputValues));

    return true;
}

bool FromRefBoolAssignmentOperator::validateAndSetParameters(const JsonArray &parameters,
                                                             JsonArray &errors)
{
    RETURN_IF_FALSE(AFromRefOperator::validateAndSetParameters(parameters, errors));

    QString errorStr;

    QVector<QString> paramValues;
    if(!BankJsonParser::getArrayStringValue(parameters, assingParamName, paramValues, &errorStr))
    {
        fillErrorArray(assingParamName, errorStr, errors);
        return false;
    }

    if(paramValues.length() != 1)
    {
        fillErrorArray(assingParamName,
                       tr("(TR) One select value has to be chosen (only one)"),
                       errors);
        return false;
    }

    QString paramValue = paramValues.first();

    if(paramValue == optionSameKey )
    {
        _sameValue = true;
    }
    else if(paramValue == optionNotKey)
    {
        _sameValue = false;
    }
    else
    {
        fillErrorArray(assingParamName,
                       tr("(TR) A select value has to be chosen, unknown key: %1").arg(paramValue),
                       errors);
        return false;
    }

    return true;
}
