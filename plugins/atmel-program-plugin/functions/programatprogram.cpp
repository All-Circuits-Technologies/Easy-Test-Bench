// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "programatprogram.hpp"

#include <QFileInfo>

#include "abstractpluginelements/defaultbankglobalmanager.hpp"
#include "hmi-helper-lib/hmidisplayinfinitewaitview.hpp"
#include "testbedcore/logs/sequencelogformatter.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/testbedglobal.hpp"
#include "testbedcore/utility/pauseandstophelper.hpp"

#include "types/atprogramcmds.hpp"


ProgramAtProgram::ProgramAtProgram(QObject *parent) :
    AAtmelProgramItf(staticMetaObject.className(), parent)
{
}

ProgramAtProgram::~ProgramAtProgram()
{
}

void ProgramAtProgram::processModule(const JsonArray &jsonObjectInputValues)
{
    if(!PauseAndStopHelper::managePauseAndStop(getSequenceState()))
    {
        emit moduleFinished(false, {});
        return;
    }

    emit newLog(SequenceLogData(LogMsgType::Info,
                                SequenceLogCategory::BusinessScope,
                                "Programing atmel memory"));

    bool success = writeProcess(jsonObjectInputValues);

    if(!success)
    {
        emit newLog(SequenceLogFormatter::formatABusinessProblem(tr("(TR) A problem occured while "
                                                                    "writing the board")));
    }
    else
    {
        emit newLog(SequenceLogData(LogMsgType::Info,
                                    SequenceLogCategory::BusinessScope,
                                    "The memory has been written"));
    }

    emit moduleFinished(success, {});
}

bool ProgramAtProgram::initInputsFormat(JsonArray &inputsFormat) const
{
    RETURN_IF_FALSE(AAtmelProgramItf::initInputsFormat(inputsFormat));

    JsonObject inputReadFile;
    if(!BankJsonFormatter::formatInOutString(InputReadFileName,
                                             inputReadFile,
                                              InputReadFileDescription))
    {
        return false;
    }

    inputsFormat.append(inputReadFile);

    return true;
}

bool ProgramAtProgram::initParamsFormat(JsonArray &paramsFormat) const
{
    RETURN_IF_FALSE(AAtmelProgramItf::initParamsFormat(paramsFormat));

    JsonObject verifyAfterProgramObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamBoolRequired(VerifyAfterProgramParamName,
                                                               VerifyAfterProgramParamDesc,
                                                               verifyAfterProgramObj));
    
    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueBool(DefaultValueVerifyAfterProgram,
                                                                   verifyAfterProgramObj));

    paramsFormat.append(verifyAfterProgramObj);

    JsonObject deleteFileAfterProgramObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamBoolRequired(DeleteFileAfterProgramParamName,
                                                               DeleteFileAfterProgramParamDesc,
                                                               deleteFileAfterProgramObj));
    
    RETURN_IF_FALSE(BankJsonFormatter::formatParamDefaultValueBool(DefaultDeleteFileAfterProgram,
                                                                   deleteFileAfterProgramObj));

    paramsFormat.append(deleteFileAfterProgramObj);

    JsonObject memoryTypeObj;
    RETURN_IF_FALSE(BankJsonFormatter::formatParamSelectRequired(MemoryTypeParamName,
                                                                 MemoryTypeParamDesc,
                                                                 memoryTypeObj));

    for(MemoryType::Enum memoryType : MemoryType::getAllEnums(true))
    {
        QString memoryTypeStr = MemoryType::toString(memoryType);
        RETURN_IF_FALSE(BankJsonFormatter::formatParamAddSelectOption(memoryTypeStr,
                                                                      memoryTypeStr,
                                                                      memoryTypeObj));
    }

    RETURN_IF_FALSE(BankJsonFormatter::formatParamRequired(true, memoryTypeObj));

    paramsFormat.append(memoryTypeObj);

    return true;
}

bool ProgramAtProgram::validateAndSetParameters(const JsonArray &parameters,
                                                      JsonArray &errors)
{
    RETURN_IF_FALSE(AAtmelProgramItf::validateAndSetParameters(parameters, errors));

    QString error;
    if(!BankJsonParser::getBoolValue(parameters,
                                      VerifyAfterProgramParamName,
                                     _verifyAfterWriting,
                                     &error,
                                     true))
    {
        fillErrorArray(VerifyAfterProgramParamName, error, errors);
        return false;
    }

    if(!BankJsonParser::getBoolValue(parameters,
                                      DeleteFileAfterProgramParamName,
                                     _deleteFileAfterProgram,
                                     &error,
                                     true))
    {
        fillErrorArray(DeleteFileAfterProgramParamName, error, errors);
        return false;
    }

    QVector<QString> memoryTypeStr;
    if(!BankJsonParser::getArrayStringValue(parameters, MemoryTypeParamName, memoryTypeStr, &error))
    {
        fillErrorArray(MemoryTypeParamName, error, errors);
        return false;
    }

    if(memoryTypeStr.isEmpty())
    {
        fillErrorArray(MemoryTypeParamName, "No memory type selected", errors);
        return false;
    }

    _memoryType = MemoryType::parseFromString(memoryTypeStr.first());

    if(_memoryType == MemoryType::Unknown)
    {
        fillErrorArray(MemoryTypeParamName,
                       "The memory type is unknown: " + memoryTypeStr.first(),
                       errors);
        return false;
    }

    return true;
}

bool ProgramAtProgram::writeProcess(const JsonArray &jsonObjectInputValues)
{
    QString atPgmPath;
    RETURN_IF_FALSE(parseAtProgramPath(jsonObjectInputValues, atPgmPath));

    QString hexFilePath;
    RETURN_IF_FALSE(BankJsonParser::getStringValue(jsonObjectInputValues,
                                                   InputReadFileName,
                                                   hexFilePath));

    ICommonInterface &comItf = DefaultBankGlobalManager::Instance()->accessCommonInterface();

    HmiDisplayInfiniteWaitView displayHmi(comItf);
    RETURN_IF_FALSE(displayHmi.displayInfiniteHmi(HmiName, tr(HmiDescription), false));

    RETURN_IF_FALSE(callProcess(atPgmPath,
                                {
                                    AtProgramCmds::toString(AtProgramCmds::Program),
                                    MemoryType::toProgramArgumentStr(_memoryType),
                                    AtPgmVerifyArg,
                                    AtPgmFileArg, hexFilePath,
                                }));

    if(_deleteFileAfterProgram && !QFile::remove(hexFilePath))
    {
        qWarning() << "Cannot remove the: " << hexFilePath << ", hex file after having programing "
                      "its content: " << QFileInfo(hexFilePath).permissions();
        return false;
    }

    return true;
}
