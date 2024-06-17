// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "avisasetup.hpp"

#include "abstractpluginelements/defaultbankglobalmanager.hpp"
#include "qtvisacomlib/src/avisacom.hpp"
#include "testbedcore/logs/sequencelogformatter.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/testbedglobal.hpp"
#include "testbedcore/utility/pauseandstophelper.hpp"


AVisaSetup::AVisaSetup(const QString &moduleClassName,
                       QObject *parent) :
    DefaultSequenceInstanceModule(moduleClassName,
                                  DefaultBankGlobalManager::Instance(),
                                  parent)
{
}

bool AVisaSetup::initOutputsFormat(JsonArray &outputsFormat) const
{
    JsonObject interfaceId;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(outputInterfaceIdName,
                                                         interfaceId,
                                                         outputInterfaceIdDesc));

    outputsFormat.append(interfaceId);
    return true;
}

void AVisaSetup::processModule(const JsonArray &jsonObjectInputValues)
{
    Q_UNUSED(jsonObjectInputValues)

    if(!PauseAndStopHelper::managePauseAndStop(getSequenceState()))
    {
        emit moduleFinished(false, {});
        return;
    }

    _visaCom = createAndSetupItf();
    if(!_visaCom)
    {
        emit newLog(SequenceLogFormatter::formatABusinessProblem(tr("(TR) A problem occured while "
                                                                    "trying to setup a visa "
                                                                    "interface for "
                                                                    "communication")));
        emit moduleFinished(false, {});
        return;
    }

    JsonArray output;
    if(!BankJsonParser::setStringValue(outputInterfaceIdName, _visaCom->getInterfaceId(), output))
    {
        emit newLog(SequenceLogFormatter::formatABusinessProblem(tr("(TR) A problem occured while "
                                                                    "trying to set the interface "
                                                                    "id in output")));
        emit moduleFinished(false, {});
        return;
    }

    emit newLog(SequenceLogData(LogMsgType::Info,
                                SequenceLogCategory::BusinessScope,
                                "Succeeds to setup the VISA communication"));

    emit moduleFinished(true, output);
}
