// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "oneboardonefile.hpp"

#include "fileutility/filehelper.hpp"
#include "logsutility/saveloginfilesthread.hpp"
#include "testbedcore/logs/sequencelogformatter.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/testbedglobal.hpp"

const QHash<char, QString> OneBoardOneFile::DefaultReplacementStrings = {
    { '/', "-" }
};


OneBoardOneFile::OneBoardOneFile(QObject *parent) :
    ASavingLogFile(staticMetaObject.className(), parent)
{
}

OneBoardOneFile::~OneBoardOneFile()
{
}

bool OneBoardOneFile::initInputsFormat(JsonArray &inputsFormat) const
{
    RETURN_IF_FALSE(ASavingLogFile::initInputsFormat(inputsFormat));

    JsonObject serialNumberFormat;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(InputSerialNumberName,
                                                         serialNumberFormat,
                                                         InputSerialNumberDesc));

    inputsFormat.append(serialNumberFormat);

    return true;
}

void OneBoardOneFile::firstLogToSave(SequenceLogData &firstFileLog) const
{
    firstFileLog = SequenceLogFormatter::formatFirstLog(_serialNumber);
}

void OneBoardOneFile::getLogArguments(const SequenceLogData &/*log*/, LoggingOptions &arguments)
{
    arguments.insert(LoggingOption::Id,
                     FileHelper::cleanFileName(_serialNumber,
                                               FileHelper::CleanFileNameDefaultPattern,
                                               FileHelper::CleanFileNameDefaultReplacement,
                                               DefaultReplacementStrings));
}

bool OneBoardOneFile::startLoggingProcess(const QString &logsFolderPath,
                                          const JsonArray &jsonObjectInputValues)
{
    RETURN_IF_FALSE(BankJsonParser::getStringValue(jsonObjectInputValues,
                                                   InputSerialNumberName,
                                                   _serialNumber));

    RETURN_IF_FALSE(accessLogsFileThread()->resetToOneFilePerObjectStrategy(
                                                                        logsFolderPath,
                                                                        getLogFileSuffix(),
                                                                        DefaultStrategies,
                                                                        getMaxFolderSizeInMo()));

    RETURN_IF_FALSE(accessLogsFileThread()->startLogging());

    return true;
}
