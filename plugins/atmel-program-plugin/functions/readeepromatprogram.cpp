// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "readeepromatprogram.hpp"

#include "abstractpluginelements/defaultbankglobalmanager.hpp"
#include "fileutility/temporaryfile.hpp"
#include "hmi-helper-lib/hmidisplayinfinitewaitview.hpp"
#include "testbedcore/logs/sequencelogformatter.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/testbedglobal.hpp"
#include "testbedcore/utility/pauseandstophelper.hpp"

#include "types/atprogramcmds.hpp"
#include "types/memorytype.hpp"


ReadEepromAtProgram::ReadEepromAtProgram(QObject *parent) :
    AAtmelProgramItf(staticMetaObject.className(), parent)
{
}

ReadEepromAtProgram::~ReadEepromAtProgram()
{
}

bool ReadEepromAtProgram::initOutputsFormat(JsonArray &outputsFormat) const
{
    RETURN_IF_FALSE(AAtmelProgramItf::initOutputsFormat(outputsFormat));

    JsonObject ouputStringFileRead;
    if (!BankJsonFormatter::formatInOutString(OutputReadFileName,
                                              ouputStringFileRead,
                                              OutputReadFileDescription))
    {
        return false;
    }

    outputsFormat.append(ouputStringFileRead);

    return true;
}

void ReadEepromAtProgram::processModule(const JsonArray &jsonObjectInputValues)
{
    Q_UNUSED(jsonObjectInputValues)

    if(!PauseAndStopHelper::managePauseAndStop(getSequenceState()))
    {
        emit moduleFinished(false, {});
        return;
    }

    emit newLog(SequenceLogData(LogMsgType::Info,
                                SequenceLogCategory::BusinessScope,
                                "Read the binary on the board"));

    bool success = readProcess(jsonObjectInputValues);

    if(!success)
    {
        emit newLog(SequenceLogFormatter::formatABusinessProblem(tr("(TR) A problem occured while "
                                                                    "reading the board")));
        emit moduleFinished(false, {});
        return;
    }

    JsonArray output;

    QString error;
    success = BankJsonParser::setStringValue(OutputReadFileName,
                                             _readInEepromFile->fileName(),
                                             output,
                                             &error);

    if(!success)
    {
        emit newLog(SequenceLogFormatter::formatABusinessProblem(tr("(TR) A problem occured while "
                                                                    "getting the hex file which "
                                                                    "contains the EEPROM "
                                                                    "content")));
        emit moduleFinished(false, {});
        return;
    }

    emit newLog(SequenceLogData(LogMsgType::Info,
                                SequenceLogCategory::BusinessScope,
                                "The EEPROM has been read"));

    emit moduleFinished(true, output);
}

bool ReadEepromAtProgram::readProcess(const JsonArray &jsonObjectInputValues)
{
    QString atPgmPath;
    RETURN_IF_FALSE(parseAtProgramPath(jsonObjectInputValues, atPgmPath));

    ICommonInterface &comItf = DefaultBankGlobalManager::Instance()->accessCommonInterface();

    HmiDisplayInfiniteWaitView displayHmi(comItf);
    RETURN_IF_FALSE(displayHmi.displayInfiniteHmi(HmiName, tr(HmiDescription), false));

    RETURN_IF_FALSE(manageTemporaryFile());

    if(!callProcess(atPgmPath,
                    {
                        AtProgramCmds::toString(AtProgramCmds::Read),
                        MemoryType::toProgramArgumentStr(MemoryType::Eeprom),
                        AtPgmFileArg, _readInEepromFile->fileName(),
                    }))
    {
        delete _readInEepromFile;
        _readInEepromFile = nullptr;
        return false;
    }

    return true;
}

bool ReadEepromAtProgram::manageTemporaryFile()
{
    // If file already exists, remove it
    delete _readInEepromFile;
    _readInEepromFile = new TemporaryFile(HexFileBaseName, FileHelper::IntelHexSuffixFileName, this);
    if(!_readInEepromFile->open(QIODevice::ReadWrite))
    {
        qWarning() << "A problem occured when tried to create the temporary file: "
                   << _readInEepromFile->fileName() << ", for reading the Atmel device EEPROM";
        return false;
    }

    _readInEepromFile->close();

    return true;
}
