// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "stopserialautoanswer.hpp"

#include "serial-link-lib/serialpluginlibconstants.hpp"
#include "serial-link-lib/utilities/serialmodulesutility.hpp"
#include "testbedcore/logs/sequencelogformatter.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/utility/pauseandstophelper.hpp"

#include "autoanswer/processes/autoansserialhandler.hpp"
#include "managers/autoanswerserialmanager.hpp"
#include "managers/seriallinkbankglobalmanager.hpp"


StopSerialAutoAnswer::StopSerialAutoAnswer(QObject *parent)
    : DefaultSequenceInstanceModule{staticMetaObject.className(),
                                    SerialLinkBankGlobalManager::Instance(),
                                    parent}
{
}

StopSerialAutoAnswer::~StopSerialAutoAnswer()
{
}

bool StopSerialAutoAnswer::initInputsFormat(JsonArray &inputsFormat) const
{
    JsonObject serialPortName;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(
        SerialPluginLibConstants::Json::PortName,
        serialPortName,
        SerialPluginLibConstants::Json::PortNameDesc));

    inputsFormat.append(serialPortName);

    return true;
}

void StopSerialAutoAnswer::processModule(const JsonArray &jsonObjectInputValues)
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

bool StopSerialAutoAnswer::stopAutoAnswer(const JsonArray &jsonObjectInputValues)
{
    QString portName;
    RETURN_IF_FALSE(SerialModulesUtility::extractSerialLink(jsonObjectInputValues, portName));

    QSharedPointer<AutoAnsSerialHandler> handler = SerialLinkBankGlobalManager::Instance()->
                                            accessAutoAnswerManager().getEchoProcess(portName);

    if(handler == nullptr || handler->isThreadNull())
    {
        qWarning() << "A problem occurred when tried to get the echo process";
        return false;
    }

    return handler->accessThread()->stopEchoing();
}
