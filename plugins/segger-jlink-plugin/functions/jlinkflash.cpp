// SPDX-FileCopyrightText: 2019 - 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
// SPDX-FileCopyrightText: 2020 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "jlinkflash.hpp"

#include "abstractpluginelements/defaultbankglobalmanager.hpp"
#include "bankcoreshared/usebyplugin/icommoninterface.hpp"
#include "hmi-helper-lib/hmidisplayinfinitewaitview.hpp"
#include "testbedcore/logs/sequencelogformatter.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/utility/sequencefileshelper.hpp"


JLinkFlash::JLinkFlash(QObject *parent) :
    AJLinkIntfFromParams(staticMetaObject.className(), parent)
{
}

JLinkFlash::~JLinkFlash()
{
}

bool JLinkFlash::initParamsFormat(JsonArray &paramsFormat) const
{
    RETURN_IF_FALSE(AJLinkIntfFromParams::initParamsFormat(paramsFormat));

    JsonObject flashingAddress;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamNumberRequired(DestAddressParamName,
                                                                 DestAddressParamDesc,
                                                                 0,
                                                                 0,
                                                                 flashingAddress));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamStepMinMaxNumber(flashingAddress,
                                                                   Number::minUint32(),
                                                                   Number::maxUint32(),
                                                                   Number(1)));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, flashingAddress));

    paramsFormat.append(flashingAddress);

    JsonObject eraseBeforeFlash;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamBoolRequired(EraseBeforeFlashParamName,
                                                               EraseBeforeFlashParamDesc,
                                                               eraseBeforeFlash));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueBool(false, eraseBeforeFlash));

    paramsFormat.append(eraseBeforeFlash);

    JsonObject startCpuAfterFlash;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamBoolRequired(StartCpuAfterFlashParamName,
                                                               StartCpuAfterFlashParamDesc,
                                                               startCpuAfterFlash));

    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueBool(false, startCpuAfterFlash));

    paramsFormat.append(startCpuAfterFlash);

    return true;
}

bool JLinkFlash::initInputsFormat(JsonArray &inputsFormat) const
{
    RETURN_IF_FALSE(AJLinkIntfFromParams::initInputsFormat(inputsFormat));

    JsonObject binPath;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(InputBinPathName,
                                                         binPath,
                                                         InputBinPathDesc));

    inputsFormat.append(binPath);

    return true;
}

bool JLinkFlash::validateAndSetParameters(const JsonArray &parameters, JsonArray &errors)
{
    RETURN_IF_FALSE(AJLinkIntfFromParams::validateAndSetParameters(parameters, errors));

    QString error;
    Number tmpFlashingAddr;
    if(!BankJsonParser::getNumberValue(parameters,
                                        DestAddressParamName,
                                       tmpFlashingAddr,
                                       &error))
    {
        fillErrorArray(DestAddressParamName, error, errors);
        return false;
    }

    // We can do that because we test the limits of Number thanks to the parameter format and the
    // parameter verification
    _flashingAddress = tmpFlashingAddr.toUint32();

    if(!BankJsonParser::getBoolValue(parameters, EraseBeforeFlashParamName, _eraseBeforeFlash, &error))
    {
        fillErrorArray(DestAddressParamName, error, errors);
        return false;
    }

    if(!BankJsonParser::getBoolValue(parameters,
                                      StartCpuAfterFlashParamName,
                                     _startCpuAfterFlash,
                                     &error))
    {
        fillErrorArray(StartCpuAfterFlashParamName, error, errors);
        return false;
    }

    return true;
}

bool JLinkFlash::mainJLinkProcess(const JsonArray &jsonObjectInputValues,
                                  const QString &jLinkPgmPath,
                                  JsonArray &/*output*/)
{
    emit newLog(SequenceLogData(LogMsgType::Info,
                                SequenceLogCategory::BusinessScope,
                                "Flash the binary on the board"));

    if(!operate(jsonObjectInputValues, jLinkPgmPath))
    {
        emit newLog(SequenceLogFormatter::formatABusinessProblem(tr("(TR) A problem occured while "
                                                                    "flashing the board")));
        return false;
    }

    emit newLog(SequenceLogData(LogMsgType::Info,
                                SequenceLogCategory::BusinessScope,
                                "The board has been flashed"));

    return true;
}

bool JLinkFlash::operate(const JsonArray &inputValues, const QString &jLinkPgmPath)
{
    QString valueBinPath;
    RETURN_IF_FALSE(BankJsonParser::getStringValue(inputValues,
                                                   InputBinPathName,
                                                   valueBinPath));

    ICommonInterface &comItf = DefaultBankGlobalManager::Instance()->accessCommonInterface();

    QDir seqDir;
    if(!comItf.getSequenceDirectory(getSequenceToken(), seqDir))
    {
        qWarning() << "Can't get the directory path of the sequence: " << getSequenceToken();
        return false;
    }

    QString binFilePath = SequenceFilesHelper::getAbsFilePathFromSequ(seqDir, valueBinPath);

    if(binFilePath.isEmpty())
    {
        return false;
    }

    HmiDisplayInfiniteWaitView displayHmi(comItf);
    RETURN_IF_FALSE(displayHmi.displayInfiniteHmi(HmiName, tr(HmiDescription), false));

    QString flashingAddress = QString("0x%1").arg(_flashingAddress,
                                                  JLinkFlash::FlashingAddressFieldWidth,
                                                  JLinkFlash::HexBase,
                                                  QChar('0'));

    QStringList commands = { JLinkCmdEoeEnable,      // Activate the exit on error
                            JLinkCmdConnect,        // Connect to target
                            JLinkCmdResetAndHalts }; // Always add a reset after connect, normaly
                                                     // it's not needed but if the board is in a
                                                     // strange state, the reset is compulsory to be
                                                     // in a correct state

    if(_eraseBeforeFlash)
    {
        commands.append({ JLinkCmdErase, JLinkCmdResetAndHalts });
    }

    commands.append({ QString(JLinkCmdLoadFile).arg(binFilePath, flashingAddress),
                      JLinkCmdResetAndHalts });

    if(_startCpuAfterFlash)
    {
        commands.append(JLinkCmdCpuStart);
    }

    commands.append(JLinkCmdCloseAndQuit);
    
    RETURN_IF_FALSE(callCmdsProcess(commands, jLinkPgmPath, getTimeoutInMs()));

    return true;
}
