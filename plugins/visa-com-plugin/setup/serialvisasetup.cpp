// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "serialvisasetup.hpp"

#include "qtvisacomlib/src/visacommanager.hpp"
#include "qtvisacomlib/src/visacomasrl.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"


SerialVisaSetup::SerialVisaSetup(QObject *parent) :
    AVisaSetup(staticMetaObject.className(), parent)
{
}

bool SerialVisaSetup::initParamsFormat(JsonArray &paramsFormat) const
{
    JsonObject comPort;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(comPortParamName,
                                                                 comPortParamDesc,
                                                                 0,
                                                                 0,
                                                                 comPort));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(comPort,
                                                                   Number::zero(),
                                                                   Number::maxUint16(),
                                                                   Number(1)));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, comPort));

    paramsFormat.append(comPort);

    JsonObject baudrate;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(baudrateParamName,
                                                                 baudrateParamDesc,
                                                                 0,
                                                                 0,
                                                                 baudrate));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(baudrate,
                                                                   Number(minBaudrate),
                                                                   Number(maxBaudrate),
                                                                   Number(1)));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueNumber(Number(defaultBaudrate),
                                                                     baudrate));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, baudrate));

    paramsFormat.append(baudrate);

    JsonObject databits;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(databitsParamName,
                                                                 databitsParamDesc,
                                                                 0,
                                                                 0,
                                                                 databits));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(databits,
                                                                   Number(minDatabits),
                                                                   Number(maxDatabits),
                                                                   Number(1)));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueNumber(Number(maxDatabits),
                                                                     databits));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, databits));

    paramsFormat.append(databits);

    JsonObject flowcontrolObj;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamSelectRequired(flowControlParamName,
                                                                 flowControlParamDesc,
                                                                 flowcontrolObj));

    for(FlowControl::Enum flowcontrol : FlowControl::getAllEnums()){
        QString fctrlTypeStr = FlowControl::toString(flowcontrol);
        RETURN_IF_FALSE(BankJsonFormatter::formatParamAddSelectOption(fctrlTypeStr,
                                                                      fctrlTypeStr,
                                                                      flowcontrolObj));
    }

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, flowcontrolObj));

    paramsFormat.append(flowcontrolObj);

    JsonObject parityObj;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamSelectRequired(parityParamName,
                                                                 parityParamDesc,
                                                                 parityObj));

    for(Parity::Enum parity : Parity::getAllEnums()){
        QString parityTypeStr = Parity::toString(parity);
        RETURN_IF_FALSE(BankJsonFormatter::formatParamAddSelectOption(parityTypeStr,
                                                                      parityTypeStr,
                                                                      parityObj));
    }

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, parityObj));

    paramsFormat.append(parityObj);

    JsonObject stopbitsObj;

    RETURN_IF_FALSE(BankJsonFormatter::formatParamSelectRequired(stopBitsParamName,
                                                                 stopBitsParamDesc,
                                                                 stopbitsObj));

    for(StopBits::Enum stopbits : StopBits::getAllEnums()){
        QString stopbitsTypeStr = StopBits::toString(stopbits);
        RETURN_IF_FALSE(BankJsonFormatter::formatParamAddSelectOption(stopbitsTypeStr,
                                                                      stopbitsTypeStr,
                                                                      stopbitsObj));
    }

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, stopbitsObj));

    paramsFormat.append(stopbitsObj);

    return true;
}

bool SerialVisaSetup::validateAndSetParameters(const JsonArray &parameters, JsonArray &errors)
{
    RETURN_IF_FALSE(AVisaSetup::validateAndSetParameters(parameters, errors));

    QString error;

    Number tmpValue;
    if(!BankJsonParser::getNumberValue(parameters, comPortParamName, tmpValue, &error))
    {
        fillErrorArray(comPortParamName, error, errors);
        return false;
    }

    // In the validate method we already check if the value isn't over the limit
    _comPort = tmpValue.toUint16();

    if(!BankJsonParser::getNumberValue(parameters, baudrateParamName, tmpValue, &error))
    {
        fillErrorArray(baudrateParamName, error, errors);
        return false;
    }

    // In the validate method we already check if the value isn't over the limit
    _baudrate = tmpValue.toUint32();

    if(!BankJsonParser::getNumberValue(parameters, databitsParamName, tmpValue, &error))
    {
        fillErrorArray(databitsParamName, error, errors);
        return false;
    }

    // In the validate method we already check if the value isn't over the limit
    _databits = tmpValue.toUint16();

    QString enumTypeStr;

    RETURN_IF_FALSE(getEnumSelection(parameters, flowControlParamName, errors, enumTypeStr));
    _flowControl = FlowControl::parseFromString(enumTypeStr);

    if(_flowControl == FlowControl::Unknown)
    {
        fillErrorArray(flowControlParamName,
                       "The flowcontrol mode is unknown: " + enumTypeStr,
                       errors);
        return false;
    }

    RETURN_IF_FALSE(getEnumSelection(parameters, parityParamName, errors, enumTypeStr));
    _parity = Parity::parseFromString(enumTypeStr);

    if(_parity == Parity::Unknown)
    {
        fillErrorArray(parityParamName,
                       "The parity mode is unknown: " + enumTypeStr,
                       errors);
        return false;
    }

    RETURN_IF_FALSE(getEnumSelection(parameters, stopBitsParamName, errors, enumTypeStr));
    _stopBits = StopBits::parseFromString(enumTypeStr);

    if(_stopBits == StopBits::Unknown)
    {
        fillErrorArray(stopBitsParamName,
                       "The stopbits mode is unknown: " + enumTypeStr,
                       errors);
        return false;
    }

    return true;
}


bool SerialVisaSetup::getEnumSelection(const JsonArray &parameters,
                                       const char * paramName,
                                       JsonArray &errors,
                                       QString &enumTypeStr)
{
    QString error;
    QVector<QString> enumValue;

    if(!BankJsonParser::getArrayStringValue(parameters,paramName, enumValue, &error))
    {
        fillErrorArray(paramName, error, errors);
        return false;
    }

    if(enumValue.isEmpty())
    {
        fillErrorArray(paramName, "No Enum type selected", errors);
        return false;
    }

    enumTypeStr = enumValue.first();

    return true;
}

QSharedPointer<AVisacom> SerialVisaSetup::createAndSetupItf()
{
    _visacomasrl = VisacomManager::getInstance().createAndOpenAsrl(_comPort);
    _visacomasrl->setBaudrate(_baudrate);
    _visacomasrl->setDataBits(_databits);
    _visacomasrl->setFlowControl(_flowControl);
    _visacomasrl->setParity(_parity);
    _visacomasrl->setStopBits(_stopBits);

    return _visacomasrl;
}
