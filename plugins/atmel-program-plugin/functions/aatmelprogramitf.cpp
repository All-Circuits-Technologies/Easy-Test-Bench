// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "aatmelprogramitf.hpp"

#include <QFile>
#include <QProcess>

#include "abstractpluginelements/defaultbankglobalmanager.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/testbedglobal.hpp"
#include "waitutility/waithelper.hpp"


AAtmelProgramItf::AAtmelProgramItf(const QString &moduleClassName,
                                   QObject *parent) :
    DefaultSequenceInstanceModule(moduleClassName,
                                  DefaultBankGlobalManager::Instance(),
                                  parent)
{
}

bool AAtmelProgramItf::initParamsFormat(JsonArray &paramsFormat) const
{
    JsonObject deviceObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(ChipUsedParamName,
                                                                 ChipUsedParamDesc,
                                                                 deviceObj));
    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, deviceObj));

    paramsFormat.append(deviceObj);

    JsonObject interfaceObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamStringRequired(PhysicalItfParamName,
                                                                 PhysicalItfParamDesc,
                                                                 interfaceObj));
    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, interfaceObj));

    paramsFormat.append(interfaceObj);

    JsonObject timeoutObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(TimeoutMsParamName,
                                                                 TimeoutMsParamDesc,
                                                                 0,
                                                                 0,
                                                                 timeoutObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamUnitNumber(Unit(UnitType::Second, -3),
                                                             timeoutObj));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(timeoutObj,
                                                                   Number(1, 0, false),
                                                                   Number::maxInt32(),
                                                                   Number(1)));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueNumber(
        Number::fromInt32(DefaultTimeoutInMs),
        timeoutObj));

    paramsFormat.append(timeoutObj);

    return true;
}

bool AAtmelProgramItf::initInputsFormat(JsonArray &inputsFormat) const
{
    JsonObject atProgramObj;
    
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(InputAtPgmExePathName,
                                                         atProgramObj,
                                                         InputAtPgmExePathDesc));

    inputsFormat.append(atProgramObj);
    return true;
}

bool AAtmelProgramItf::validateAndSetParameters(const JsonArray &parameters, JsonArray &errors)
{
    RETURN_IF_FALSE(DefaultSequenceInstanceModule::validateAndSetParameters(parameters, errors));

    QString error;

    QString tmpPhysicalItf;
    if(!BankJsonParser::getStringValue(parameters, PhysicalItfParamName, tmpPhysicalItf, &error))
    {
        fillErrorArray(PhysicalItfParamName, error, errors);
        return false;
    }

    _itfType = AtProgramPhysicalItfType::parseFromString(tmpPhysicalItf);

    if(_itfType == AtProgramPhysicalItfType::Unknown)
    {
        MANAGE_LOG_ERR(QString("Unknown physical interface: %1, to communicate "
                               "with the atmel board").arg(_itfType),
                       error);
        fillErrorArray(PhysicalItfParamName, error, errors);
        return false;
    }
    
    if(!BankJsonParser::getStringValue(parameters, ChipUsedParamName, _chipName, &error))
    {
        fillErrorArray(ChipUsedParamName, error, errors);
        return false;
    }

    Number tmpTimeout;
    if(!BankJsonParser::getNumberValue(parameters,
                                        TimeoutMsParamName,
                                       tmpTimeout,
                                       &error))
    {
        fillErrorArray(TimeoutMsParamName, error, errors);
        return false;
    }

    // An error cannot occure because the limits of the number are already tested in the validate
    // module parameters
    _timeoutInMs = tmpTimeout.toInt32();

    return true;
}

bool AAtmelProgramItf::callProcess(const QString &atProgramPath,
                                   const QStringList &arguments,
                                   bool logProcessError,
                                   int overrideTimeoutInMs,
                                   bool *processExitProperly)
{
    int timeoutInMs = _timeoutInMs;

    if(processExitProperly != nullptr)
    {
        *processExitProperly  = false;
    }

    if(overrideTimeoutInMs >= 0)
    {
        // Use the timeout overloaded
        timeoutInMs = overrideTimeoutInMs;
    }

    QStringList defaultArgs = {
        AtPgmDbgArg,        AtPgmDefaultDbg,
        AtPgmItfArg,        AtProgramPhysicalItfType::toString(_itfType),
        AtPgmChipUsedArg,   _chipName
    };

    defaultArgs.append(arguments);

    qDebug() << "Try to call process: " << atProgramPath << " " << defaultArgs.join(" ");

    QProcess process;

    bool waitForStarted = false;
    bool waitForFinished = false;

    auto tokenStart = connect(&process, &QProcess::started,
                              this, [&waitForStarted]() { waitForStarted = true; });
    auto tokenFinish = connect(&process, qOverload<int, QProcess::ExitStatus>(&QProcess::finished),
                               this, [&waitForFinished]() { waitForFinished = true; });

    process.start(atProgramPath,
                  defaultArgs);

    if(!WaitHelper::pseudoWait(waitForStarted, timeoutInMs))
    {
        disconnect(tokenStart);
        disconnect(tokenFinish);

        qWarning() << "Process not started";
        qWarning() << "StandartOutput: "<< process.readAllStandardOutput()
                                                .right(ProcessLogCharLimitToDisplay);
        qWarning() << "ErrorOutput: " << process.readAllStandardError()
                                             .right(ProcessLogCharLimitToDisplay);
        return false;
    }

    disconnect(tokenStart);

    if(!WaitHelper::pseudoWait(waitForFinished, timeoutInMs))
    {
        disconnect(tokenFinish);

        if(logProcessError)
        {
            qWarning() << "Atprogram didn't end as expected, timeout occured: " << process.error()
                       << " Exit code: " << process.exitCode();
            qWarning() << "StandartOutput: "<< process.readAllStandardOutput()
                                                    .right(ProcessLogCharLimitToDisplay);
            qWarning() << "ErrorOutput: " << process.readAllStandardError()
                                                 .right(ProcessLogCharLimitToDisplay);
        }
        return false;
    }

    disconnect(tokenFinish);

    if((process.exitStatus() != QProcess::NormalExit) || (process.exitCode() != 0))
    {
        if(processExitProperly == nullptr && logProcessError)
        {
            // If processExitProperly isn't equal to nullptr, we can deduce that's an expected case
            // (and managed) therefore it's useless to log the error output
            qWarning() << "Atprogram didn't end as expected : " << process.error()
                       << " Exit code: " << process.exitCode();
            qWarning() << "StandartOutput: "<< process.readAllStandardOutput()
                                                    .right(ProcessLogCharLimitToDisplay);
            qWarning() << "ErrorOutput: " << process.readAllStandardError()
                                                 .right(ProcessLogCharLimitToDisplay);
        }

        return (processExitProperly != nullptr);
    }

    if(processExitProperly != nullptr)
    {
        *processExitProperly  = true;
    }

    return true;
}

bool AAtmelProgramItf::parseAtProgramPath(const JsonArray &jsonObjectInputValues,
                                          QString &atProgramPath)
{
    if(!BankJsonParser::getStringValue(jsonObjectInputValues,
                                        InputAtPgmExePathName,
                                       atProgramPath))
    {
        qWarning() << "A problem occured when tryied to get the AT program path from input values";
        return false;
    }

    // Check the presence of the file here rather than in function operate to spot an error sooner
    if(!QFile::exists(atProgramPath))
    {
        qWarning() << "atprogram.exe not found here: " << atProgramPath;
        return false;
    }

    return true;
}
