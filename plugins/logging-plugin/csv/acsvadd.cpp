// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "acsvadd.hpp"

#include "abstractpluginelements/defaultbankglobalmanager.hpp"
#include "testbedcore/logs/sequencelogformatter.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/testbedglobal.hpp"
#include "testbedcore/utility/pauseandstophelper.hpp"


ACsvAdd::ACsvAdd(const QString &moduleClassName,
                 QObject *parent) :
    DefaultSequenceInstanceModule(moduleClassName,
                                  DefaultBankGlobalManager::Instance(),
                                  parent)
{
}

void ACsvAdd::processModule(const JsonArray &jsonObjectInputValues)
{
    if(!PauseAndStopHelper::managePauseAndStop(getSequenceState()))
    {
        emit moduleFinished(false, {});
        return;
    }

    emit newLog(SequenceLogData(LogMsgType::Info,
                                SequenceLogCategory::BusinessScope,
                                "Add an element to the CSV line"));

    QVector<QString> csvLine;
    if(!BankJsonParser::getArrayStringValue(jsonObjectInputValues,
                                             InputCsvLineName,
                                            csvLine))
    {
        emit newLog(SequenceLogFormatter::formatABusinessProblem(tr("(TR) Can't get the csv line "
                                                                    "data")));
        emit moduleFinished(false, {});
        return;
    }

    if(!append(jsonObjectInputValues, csvLine))
    {
        emit newLog(SequenceLogFormatter::formatABusinessProblem(tr("(TR) A problem occured while "
                                                                    "trying to append an element "
                                                                    "to the CSV line.")));
        emit moduleFinished(false, {});
        return;
    }

    JsonArray output;
    if(!BankJsonParser::setArrayStringValue(OutputCsvLineName,
                                            csvLine,
                                            output))
    {
        emit newLog(SequenceLogFormatter::formatABusinessProblem(tr("(TR) A problem occured while "
                                                                    "trying to output "
                                                                    "the CSV line.")));
        emit moduleFinished(false, {});
        return;
    }

    emit newLog(SequenceLogData(LogMsgType::Info,
                                SequenceLogCategory::BusinessScope,
                                "The CSV line has been append"));

    emit moduleFinished(true, output);
}

bool ACsvAdd::initInputsFormat(JsonArray &inputsFormat) const
{
    JsonObject csvLine;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutStringArray(InputCsvLineName,
                                                              csvLine,
                                                              InputCsvLineDesc));

    inputsFormat.append(csvLine);

    return true;
}

bool ACsvAdd::initOutputsFormat(JsonArray &outputsFormat) const
{
    JsonObject csvLine;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutStringArray(OutputCsvLineName,
                                                              csvLine,
                                                              OutputCsvLineDesc));

    outputsFormat.append(csvLine);

    return true;
}
