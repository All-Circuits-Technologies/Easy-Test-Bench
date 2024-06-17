// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "ajlinkintffromfile.hpp"

#include <QDebug>
#include <QFile>

#include "abstractpluginelements/defaultbankglobalmanager.hpp"
#include "bankcoreshared/usebyplugin/icommoninterface.hpp"
#include "hmi-helper-lib/hmidisplayinfinitewaitview.hpp"
#include "testbedcore/logs/sequencelogformatter.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/utility/sequencefileshelper.hpp"


AJLinkIntfFromFile::AJLinkIntfFromFile(int argumentsNb,
                                       const QString &moduleClassName,
                                       QObject *parent)
    : AJLinkInterface{moduleClassName, parent},
    MixinMultiStringArgsModule{argumentsNb}
{
}

AJLinkIntfFromFile::~AJLinkIntfFromFile()
{
}

bool AJLinkIntfFromFile::initInputsFormat(JsonArray &inputsFormat) const
{
    RETURN_IF_FALSE(AJLinkInterface::initInputsFormat(inputsFormat));
    RETURN_IF_FALSE(initArgsInputsFormat(inputsFormat));

    JsonObject jLinkFileInput;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(
        InputJLinkFilePathName,
        jLinkFileInput,
        InputJLinkFilePathDesc));

    inputsFormat.append(jLinkFileInput);

    return true;
}

bool AJLinkIntfFromFile::initParamsFormat(JsonArray &paramsFormat) const
{
    RETURN_IF_FALSE(AJLinkInterface::initParamsFormat(paramsFormat));
    return initArgsParamsFormat(paramsFormat);
}

bool AJLinkIntfFromFile::validateAndSetParameters(const JsonArray &parameters, JsonArray &errors)
{
    RETURN_IF_FALSE(AJLinkInterface::validateAndSetParameters(parameters, errors));
    return validateAndSetArgsParameters(parameters, errors);
}

bool AJLinkIntfFromFile::mainJLinkProcess(const JsonArray &jsonObjectInputValues,
                                          const QString &jLinkPgmPath,
                                          JsonArray &/*output*/)
{
    emit newLog(SequenceLogData(LogMsgType::Info,
                                SequenceLogCategory::BusinessScope,
                                "Call JLink program with the JLink file given"));

    if(!operate(jsonObjectInputValues, jLinkPgmPath))
    {

        emit newLog(SequenceLogFormatter::formatABusinessProblem(tr("(TR) A problem occured while "
                                                                    "calling JLink program with "
                                                                    "file")));

        return false;
    }

    emit newLog(SequenceLogData(LogMsgType::Info,
                                SequenceLogCategory::BusinessScope,
                                "JLink file has been succesfully called"));
    return true;
}

bool AJLinkIntfFromFile::operate(const JsonArray &jsonObjectInputValues,
                                 const QString &jLinkPgmPath)
{
    QVector<QString> inputValues;

    RETURN_IF_FALSE(extractArgumentsFromInputs(jsonObjectInputValues, inputValues));

    QString jLinkFile;
    RETURN_IF_FALSE(BankJsonParser::getStringValue(jsonObjectInputValues,
                                                   InputJLinkFilePathName,
                                                   jLinkFile));

    ICommonInterface &comItf = DefaultBankGlobalManager::Instance()->accessCommonInterface();

    QDir seqDir;
    if(!comItf.getSequenceDirectory(getSequenceToken(), seqDir))
    {
        qWarning() << "Can't get the directory path of the sequence: " << getSequenceToken();
        return false;
    }

    QString absJLinkFilePath = SequenceFilesHelper::getAbsFilePathFromSequ(seqDir, jLinkFile);

    if(absJLinkFilePath.isEmpty())
    {
        return false;
    }

    HmiDisplayInfiniteWaitView displayHmi(comItf);
    RETURN_IF_FALSE(displayHmi.displayInfiniteHmi(HmiName, tr(HmiDescription), false));

    QFile tmpFile;
    RETURN_IF_FALSE(createCommandScriptFromFile(absJLinkFilePath, inputValues, tmpFile));
    
    bool result = callJLinkPgm(tmpFile, jLinkPgmPath, getTimeoutInMs());

    tmpFile.remove();

    return result;
}

bool AJLinkIntfFromFile::createCommandScriptFromFile(const QString &jLinkFile,
                                                     const QVector<QString> &inputValues,
                                                     QFile &tmpFile)
{
    QFile jLinkFileFromInput(jLinkFile);

    QStringList commands;

    if(!jLinkFileFromInput.open(QIODevice::ReadOnly))
    {
        qWarning() << "A problem occurred when tried to read the JLink file: " << jLinkFile;
        return false;
    }

    QString lineRead = jLinkFileFromInput.readLine();

    while(!lineRead.isNull())
    {
        lineRead = lineRead.trimmed();
        RETURN_IF_FALSE(replaceArgumentsInString(inputValues, lineRead));

        commands.append(lineRead);

        lineRead = jLinkFileFromInput.readLine();
    }

    jLinkFileFromInput.close();

    return createCommandScriptFile(commands, tmpFile);
}
