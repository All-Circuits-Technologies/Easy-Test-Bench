// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "stopautoanswer.hpp"

#include "peak-can-lib/canmodulesutility.hpp"
#include "peak-can-lib/peakcanlibconstants.hpp"
#include "testbedcore/logs/sequencelogformatter.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/utility/pauseandstophelper.hpp"

#include "managers/autoanswerpeakcanmanager.hpp"
#include "process/autoansthreadhandler.hpp"
#include "peakcanbankglobalmanager.hpp"


StopAutoAnswer::StopAutoAnswer(QObject *parent)
    : DefaultSequenceInstanceModule{staticMetaObject.className(),
                                    PeakCanBankGlobalManager::Instance(),
                                    parent}
{
}

StopAutoAnswer::~StopAutoAnswer()
{
}

bool StopAutoAnswer::initInputsFormat(JsonArray &inputsFormat) const
{
    JsonObject inputPeakCanIntf;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(
        PeakCanLibConstants::ModulesEntries::Inputs::InputPeakCanIntfName,
        inputPeakCanIntf,
        PeakCanLibConstants::ModulesEntries::Inputs::InputPeakCanIntfDesc));

    inputsFormat.append(inputPeakCanIntf);
    return true;
}

void StopAutoAnswer::processModule(const JsonArray &jsonObjectInputValues)
{
    if(!PauseAndStopHelper::managePauseAndStop(getSequenceState()))
    {
        emit moduleFinished(false, {});
        return;
    }

    if(!stopAutoAnswer(jsonObjectInputValues))
    {
        emit newLog(SequenceLogFormatter::formatABusinessProblem(tr("(TR) A problem occurred while "
                                                                    "trying to stop the auto "
                                                                    "answer process")));
        emit moduleFinished(false, {});
        return;
    }

    emit moduleFinished(true, {});
}

bool StopAutoAnswer::stopAutoAnswer(const JsonArray &jsonObjectInputValues)
{
    QString strCanBusIntf;
    PCanBusItf::Enum pCanBusIntf;
    RETURN_IF_FALSE(CanModulesUtility::extractAndParseCanBusIntf(
        jsonObjectInputValues,
        strCanBusIntf,
        pCanBusIntf));

    QSharedPointer<AutoAnsThreadHandler> process = PeakCanBankGlobalManager::Instance()->
                                                accessAutoAnswerManager().getEchoProcess(
                                                        pCanBusIntf);

    if(process == nullptr || process->isThreadNull())
    {
        qWarning() << "A problem occurred when tried to get the echo process";
        return false;
    }

    return process->accessThread()->stopEchoing();
}
