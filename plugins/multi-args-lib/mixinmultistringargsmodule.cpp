// SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "mixinmultistringargsmodule.hpp"

#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"

#include "abstractpluginelements/defaultsequenceinstancemodule.hpp"


MixinMultiStringArgsModule::MixinMultiStringArgsModule(int argumentsNb) :
    _argumentsNb{argumentsNb}
{
}

MixinMultiStringArgsModule::~MixinMultiStringArgsModule()
{
}

bool MixinMultiStringArgsModule::initArgsInputsFormat(JsonArray &inputsFormat) const
{
    for(int idx = 1; idx <= _argumentsNb; ++idx)
    {
        JsonObject inputArgument;
        RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(
            QString(InputArgumentName).arg(idx),
            inputArgument,
            QString(InputArgumentDesc).arg(idx)));

        inputsFormat.append(inputArgument);
    }

    return true;
}

bool MixinMultiStringArgsModule::initArgsParamsFormat(JsonArray &paramsFormat) const
{
    for(int idx = 1; idx <= _argumentsNb; ++idx)
    {
        JsonObject argumentObj;

        RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(
            QString(ArgumentKeyParamName).arg(idx),
            QString(ArgumentKeyParamDesc).arg(idx),
            argumentObj));

        RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, argumentObj));

        paramsFormat.append(argumentObj);
    }

    return true;
}

bool MixinMultiStringArgsModule::validateAndSetArgsParameters(const JsonArray &parameters,
                                                              JsonArray &errors)
{
    QString error;

    for(int idx = 1; idx <= _argumentsNb; ++idx)
    {
        const QString paramName = QString(ArgumentKeyParamName).arg(idx);

        QString argumentKey;
        if(!BankJsonParser::getStringValue(parameters,
                                           paramName,
                                           argumentKey,
                                           &error))
        {
            DefaultSequenceInstanceModule::fillErrorArray(paramName, error, errors);
            return false;
        }

        _argumentsKeys.append(argumentKey);
    }

    return true;
}

bool MixinMultiStringArgsModule::extractArgumentsFromInputs(const JsonArray &jsonObjectInputValues,
                                                            QVector<QString> &inputValues) const
{
    for(int idx = 1; idx <= _argumentsNb; ++idx)
    {
        const QString inputName = QString(InputArgumentName).arg(idx);

        QString argumentValue;
        RETURN_IF_FALSE(BankJsonParser::getStringValue(jsonObjectInputValues,
                                                       inputName,
                                                       argumentValue));

        inputValues.append(argumentValue);
    }

    return true;
}

bool MixinMultiStringArgsModule::replaceArgumentsInString(const QVector<QString> &inputValues,
                                                          QString &textToReplaceIn)
{
    if(Q_UNLIKELY(inputValues.length() < _argumentsNb))
    {
        qWarning() << "The input values number of multistring can't be inferior of the arguments "
                   << "number";
        return false;
    }

    for(int idx = 0; idx < _argumentsNb; ++idx)
    {
        // Here, we substitute the argument keys in the text with the variables got from inputs
        textToReplaceIn.replace(_argumentsKeys.at(idx), inputValues.at(idx));
    }

    return true;
}

bool MixinMultiStringArgsModule::replaceArgumentsInString(const JsonArray &jsonObjectInputValues,
                                                          QString &textToReplaceIn)
{
    QVector<QString> inputValues;
    RETURN_IF_FALSE(extractArgumentsFromInputs(jsonObjectInputValues, inputValues));

    return replaceArgumentsInString(inputValues, textToReplaceIn);
}
