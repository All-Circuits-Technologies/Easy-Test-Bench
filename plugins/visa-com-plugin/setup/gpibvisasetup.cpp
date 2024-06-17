// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "gpibvisasetup.hpp"

#include "qtvisacomlib/src/visacommanager.hpp"
#include "qtvisacomlib/src/visacomgpib.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"


GpibVisaSetup::GpibVisaSetup(QObject *parent) :
    AVisaSetup(staticMetaObject.className(), parent)
{
}

bool GpibVisaSetup::initParamsFormat(JsonArray &paramsFormat) const
{
    JsonObject gpibNumber;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(gpibNumberParamName,
                                                                 gpibNumberParamDesc,
                                                                 0,
                                                                 0,
                                                                 gpibNumber));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(gpibNumber,
                                                                   Number::zero(),
                                                                   Number::maxUint16(),
                                                                   Number(1)));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, gpibNumber));

    paramsFormat.append(gpibNumber);

    JsonObject address;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(gpibAddressParamName,
                                                                 gpibAddressParamDesc,
                                                                 0,
                                                                 0,
                                                                 address));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(address,
                                                                   Number::zero(),
                                                                   Number(30),
                                                                   Number(1)));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, address));

    paramsFormat.append(address);

    return true;
}

bool GpibVisaSetup::validateAndSetParameters(const JsonArray &parameters, JsonArray &errors)
{
    RETURN_IF_FALSE(AVisaSetup::validateAndSetParameters(parameters, errors));

    QString error;

    Number tmpNbrValue;
    if(!BankJsonParser::getNumberValue(parameters, gpibNumberParamName, tmpNbrValue, &error))
    {
        fillErrorArray(gpibNumberParamName, error, errors);
        return false;
    }

    // In the validate method we already check if the value isn't over the limit
    _gpibNumber = tmpNbrValue.toUint16();

    if(!BankJsonParser::getNumberValue(parameters, gpibAddressParamName, tmpNbrValue, &error))
    {
        fillErrorArray(gpibAddressParamName, error, errors);
        return false;
    }

    // In the validate method we already check if the value isn't over the limit
    _address = tmpNbrValue.toUint16();

    return true;
}

QSharedPointer<AVisacom> GpibVisaSetup::createAndSetupItf()
{
    return VisacomManager::getInstance().createAndOpenGpib(_gpibNumber, _address);
}

quint16 GpibVisaSetup::getGpibNumber()
{
    return _gpibNumber;
}

quint16 GpibVisaSetup::getGpibAddress()
{
    return _address;
}
