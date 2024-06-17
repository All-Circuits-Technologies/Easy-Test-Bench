// SPDX-FileCopyrightText: 2020 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "jlinkerase.hpp"

#include "abstractpluginelements/defaultbankglobalmanager.hpp"
#include "bankcoreshared/usebyplugin/icommoninterface.hpp"
#include "hmi-helper-lib/hmidisplayinfinitewaitview.hpp"
#include "testbedcore/logs/sequencelogformatter.hpp"
#include "testbedcore/testbedglobal.hpp"


JLinkErase::JLinkErase(QObject *parent) :
    AJLinkIntfFromParams(staticMetaObject.className(), parent)
{
}

JLinkErase::~JLinkErase()
{
}

bool JLinkErase::mainJLinkProcess(const JsonArray &/*jsonObjectInputValues*/,
                                  const QString &jLinkPgmPath,
                                  JsonArray &/*output*/)
{
    emit newLog(SequenceLogData(LogMsgType::Info,
                                SequenceLogCategory::BusinessScope,
                                "Erase the board"));

    if(!operate(jLinkPgmPath))
    {
        emit newLog(SequenceLogFormatter::formatABusinessProblem(tr("(TR) A problem occured while "
                                                                    "erasing the board")));
        return false;
    }

    emit newLog(SequenceLogData(LogMsgType::Info,
                                SequenceLogCategory::BusinessScope,
                                "The board has been erased"));
    return true;
}

bool JLinkErase::operate(const QString &jLinkPgmPath)
{
    ICommonInterface &comItf = DefaultBankGlobalManager::Instance()->accessCommonInterface();

    HmiDisplayInfiniteWaitView displayHmi(comItf);
    RETURN_IF_FALSE(displayHmi.displayInfiniteHmi(HmiName, tr(HmiDescription), false));

    QStringList commands = { JLinkCmdEoeEnable,      // Activate the exit on error
                             JLinkCmdConnect,        // Connect to target
                             JLinkCmdResetAndHalts,
                             JLinkCmdErase,
                             JLinkCmdResetAndHalts,
                             JLinkCmdCloseAndQuit };
    
    return callCmdsProcess(commands, jLinkPgmPath, getTimeoutInMs());
}
