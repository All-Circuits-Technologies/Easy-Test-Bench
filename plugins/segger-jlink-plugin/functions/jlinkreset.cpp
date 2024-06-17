// SPDX-FileCopyrightText: 2020 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "jlinkreset.hpp"

#include "abstractpluginelements/defaultbankglobalmanager.hpp"
#include "bankcoreshared/usebyplugin/icommoninterface.hpp"
#include "hmi-helper-lib/hmidisplayinfinitewaitview.hpp"
#include "testbedcore/logs/sequencelogformatter.hpp"


JLinkReset::JLinkReset(QObject *parent) :
    AJLinkIntfFromParams(staticMetaObject.className(), parent)
{
}

JLinkReset::~JLinkReset()
{
}

bool JLinkReset::mainJLinkProcess(const JsonArray &/*jsonObjectInputValues*/,
                                  const QString &jLinkPgmPath,
                                  JsonArray &/*output*/)
{
    if(!operate(jLinkPgmPath))
    {
        emit newLog(SequenceLogFormatter::formatABusinessProblem(tr("(TR) A problem occured while "
                                                                    "reseting the board")));
        return false;
    }

    return true;
}

bool JLinkReset::operate(const QString &jLinkPgmPath)
{
    ICommonInterface &comItf = DefaultBankGlobalManager::Instance()->accessCommonInterface();

    HmiDisplayInfiniteWaitView displayHmi(comItf);
    RETURN_IF_FALSE(displayHmi.displayInfiniteHmi(hmiName, tr(hmiDescription), false));

    QStringList commands = { JLinkCmdEoeEnable,      // Activate the exit on error
                            JLinkCmdConnect,        // Connect to target
                            JLinkCmdResetAndHalts,
                            JLinkCmdCloseAndQuit };
    
    return callCmdsProcess(commands, jLinkPgmPath, getTimeoutInMs());
}
