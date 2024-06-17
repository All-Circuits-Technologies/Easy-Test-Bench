// SPDX-FileCopyrightText: 2020 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "ajlinkintffromparams.hpp"

#include <QFile>

#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/testbedglobal.hpp"

const QStringList AJLinkIntfFromParams::CmdsListHeader = {
    AJLinkIntfFromParams::JLinkCmdEoeEnable
};


AJLinkIntfFromParams::AJLinkIntfFromParams(const QString &moduleClassName, QObject *parent) :
    AJLinkInterface(moduleClassName, parent)
{
}

AJLinkIntfFromParams::~AJLinkIntfFromParams()
{
}

bool AJLinkIntfFromParams::initParamsFormat(JsonArray &paramsFormat) const
{
    RETURN_IF_FALSE(AJLinkInterface::initParamsFormat(paramsFormat));

    JsonObject deviceObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(DeviceParamName,
                                                                 DeviceParamDesc,
                                                                 deviceObj));
    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, deviceObj));

    paramsFormat.append(deviceObj);

    JsonObject interfaceObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(InterfaceParamName,
                                                                 InterfaceParamDesc,
                                                                 interfaceObj));
    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, interfaceObj));

    paramsFormat.append(interfaceObj);

    JsonObject speedObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(SpeedParamName,
                                                                 SpeedParamDesc,
                                                                 0,
                                                                 0,
                                                                 speedObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamUnitNumber(Unit(UnitType::Hertz, 3), speedObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(speedObj,
                                                                   Number(1),
                                                                   Number(),
                                                                   Number(1)));
    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, speedObj));

    paramsFormat.append(speedObj);

    return true;
}

bool AJLinkIntfFromParams::validateAndSetParameters(const JsonArray &parameters, JsonArray &errors)
{
    RETURN_IF_FALSE(AJLinkInterface::validateAndSetParameters(parameters, errors));

    QString error;

    if(!BankJsonParser::getStringValue(parameters, DeviceParamName, _valueDevice, &error))
    {
        fillErrorArray(DeviceParamName, error, errors);
        return false;
    }

    if(!BankJsonParser::getStringValue(parameters, InterfaceParamName, _valueInterface, &error))
    {
        fillErrorArray(InterfaceParamName, error, errors);
        return false;
    }

    if(!BankJsonParser::getNumberValue(parameters, SpeedParamName, _valueSpeed, &error))
    {
        fillErrorArray(SpeedParamName, error, errors);
        return false;
    }

    return true;
}

bool AJLinkIntfFromParams::callCmdsProcess(const QStringList &commands,
                                       const QString &jLinkPgmPath,
                                       int timeoutInMs,
                                       bool logProcessError,
                                       bool *processExitProperly)
{
    QFile tmpFile;

    // Initialize the temporary commands with header commands
    QStringList tmpCommands(CmdsListHeader);

    // Add the device connection information
    appendCommunicationCmds(tmpCommands);

    // Add the commands given by the derived class
    tmpCommands.append(commands);

    RETURN_IF_FALSE(createCommandScriptFile(tmpCommands, tmpFile));

    qDebug() << "Try to interact with board, arguments: " << tmpCommands.join(", ");
    
    bool result = callJLinkPgm(tmpFile,
                               jLinkPgmPath,
                               timeoutInMs,
                               logProcessError,
                               processExitProperly);

    tmpFile.remove();

    return result;
}

void AJLinkIntfFromParams::appendCommunicationCmds(QStringList &commands)
{
    commands.append(QString(JLinkCmdDevice).arg(_valueDevice));
    commands.append(QString(JLinkCmdSelectInterface).arg(_valueInterface));
    commands.append(QString(JLinkCmdSpeed).arg(_valueSpeed));
}
