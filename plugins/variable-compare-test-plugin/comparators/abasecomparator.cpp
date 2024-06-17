// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "abasecomparator.hpp"

#include "abstractpluginelements/defaultbankglobalmanager.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/testbedglobal.hpp"
#include "testbedcore/utility/pauseandstophelper.hpp"


ABaseComparator::ABaseComparator(const QString &moduleClassName,
                                 TypeManaged::Enum typeManaged,
                                 QObject *parent) :
    DefaultSequenceInstanceModule(moduleClassName,
                                  DefaultBankGlobalManager::Instance(),
                                  parent),
    _typeManaged(typeManaged)
{
}

bool ABaseComparator::initParamsFormat(JsonArray &paramsFormat) const
{
    JsonObject emitErrorIfFail;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamBoolRequired(emitErrorIfFailParamName,
                                                               emitErrorIfFailParamDesc,
                                                               emitErrorIfFail));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueBool(true, emitErrorIfFail));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, emitErrorIfFail));

    paramsFormat.append(emitErrorIfFail);

    JsonObject comparisonType;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamSelectRequired(comparisonOperatorParamName,
                                                                 comparisonOperatorParamDesc,
                                                                 comparisonType));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamAddSelectOption(optionEqualKey,
                                                                  optionEqualValue,
                                                                  comparisonType));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamAddSelectOption(optionNotEqualKey,
                                                                  optionNotEqualValue,
                                                                  comparisonType));

    if(isManagingExtendedComparisons(_typeManaged))
    {
        RETURN_IF_FALSE(BankJsonFormatter::formatParamAddSelectOption(optionLessKey,
                                                                      optionLessValue,
                                                                      comparisonType));

        RETURN_IF_FALSE(BankJsonFormatter::formatParamAddSelectOption(optionLessOrEqualKey,
                                                                      optionLessOrEqualValue,
                                                                      comparisonType));

        RETURN_IF_FALSE(BankJsonFormatter::formatParamAddSelectOption(optionGreaterKey,
                                                                      optionGreaterValue,
                                                                      comparisonType));

        RETURN_IF_FALSE(BankJsonFormatter::formatParamAddSelectOption(optionGreaterOrEqualKey,
                                                                      optionGreaterOrEqualValue,
                                                                      comparisonType));
    }


    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultOption({ optionDefault }, comparisonType));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, comparisonType));

    paramsFormat.append(comparisonType);

    return true;
}

bool ABaseComparator::initInputsFormat(JsonArray &inputsFormat) const
{
    return addInputFormat(inputPinName, inputPinDesc, _typeManaged, inputsFormat);
}

bool ABaseComparator::initOutputsFormat(JsonArray &outputsFormat) const
{
    JsonObject successObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutBool(outputSuccessPinName,
                                                       successObj,
                                                       outputSuccessPinDesc));

    outputsFormat.append(successObj);

    JsonObject errorMsgObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(outputErrorMsgPinName,
                                                         errorMsgObj,
                                                         outputErrorMsgPinDesc));

    outputsFormat.append(errorMsgObj);

    return true;
}

bool ABaseComparator::validateAndSetParameters(const JsonArray &parameters, JsonArray &errors)
{
    RETURN_IF_FALSE(DefaultSequenceInstanceModule::validateAndSetParameters(parameters, errors));

    QString errorStr;

    QVector<QString> paramValues;
    if(!BankJsonParser::getArrayStringValue(parameters,
                                            comparisonOperatorParamName,
                                            paramValues,
                                            &errorStr))
    {
        fillErrorArray(comparisonOperatorParamName, errorStr, errors);
        return false;
    }

    if(paramValues.length() != 1)
    {
        fillErrorArray(comparisonOperatorParamName,
                       tr("(TR) One select value has to be chosen (only one)"),
                       errors);
        qWarning() << "One select value has to be chosen (only one)";
        return false;
    }

    bool managingExtendedComparison = isManagingExtendedComparisons(_typeManaged);

    QString paramValue = paramValues.first();

    if(paramValue == optionEqualKey)
    {
        _comparisonOperator = Equal;
    }
    else if(paramValue == optionNotEqualKey)
    {
        _comparisonOperator = NotEqual;
    }
    else if(paramValue == optionLessKey && managingExtendedComparison)
    {
        _comparisonOperator = LesserThan;
    }
    else if(paramValue == optionLessOrEqualKey && managingExtendedComparison)
    {
        _comparisonOperator = LesserThanOrEqualTo;
    }
    else if(paramValue == optionGreaterKey && managingExtendedComparison)
    {
        _comparisonOperator = GreaterThan;
    }
    else if(paramValue == optionGreaterOrEqualKey && managingExtendedComparison)
    {
        _comparisonOperator = GreaterThanOrEqualTo;
    }
    else
    {
        fillErrorArray(comparisonOperatorParamName,
                       tr("(TR) A select value has to be chosen, unknown key: %1").arg(paramValue),
                       errors);
        qWarning() << "A select value has to be chosen, unknown key: " << paramValue;
        return false;
    }

    if(!BankJsonParser::getBoolValue(parameters,
                                     emitErrorIfFailParamName,
                                     _emitErrorIfFail,
                                     &errorStr))
    {
        fillErrorArray(emitErrorIfFailParamName, errorStr, errors);
        return false;
    }

    return true;
}

bool ABaseComparator::addInputFormat(const QString &inputName,
                                     const QString &inputDesc,
                                     TypeManaged::Enum managedType,
                                     JsonArray &inputsFormat)
{
    JsonObject inputElement;

    switch(managedType)
    {
        case TypeManaged::Boolean:
            RETURN_IF_FALSE(BankJsonFormatter::formatInOutBool(inputName,
                                                               inputElement,
                                                               inputDesc));
            break;
        case TypeManaged::Number:
            RETURN_IF_FALSE(BankJsonFormatter::formatInOutNumber(inputName,
                                                                 inputElement,
                                                                 inputDesc));
            break;
        case TypeManaged::String:
            RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(inputName,
                                                                 inputElement,
                                                                 inputDesc));
            break;
        case TypeManaged::BooleanList:
            RETURN_IF_FALSE(BankJsonFormatter::formatInOutBoolArray(inputName,
                                                                    inputElement,
                                                                    inputDesc));
            break;
        case TypeManaged::NumberList:
            RETURN_IF_FALSE(BankJsonFormatter::formatInOutNumberArray(inputName,
                                                                      inputElement,
                                                                      inputDesc));
            break;
        case TypeManaged::StringList:
            RETURN_IF_FALSE(BankJsonFormatter::formatInOutStringArray(inputName,
                                                                      inputElement,
                                                                      inputDesc));
            break;
        case TypeManaged::Unknown:
            qWarning() << "The managed type of the input element is unknown";
            return false;
    }

    inputsFormat.append(inputElement);

    return true;
}

void ABaseComparator::processModule(const JsonArray &jsonObjectInputValues)
{
    if(!PauseAndStopHelper::managePauseAndStop(getSequenceState()))
    {
        emit moduleFinished(false, JsonArray());
        return;
    }

    bool result = false;
    QString errorLabel;

    bool success = compare(jsonObjectInputValues, result, errorLabel);

    JsonArray outputValues;

    if(success)
    {
        success = manageOutputVariables(result, errorLabel, outputValues);
    }

    emit moduleFinished(success, outputValues);
}

bool ABaseComparator::manageOutputVariables(bool comparisonResult,
                                            const QString &errorLabel,
                                            JsonArray &outputValues)
{
    RETURN_IF_FALSE(BankJsonParser::setBoolValue(outputSuccessPinName,
                                                 comparisonResult,
                                                 outputValues));

    RETURN_IF_FALSE(BankJsonParser::setStringValue(emitErrorIfFailParamName,
                                                   errorLabel,
                                                   outputValues));

    return (_emitErrorIfFail ? comparisonResult : true);
}

bool ABaseComparator::isManagingExtendedComparisons(TypeManaged::Enum typeManaged)
{
    return (typeManaged == TypeManaged::Number);
}
