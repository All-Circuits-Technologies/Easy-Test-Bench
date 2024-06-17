// SPDX-FileCopyrightText: 2019 - 2020 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2020 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "seriallinksetup.hpp"

#include <QSerialPort>
#include <QSerialPortInfo>

#include "qtseriallinklib/seriallinkintf.hpp"
#include "qtseriallinklib/seriallinkmanager.hpp"

#include "testbedcore/logs/sequencelogformatter.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/utility/pauseandstophelper.hpp"

#include "managers/seriallinkbankglobalmanager.hpp"


SerialLinkSetup::SerialLinkSetup(QObject *parent) :
    DefaultSequenceInstanceModule(staticMetaObject.className(),
                                  SerialLinkBankGlobalManager::Instance(),
                                  parent)
{
}

SerialLinkSetup::~SerialLinkSetup()
{
}

bool SerialLinkSetup::initParamsFormat(JsonArray &paramsFormat) const
{
    JsonObject baudrateInt32Param;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(
        SerialPluginLibConstants::Json::Baudrate,
        tr("(TR) Wanted baudrate (ex: 115200)"),
        0,
        0,
        baudrateInt32Param));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(baudrateInt32Param,
                                                                   Number(1),
                                                                   Number::maxInt32(),
                                                                   Number(1)));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueNumber(
        Number(SerialPluginLibConstants::Defaults::Baudrate),
                                               baudrateInt32Param));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, baudrateInt32Param));

    paramsFormat.append(baudrateInt32Param);


    JsonObject swFlowCtlBoolParam;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamBoolRequired(
        SerialPluginLibConstants::Json::SwFlowCtl,
        tr("(TR) Enable XON/XOFF"),
        swFlowCtlBoolParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueBool(
        SerialPluginLibConstants::Defaults::SwFlowCtl,
                                                   swFlowCtlBoolParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, swFlowCtlBoolParam));

    paramsFormat.append(swFlowCtlBoolParam);

    JsonObject hwFlowCtlBoolParam;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamBoolRequired(
        SerialPluginLibConstants::Json::HwFlowCtl,
        tr("(TR) Enabled CTS/RTS"),
        hwFlowCtlBoolParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueBool(
        SerialPluginLibConstants::Defaults::HwFlowCtl,
                                                   hwFlowCtlBoolParam));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(false, hwFlowCtlBoolParam));

    paramsFormat.append(hwFlowCtlBoolParam);

    return true;
}

bool SerialLinkSetup::initInputsFormat(JsonArray &inputsFormat) const
{
    JsonObject portStringInVar;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(SerialPluginLibConstants::Json::PortName,
                                                         portStringInVar,
                                                         tr("(TR) Serial port name (ex: COM1)")));
    inputsFormat.append(portStringInVar);

    return true;
}

bool SerialLinkSetup::validateAndSetParameters(const JsonArray &parameters, JsonArray &errors)
{
    RETURN_IF_FALSE(DefaultSequenceInstanceModule::validateAndSetParameters(parameters, errors));

    QString parserError;
    Number baudrateNumber;

    /* Fetch parameters */
    if(!BankJsonParser::getNumberValue(parameters,
                                       SerialPluginLibConstants::Json::Baudrate,
                                       baudrateNumber,
                                       &parserError,
                                       true))
    {
        fillErrorArray(SerialPluginLibConstants::Json::Baudrate, parserError, errors);
        return false;
    }

    _baudrate = baudrateNumber.toInt32();

    if(!BankJsonParser::getBoolValue(parameters,
                                     SerialPluginLibConstants::Json::SwFlowCtl,
                                     _swFlowCtl,
                                     &parserError,
                                     true))
    {
        fillErrorArray(SerialPluginLibConstants::Json::SwFlowCtl, parserError, errors);
        return false;
    }

    if(!BankJsonParser::getBoolValue(parameters,
                                     SerialPluginLibConstants::Json::HwFlowCtl,
                                     _hwFlowCtl,
                                     &parserError,
                                     true))
    {
        fillErrorArray(SerialPluginLibConstants::Json::HwFlowCtl, parserError, errors);
        return false;
    }


    if(_swFlowCtl && _hwFlowCtl)
    {
        fillErrorArray(SerialPluginLibConstants::Json::SwFlowCtl,
                       tr("(TR) Software and hardware flow control can not be enabled "
                          "concurrently"),
                       errors);
        return false;
    }

    return true;
}

void SerialLinkSetup::processModule(const JsonArray &jsonObjectInputValues)
{
    if(!PauseAndStopHelper::managePauseAndStop(getSequenceState()))
    {
        emit moduleFinished(false, {});
        return;
    }

    bool success = setupLink(jsonObjectInputValues);

    emit moduleFinished(success, {});
}

bool SerialLinkSetup::setupLink(const JsonArray &jsonObjectInputValues)
{
    QString portName;

    /* Fetch inputs */
    if(!BankJsonParser::getStringValue(jsonObjectInputValues,
                                        SerialPluginLibConstants::Json::PortName,
                                       portName))
    {
        SequenceLogFormatter::formatAnApplicationProblem(tr("(TR) Missing required serial port "
                                                            "name"));
        return false;
    }

    /* Adapts and validate intputs */
    if(portName.isEmpty())
    {
        SequenceLogFormatter::formatAnApplicationProblem(tr("(TR) Serial port name must not be "
                                                            "empty"));
        return false;
    }
    
    QVector<QSerialPortInfo> portsInfo = SerialLinkManager::findAllSerialPort(portName);
    if(portsInfo.isEmpty())
    {
        SequenceLogFormatter::formatABusinessProblem(tr("(TR) Serial port %1 not found on the "
                                                        "system").arg(portName));
        return false;
    }

    QSerialPort::FlowControl flowCtl = QSerialPort::NoFlowControl;
    if(_hwFlowCtl)
    {
        flowCtl = QSerialPort::HardwareControl;
    }
    else if (_swFlowCtl)
    {
        flowCtl = QSerialPort::SoftwareControl;
    }

    SerialLinkManager &serialMngr = SerialLinkManager::getInstance();

    const QSharedPointer<SerialLinkIntf> serialLink = serialMngr.createOrGetSerialLink(
        portsInfo.first());

    if(serialLink.isNull())
    {
        SequenceLogFormatter::formatAnApplicationProblem(tr("(TR) A problem occurred when trying "
                                                            "to create or get the serial port: %1")
                                                                                    .arg(portName));
        return false;
    }

    _serialLink = serialLink;

    if(!_serialLink->setBaudRate(_baudrate, QSerialPort::AllDirections))
    {
        SequenceLogFormatter::formatABusinessProblem(tr("(TR) Serial port %1 rejected baudrate %2")
                                                                        .arg(portName, _baudrate));
        return false;
    }

    RETURN_IF_FALSE(PauseAndStopHelper::managePauseAndStop(getSequenceState()));

    if(!_serialLink->setFlowControl(flowCtl))
    {
        SequenceLogFormatter::formatABusinessProblem(tr("(TR) Serial port %1 rejected wanted flow "
                                                        "contol").arg(portName));
        return false;
    }

    RETURN_IF_FALSE(PauseAndStopHelper::managePauseAndStop(getSequenceState()));

    return true;
}
