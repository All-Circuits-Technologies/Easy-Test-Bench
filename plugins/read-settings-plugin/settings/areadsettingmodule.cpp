// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "areadsettingmodule.hpp"

#include "bankcoreshared/usebyplugin/icommoninterface.hpp"
#include "abstractpluginelements/defaultbankglobalmanager.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/testbedglobal.hpp"


AReadSettingModule::AReadSettingModule(const QString &moduleClassName,
                                       QObject *parent) :
    DefaultSequenceInstanceModule(moduleClassName,
                                  DefaultBankGlobalManager::Instance(),
                                  parent)
{
}

void AReadSettingModule::processModule(const JsonArray &jsonObjectInputValues)
{
    Q_UNUSED(jsonObjectInputValues)

    JsonArray outputs;
    bool success = processReading(outputs);
    
    if(!BankJsonParser::setBoolValue(OutputSuccessName, success, outputs))
    {
        emit moduleFinished(false, JsonArray());
        return;
    }

    if(!_exitIfFail && !success)
    {
        // In that case, we return the result via the output and not the moduleFinished signal
        // So we return a success via the signal, in order to not stop all the sequence
        success = true;
    }

    emit moduleFinished(success, outputs);
}

bool AReadSettingModule::initOutputsFormat(JsonArray &outputsFormat) const
{
    JsonObject success;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutBool(OutputSuccessName,
                                                       success,
                                                       OutputSuccessDesc));
    outputsFormat.append(success);

    return true;
}

bool AReadSettingModule::initParamsFormat(JsonArray &paramsFormat) const
{
    JsonObject exitIfFail;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamBoolRequired(EmitErrorIfFailParamName,
                                                               EmitErrorIfFailParamDesc,
                                                               exitIfFail));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, exitIfFail));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueBool(DefaultEmitErrorIfFailValue,
                                                                   exitIfFail));

    paramsFormat.append(exitIfFail);

    JsonObject settingKeyParam;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(SettingKeyParamName,
                                                                 SettingKeyParamDesc,
                                                                 settingKeyParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, settingKeyParam));

    paramsFormat.append(settingKeyParam);

    return true;
}

bool AReadSettingModule::validateAndSetParameters(const JsonArray &parameters, JsonArray &errors)
{
    RETURN_IF_FALSE(DefaultSequenceInstanceModule::validateAndSetParameters(parameters, errors));

    QString error;
    
    if(!BankJsonParser::getBoolValue(parameters, EmitErrorIfFailParamName, _exitIfFail, &error))
    {
        fillErrorArray(EmitErrorIfFailParamName, error, errors);
        return false;
    }
    
    if(!BankJsonParser::getStringValue(parameters, SettingKeyParamName, _settingKey, &error))
    {
        fillErrorArray(SettingKeyParamName, error, errors);
        return false;
    }

    return true;
}

bool AReadSettingModule::processReading(JsonArray &output)
{
    output.clear();

    QVariant settingValue;
    if(!DefaultBankGlobalManager::Instance()->getCommonInterface().readSetting(_settingKey,
                                                                               settingValue))
    {
        qWarning() << "A problem occured when trying to read the setting: " << _settingKey;
        return false;
    }

    return formatOutputWithReadValue(settingValue, output);
}
