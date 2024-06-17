// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "cansetup.hpp"

#include "qtpeakcanlib/src/models/candeviceconfigdetails.hpp"
#include "qtpeakcanlib/src/pcanapi/pcanbaudrate.hpp"

#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"


CanSetup::CanSetup(QObject *parent)
    : ACanSetup{staticMetaObject.className(), false, parent}
{
}

CanSetup::~CanSetup()
{
}

bool CanSetup::initParamsFormat(JsonArray &paramsFormat) const
{
    RETURN_IF_FALSE(ACanSetup::initParamsFormat(paramsFormat));

    JsonObject baudRateObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(BaudRateParamName,
                                                                 BaudRateParamDesc,
                                                                 0,
                                                                 0,
                                                                 baudRateObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamUnitNumber(Unit(UnitType::BitPerSecond, 0),
                                                             baudRateObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(baudRateObj,
                                                                   Number::minUint32(),
                                                                   Number::maxUint32()));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, baudRateObj));

    paramsFormat.append(baudRateObj);

    return true;
}

bool CanSetup::validateAndSetParameters(const JsonArray &parameters, JsonArray &errors)
{
    RETURN_IF_FALSE(ACanSetup::validateAndSetParameters(parameters, errors));

    CanDeviceConfigDetails& configDetails = accessDeviceConfig().accessDetails();

    QString error;

    Number baudRate;
    if(!BankJsonParser::getNumberValue(parameters,
                                        BaudRateParamName,
                                        baudRate,
                                        &error))
    {
        fillErrorArray(BaudRateParamName, error, errors);
        return false;
    }

    PCanBaudRate::Enum baudRateType = PCanBaudRate::parseFromRealValue(baudRate);
    if(baudRateType == PCanBaudRate::Unknown)
    {
        fillErrorArray(BaudRateParamName,
                       QString("A problem occurred when tried to parse the baudrate value: %1, "
                               "the value isn't supported by the PeakCanLib. If you want to "
                               "support this value, contact the developpers.")
                           .arg(baudRate.toString()),
                       errors);
        return false;
    }

    configDetails.setBaudRate(baudRateType);

    return true;
}
