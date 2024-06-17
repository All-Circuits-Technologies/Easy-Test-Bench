// SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "oneargscriptcallviaseriallink.hpp"

#include "managers/seriallinkbankglobalmanager.hpp"


OneArgScriptCallViaSerialLink::OneArgScriptCallViaSerialLink(QObject *parent)
    : ACallScriptViaSerialLink{staticMetaObject.className(),
                               SerialLinkBankGlobalManager::Instance(),
                               parent},
    MixinMultiStringArgsModule{1}
{
}

OneArgScriptCallViaSerialLink::~OneArgScriptCallViaSerialLink()
{
}

bool OneArgScriptCallViaSerialLink::initInputsFormat(JsonArray &inputsFormat) const
{
    RETURN_IF_FALSE(ACallScriptViaSerialLink::initInputsFormat(inputsFormat));

    return initArgsInputsFormat(inputsFormat);
}

bool OneArgScriptCallViaSerialLink::initParamsFormat(JsonArray &paramsFormat) const
{
    RETURN_IF_FALSE(ACallScriptViaSerialLink::initParamsFormat(paramsFormat));

    return initArgsParamsFormat(paramsFormat);
}

bool OneArgScriptCallViaSerialLink::validateAndSetParameters(const JsonArray &parameters,
                                                             JsonArray &errors)
{
    RETURN_IF_FALSE(ACallScriptViaSerialLink::validateAndSetParameters(parameters, errors));

    return validateAndSetArgsParameters(parameters, errors);
}

bool OneArgScriptCallViaSerialLink::manageScriptToCall(const JsonArray &jsonObjectInputValues,
                                                       const QString &scriptToCall,
                                                       QString &managedScriptToCall)
{
    managedScriptToCall = scriptToCall;
    return replaceArgumentsInString(jsonObjectInputValues, managedScriptToCall);
}
