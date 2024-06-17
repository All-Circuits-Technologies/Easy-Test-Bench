// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "usbvisasetup.hpp"

#include "qtvisacomlib/src/visacommanager.hpp"
#include "qtvisacomlib/src/visacomusb.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"


UsbVisaSetup::UsbVisaSetup(QObject *parent) :
    AVisaSetup(staticMetaObject.className(), parent)
{
}

bool UsbVisaSetup::initParamsFormat(JsonArray &paramsFormat) const
{
    JsonObject usbNumber;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(usbNumberParamName,
                                                                 usbNumberParamDesc,
                                                                 0,
                                                                 0,
                                                                 usbNumber));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(usbNumber,
                                                                   Number::zero(),
                                                                   Number::maxUint16(),
                                                                   Number(1)));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, usbNumber));

    paramsFormat.append(usbNumber);

    JsonObject vid;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(vendorIdParamName,
                                                                 vendorIdParamDesc,
                                                                 0,
                                                                 0,
                                                                 vid));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(vid,
                                                                   Number::zero(),
                                                                   Number::maxUint32(),
                                                                   Number(1)));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, vid));

    paramsFormat.append(vid);

    JsonObject pid;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(productIdParamName,
                                                                 productIdParamDesc,
                                                                 0,
                                                                 0,
                                                                 pid));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(pid,
                                                                   Number::zero(),
                                                                   Number::maxUint32(),
                                                                   Number(1)));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, pid));

    paramsFormat.append(pid);

    JsonObject serialNumber;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(serialNumberParamName,
                                                                 serialNumberParamDesc,
                                                                 serialNumber));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, serialNumber));

    paramsFormat.append(serialNumber);

    JsonObject itfNumber;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(itfNumberParamName,
                                                                 itfNumberParamDesc,
                                                                 0,
                                                                 0,
                                                                 itfNumber));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(itfNumber,
                                                                   Number::zero(),
                                                                   Number::maxUint16(),
                                                                   Number(1)));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, itfNumber));

    paramsFormat.append(itfNumber);

    return true;
}

bool UsbVisaSetup::validateAndSetParameters(const JsonArray &parameters, JsonArray &errors)
{
    RETURN_IF_FALSE(AVisaSetup::validateAndSetParameters(parameters, errors));

    QString error;

    Number tmpValue;
    if(!BankJsonParser::getNumberValue(parameters, usbNumberParamName, tmpValue, &error))
    {
        fillErrorArray(usbNumberParamName, error, errors);
        return false;
    }

    // In the validate method we already check if the value isn't over the limit
    _usbNumber = tmpValue.toUint16();

    if(!BankJsonParser::getNumberValue(parameters, vendorIdParamName, tmpValue, &error))
    {
        fillErrorArray(vendorIdParamName, error, errors);
        return false;
    }

    // In the validate method we already check if the value isn't over the limit
    _vid = tmpValue.toUint32();

    if(!BankJsonParser::getNumberValue(parameters, productIdParamName, tmpValue, &error))
    {
        fillErrorArray(productIdParamName, error, errors);
        return false;
    }

    // In the validate method we already check if the value isn't over the limit
    _pid = tmpValue.toUint32();

    if(!BankJsonParser::getStringValue(parameters, serialNumberParamName, _serialNumber, &error))
    {
        fillErrorArray(serialNumberParamName, error, errors);
        return false;
    }

    if(!BankJsonParser::getNumberValue(parameters, itfNumberParamName, tmpValue, &error))
    {
        fillErrorArray(itfNumberParamName, error, errors);
        return false;
    }

    // In the validate method we already check if the value isn't over the limit
    _itfNumber = tmpValue.toUint16();

    return true;
}

QSharedPointer<AVisacom> UsbVisaSetup::createAndSetupItf()
{
    return VisacomManager::getInstance().createAndOpenUsb(_usbNumber,
                                                          _vid,
                                                          _pid,
                                                          _serialNumber,
                                                          _itfNumber);
}
