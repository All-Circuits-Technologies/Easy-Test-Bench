// SPDX-FileCopyrightText: 2019 - 2020 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2020 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "seriallinkprobe.hpp"

#include <QSerialPortInfo>

#include "qtseriallinklib/seriallinkmanager.hpp"

#include "testbedcore/logs/sequencelogformatter.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/utility/pauseandstophelper.hpp"

#include "managers/seriallinkbankglobalmanager.hpp"


SerialLinkProbe::SerialLinkProbe(QObject *parent) :
    DefaultSequenceInstanceModule(staticMetaObject.className(),
                                  SerialLinkBankGlobalManager::Instance(),
                                  parent)
{
}

SerialLinkProbe::~SerialLinkProbe()
{
}

void SerialLinkProbe::processModule(const JsonArray &jsonObjectInputValues)
{
    Q_UNUSED(jsonObjectInputValues)

    if(!PauseAndStopHelper::managePauseAndStop(getSequenceState()))
    {
        emit moduleFinished(false, JsonArray());
        return;
    }

    /* search */
    QVector<QSerialPortInfo> portsInfo = SerialLinkManager::findAllSerialPort(_name,
                                                                              _serialNumber,
                                                                              _usbVid,
                                                                              _usbPid,
                                                                              _description,
                                                                              _manufacturer,
                                                                              _location);
    QSerialPortInfo portInfo;

    if(!portsInfo.isEmpty() && portsInfo.length() > _nthElement)
    {
        portInfo = portsInfo.at(_nthElement);
    }

    const bool found = !portInfo.isNull();

    JsonArray outputValues;
    if(!updateOutput(portInfo, found, outputValues))
    {
        qWarning() << "A problem occured when tried to set the ASerialLinkProbe output";
        emit moduleFinished(false, {});
        return;
    }

    /* Debug traces */
    if(found)
    {
        qDebug() << "Found serial port" << portInfo.portName();
    }
    else if(!_mandatory)
    {
        qInfo() << "Serial port not found";
    }
    else
    {
        emit newLog(SequenceLogFormatter::formatABusinessProblem(tr("(TR) Mandatory serial port "
                                                                    "not found")));
        qWarning() << "Mandatory serial port not found";
    }

    /* Done */
    emit moduleFinished((found || !_mandatory), outputValues);
}

bool SerialLinkProbe::initParamsFormat(JsonArray &paramsFormat) const
{
    JsonObject mandatoryBoolParam;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamBoolRequired(
                                                       SerialPluginLibConstants::Json::Mandatory,
                                                       tr("(TR) Should we die if not found"),
                                                       mandatoryBoolParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueBool(DefaultMandatoryParam,
                                                                   mandatoryBoolParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, mandatoryBoolParam));
    paramsFormat.append(mandatoryBoolParam);


    JsonObject portStringParam;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(
        SerialPluginLibConstants::Json::PortName,
                                                           tr("(TR) Serial port name (ex: COM1)"),
                                                           portStringParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueString("", portStringParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, portStringParam));

    paramsFormat.append(portStringParam);


    JsonObject serialNumberStringParam;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(
        SerialPluginLibConstants::Json::SerialNumber,
                                                       tr("(TR) Serial port SN"),
                                                       serialNumberStringParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueString("", serialNumberStringParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, serialNumberStringParam));

    paramsFormat.append(serialNumberStringParam);


    JsonObject usbVidUint16Param;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(
        SerialPluginLibConstants::Json::UsbVid,
                                                           tr("(TR) Serial port USB Vendor ID"),
                                                           0,
                                                           0,
                                                           usbVidUint16Param));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueNumber(Number(0),
                                                                     usbVidUint16Param));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(usbVidUint16Param,
                                                                   Number(0),
                                                                   Number::maxUint16(),
                                                                   Number(1)));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, usbVidUint16Param));

    paramsFormat.append(usbVidUint16Param);


    JsonObject usbPidUint16Param;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(
        SerialPluginLibConstants::Json::UsbPid,
                                                           tr("(TR) Serial port USB Product ID"),
                                                           0,
                                                           0,
                                                           usbPidUint16Param));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueNumber(Number(0), usbPidUint16Param));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(usbPidUint16Param,
                                                                   Number(0),
                                                                   Number::maxUint16(),
                                                                   Number(1)));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, usbPidUint16Param));

    paramsFormat.append(usbPidUint16Param);


    JsonObject descriptionStringParam;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(
                                                       SerialPluginLibConstants::Json::Description,
                                                       tr("(TR) Serial port description"),
                                                       descriptionStringParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueString("", descriptionStringParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, descriptionStringParam));

    paramsFormat.append(descriptionStringParam);


    JsonObject manufacturerStringParam;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(
        SerialPluginLibConstants::Json::Manufacturer,
                                                       tr("(TR) Serial port manufacturer"),
                                                       manufacturerStringParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueString("", manufacturerStringParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, manufacturerStringParam));

    paramsFormat.append(manufacturerStringParam);


    JsonObject locationStringParam;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(
                                                           SerialPluginLibConstants::Json::Location,
                                                           tr("(TR) Serial port system location"),
                                                           locationStringParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueString("", locationStringParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, locationStringParam));

    paramsFormat.append(locationStringParam);

    JsonObject nthElementParam;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(
        SerialPluginLibConstants::Json::NthElement,
        tr("This is the nth element to take in the list of found elements"),
        0,
        0,
        nthElementParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueNumber(
        Number::fromInt32(SerialPluginLibConstants::Defaults::NthElement),
        nthElementParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(nthElementParam,
                                                                   Number(0),
                                                                   Number::maxUint16(),
                                                                   Number(1)));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, nthElementParam));

    paramsFormat.append(nthElementParam);


    return true;
}

bool SerialLinkProbe::initOutputsFormat(JsonArray &outputsFormat) const
{
    JsonObject foundBoolOutVar;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutBool(SerialPluginLibConstants::Json::Found,
                                                       foundBoolOutVar,
                                                       tr("(TR) Is serial link found")));

    outputsFormat.append(foundBoolOutVar);

    JsonObject portStringOutVar;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(SerialPluginLibConstants::Json::PortName,
                                                         portStringOutVar,
                                                         tr("(TR) Serial port name (ex: COM1)")));

    outputsFormat.append(portStringOutVar);

    JsonObject serialNumberStringOutVar;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(
        SerialPluginLibConstants::Json::SerialNumber,
                                                       serialNumberStringOutVar,
                                                       tr("(TR) Serial port SN")));

    outputsFormat.append(serialNumberStringOutVar);

    JsonObject usbVidUint16OutVar;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutNumber(SerialPluginLibConstants::Json::UsbVid,
                                                         usbVidUint16OutVar,
                                                         tr("(TR) Serial port USB Vendor ID")));

    outputsFormat.append(usbVidUint16OutVar);

    JsonObject usbPidUint16OutVar;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutNumber(SerialPluginLibConstants::Json::UsbPid,
                                                         usbPidUint16OutVar,
                                                         tr("(TR) Serial port USB Product ID")));

    outputsFormat.append(usbPidUint16OutVar);

    JsonObject descriptionStringOutVar;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(
                                                       SerialPluginLibConstants::Json::Description,
                                                       descriptionStringOutVar,
                                                       tr("(TR) Serial port description")));

    outputsFormat.append(descriptionStringOutVar);

    JsonObject manufacturerStringOutVar;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(
        SerialPluginLibConstants::Json::Manufacturer,
                                                       manufacturerStringOutVar,
                                                       tr("(TR) Serial port manufacturer")));

    outputsFormat.append(manufacturerStringOutVar);

    JsonObject locationStringOutVar;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(SerialPluginLibConstants::Json::Location,
                                                         locationStringOutVar,
                                                         tr("(TR) Serial port system location")));

    outputsFormat.append(locationStringOutVar);

    JsonObject nthElementNumberOutVar;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(SerialPluginLibConstants::Json::NthElement,
                                                         nthElementNumberOutVar,
                                                         tr("(TR) Serial port nth element")));

    outputsFormat.append(nthElementNumberOutVar);

    return true;
}

bool SerialLinkProbe::validateAndSetParameters(const JsonArray &parameters, JsonArray &errors)
{
    RETURN_IF_FALSE(DefaultSequenceInstanceModule::validateAndSetParameters(parameters, errors));

    QString parserError;

    /* Fetch parameters (all are optional) */
    _mandatory = DefaultMandatoryParam;
    if(!BankJsonParser::getBoolValue(parameters,
                                     SerialPluginLibConstants::Json::Mandatory,
                                     _mandatory,
                                     &parserError,
                                     true))
    {
        fillErrorArray(SerialPluginLibConstants::Json::Mandatory, parserError, errors);
        return false;
    }

    _name.clear();
    if(!BankJsonParser::getStringValue(parameters,
                                        SerialPluginLibConstants::Json::PortName,
                                       _name,
                                       &parserError,
                                       true))
    {
        fillErrorArray(SerialPluginLibConstants::Json::PortName, parserError, errors);
        return false;
    }

    _serialNumber.clear();
    if(!BankJsonParser::getStringValue(parameters,
                                        SerialPluginLibConstants::Json::SerialNumber,
                                       _serialNumber,
                                       &parserError,
                                       true))
    {
        fillErrorArray(SerialPluginLibConstants::Json::SerialNumber, parserError, errors);
        return false;
    }

    Number usbVidNumber;

    if(!BankJsonParser::getNumberValue(parameters,
                                        SerialPluginLibConstants::Json::UsbVid,
                                       usbVidNumber,
                                       &parserError,
                                       true))
    {
        fillErrorArray(SerialPluginLibConstants::Json::UsbVid, parserError, errors);
        return false;
    }

    // Validity and boundaries have already been tested before
    if(usbVidNumber.isValid())
    {
        _usbVid = usbVidNumber.toUint16();
    }
    else
    {
        _usbVid = DefaultUsbVid;
    }

    Number usbPidNumber;

    if(!BankJsonParser::getNumberValue(parameters,
                                        SerialPluginLibConstants::Json::UsbPid,
                                       usbPidNumber,
                                       &parserError,
                                       true))
    {
        fillErrorArray(SerialPluginLibConstants::Json::UsbPid, parserError, errors);
        return false;
    }

    if(usbPidNumber.isValid())
    {
        _usbPid = usbPidNumber.toUint16();
    }
    else
    {
        _usbPid = DefaultUsbPid;
    }

    _description.clear();
    if(!BankJsonParser::getStringValue(parameters,
                                       SerialPluginLibConstants::Json::Description,
                                       _description,
                                       &parserError,
                                       true))
    {
        fillErrorArray(SerialPluginLibConstants::Json::Description, parserError, errors);
        return false;
    }

    _manufacturer.clear();
    if(!BankJsonParser::getStringValue(parameters,
                                        SerialPluginLibConstants::Json::Manufacturer,
                                       _manufacturer,
                                       &parserError,
                                       true))
    {
        fillErrorArray(SerialPluginLibConstants::Json::Manufacturer, parserError, errors);
        return false;
    }

    _location.clear();
    if(!BankJsonParser::getStringValue(parameters,
                                       SerialPluginLibConstants::Json::Location,
                                       _location,
                                       &parserError,
                                       true))
    {
        fillErrorArray(SerialPluginLibConstants::Json::Location, parserError, errors);
        return false;
    }

    Number nthElementNumber;

    if(!BankJsonParser::getNumberValue(parameters,
                                       SerialPluginLibConstants::Json::NthElement,
                                       nthElementNumber,
                                       &parserError,
                                       true))
    {
        fillErrorArray(SerialPluginLibConstants::Json::NthElement, parserError, errors);
        return false;
    }

    if(nthElementNumber.isValid())
    {
        _nthElement = nthElementNumber.toUint16();
    }
    else
    {
        _nthElement = SerialPluginLibConstants::Defaults::NthElement;
    }

    return true;
}

bool SerialLinkProbe::updateOutput(const QSerialPortInfo &portInfo, bool found, JsonArray &output)
{
    RETURN_IF_FALSE(BankJsonParser::setBoolValue(SerialPluginLibConstants::Json::Found,
                                                 found,
                                                 output));
    
    RETURN_IF_FALSE(BankJsonParser::setStringValue(SerialPluginLibConstants::Json::PortName,
                                                   portInfo.portName(),
                                                   output));
    
    RETURN_IF_FALSE(BankJsonParser::setStringValue(SerialPluginLibConstants::Json::SerialNumber,
                                                   portInfo.serialNumber(),
                                                   output));
    
    RETURN_IF_FALSE(BankJsonParser::setNumberValue(SerialPluginLibConstants::Json::UsbVid,
                                                   Number::fromUInt16(portInfo.vendorIdentifier()),
                                                   output));
    
    RETURN_IF_FALSE(BankJsonParser::setNumberValue(SerialPluginLibConstants::Json::UsbPid,
                                                   Number::fromUInt16(portInfo.productIdentifier()),
                                                   output));

    RETURN_IF_FALSE(BankJsonParser::setStringValue(SerialPluginLibConstants::Json::Description,
                                                   portInfo.description(),
                                                   output));
    
    RETURN_IF_FALSE(BankJsonParser::setStringValue(SerialPluginLibConstants::Json::Manufacturer,
                                                   portInfo.manufacturer(),
                                                   output));

    RETURN_IF_FALSE(BankJsonParser::setStringValue(SerialPluginLibConstants::Json::Location,
                                                   portInfo.systemLocation(),
                                                   output));

    RETURN_IF_FALSE(BankJsonParser::setNumberValue(SerialPluginLibConstants::Json::NthElement,
                                                   Number::fromUInt16(_nthElement),
                                                   output));

    return true;
}
