// SPDX-FileCopyrightText: 2020 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "ajlinkinterface.hpp"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QTemporaryFile>

#include "abstractpluginelements/defaultbankglobalmanager.hpp"
#include "bankcoreshared/usebyplugin/icommoninterface.hpp"
#include "testbedcore/logs/sequencelogformatter.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/testbedglobal.hpp"
#include "testbedcore/utility/pauseandstophelper.hpp"
#include "testbedcore/utility/sequencefileshelper.hpp"
#include "waitutility/waithelper.hpp"


AJLinkInterface::AJLinkInterface(const QString &moduleClassName, QObject *parent)
    : DefaultSequenceInstanceModule{moduleClassName, DefaultBankGlobalManager::Instance(), parent}
{
}

AJLinkInterface::~AJLinkInterface()
{
}

bool AJLinkInterface::initInputsFormat(JsonArray &inputsFormat) const
{
    JsonObject inputJLinkPath;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(
        InputJLinkPgmPathName,
        inputJLinkPath,
        InputJLinkPgmPathDesc));

    inputsFormat.append(inputJLinkPath);
    return true;
}

bool AJLinkInterface::initParamsFormat(JsonArray &paramsFormat) const
{
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

bool AJLinkInterface::validateAndSetParameters(const JsonArray &parameters, JsonArray &errors)
{
    RETURN_IF_FALSE(DefaultSequenceInstanceModule::validateAndSetParameters(parameters, errors));

    QString error;

    Number timeoutInMs;
    if(!BankJsonParser::getNumberValue(parameters,
                                        TimeoutMsParamName,
                                        timeoutInMs,
                                        &error,
                                        true))
    {
        fillErrorArray(TimeoutMsParamName, error, errors);
        return false;
    }

    // The integer boundaries are already tested before
    if(timeoutInMs.isValid())
    {
        _timeoutInMs = timeoutInMs.toInt32();
    }
    else
    {
        _timeoutInMs = DefaultTimeoutInMs;
    }

    return true;
}

bool AJLinkInterface::callJLinkPgm(const QFile &jLinkScriptFile,
                                  const QString &jLinkPgmPath,
                                  int timeoutInMs,
                                  bool logProcessError,
                                  bool *processExitProperly)
{
    QProcess process;

    bool waitForStarted = false;
    bool waitForFinished = false;

    auto tokenStart = connect(&process, &QProcess::started,
                              this, [&waitForStarted]() { waitForStarted = true; });
    auto tokenFinish = connect(&process, qOverload<int, QProcess::ExitStatus>(&QProcess::finished),
                               this, [&waitForFinished]() { waitForFinished = true; });

    process.start(jLinkPgmPath, { "-CommanderScript", jLinkScriptFile.fileName() });

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
            qWarning() << "Jlink didn't end as expected, timeout occured: " << process.error()
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
        if(logProcessError)
        {
            qWarning() << "Jlink didn't end as expected : " << process.error()
                       << " Exit code: " << process.exitCode();
            qWarning() << "StandartOutput: "<< process.readAllStandardOutput()
                                                    .right(ProcessLogCharLimitToDisplay);
            qWarning() << "ErrorOutput: " << process.readAllStandardError()
                                                 .right(ProcessLogCharLimitToDisplay);
        }

        if(processExitProperly != nullptr)
        {
            *processExitProperly = false;
        }

        // If processExitProperly is equal to null, it means that the caller has no way to know if
        // the call has succeeded or not, that's why we return false here.
        // If processExitProperly is not equal to null, it means that the caller gets the cmd result
        // throught this variable; therefore we don't return false (which means that a not managed
        // error has occurred).
        return (processExitProperly != nullptr);
    }

    if(processExitProperly != nullptr)
    {
        *processExitProperly = true;
    }

    return true;
}

bool AJLinkInterface::createCommandScriptFile(const QStringList &commands, QFile &tmpFile)
{
    QString tmpFileName;

    {
        /* QTemporaryFile are locked and can't be used outside of the app by lock-aware tools.
           We create it to get the random name and create a QFile with it to be used by JLink.exe */
        QTemporaryFile temporaryFile(QString(JLinkCompleteTmpFilePath).arg(
            QDir::tempPath(),
            QString(JLinkFileNameTmpPart).arg(QString(JLinkTmpFileBaseName),
                                              QString(JLinkSuffix))));

        temporaryFile.open();
        tmpFileName = temporaryFile.fileName();
    }

    tmpFile.setFileName(tmpFileName);

    if(!tmpFile.open(QIODevice::WriteOnly))
    {
        tmpFile.remove();
        qWarning() << "Can't open in writing mode, the file: " << tmpFileName;
        return false;
    }

    QTextStream jlinkCommandStream(&tmpFile);
    jlinkCommandStream << commands.join("\n");
    tmpFile.close();

    return true;
}

void AJLinkInterface::processModule(const JsonArray &jsonObjectInputValues)
{
    if(!PauseAndStopHelper::managePauseAndStop(getSequenceState()))
    {
        emit moduleFinished(false, {});
        return;
    }

    JsonArray output;
    if(!localJLinkProcess(jsonObjectInputValues, output))
    {
        emit newLog(SequenceLogFormatter::formatABusinessProblem(tr("(TR) A problem occurred while "
                                                                    "trying to work with JLink "
                                                                    "interface")));
        emit moduleFinished(false, {});
        return;
    }

    emit moduleFinished(true, output);
}

bool AJLinkInterface::localJLinkProcess(const JsonArray &jsonObjectInputValues, JsonArray &output)
{
    QString jLinkPgmPath;

    RETURN_IF_FALSE(BankJsonParser::getStringValue(jsonObjectInputValues,
                                                   InputJLinkPgmPathName,
                                                   jLinkPgmPath));

    ICommonInterface &comItf = DefaultBankGlobalManager::Instance()->accessCommonInterface();

    QDir seqDir;
    if(!comItf.getSequenceDirectory(getSequenceToken(), seqDir))
    {
        qWarning() << "Can't get the directory path of the sequence: " << getSequenceToken();
        return false;
    }

    QString absJLinkPgmPath = SequenceFilesHelper::getAbsFilePathFromSequ(seqDir, jLinkPgmPath);

    if(absJLinkPgmPath.isEmpty())
    {
        return false;
    }

    return mainJLinkProcess(jsonObjectInputValues, absJLinkPgmPath, output);
}
