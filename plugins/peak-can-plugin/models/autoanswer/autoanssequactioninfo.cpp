// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "autoanssequactioninfo.hpp"

#include "numberutility/number.hpp"
#include "stringutility/stringhelper.hpp"

#include "models/autoanswer/autoansexpectedmessage.hpp"
#include "peakcanconstants.hpp"


AutoAnsSequActionInfo::AutoAnsSequActionInfo(quint8 msgIdx, QObject *parent)
    : APCanSeqActionInfo{parent},
    _msgIdx{msgIdx}
{
}

AutoAnsSequActionInfo::~AutoAnsSequActionInfo()
{
}

const AutoAnsSequActionInfo *AutoAnsSequActionInfo::parsePCanSequenceActionInfo(
    const QString &cmd,
    bool hasBitrateSwitch,
    bool isExtendedFrameFormat)
{
    quint8 msgIdx;
    QString pureCmd;
    if(!extractMsgIdxAndCmdStr(cmd, msgIdx, pureCmd))
    {
        return nullptr;
    }

    AutoAnsSequActionInfo *action = new AutoAnsSequActionInfo(msgIdx);

    if(!action->fillFromParams(pureCmd, hasBitrateSwitch, isExtendedFrameFormat, Number()))
    {
        delete action;
        return nullptr;
    }

    return action;
}

void AutoAnsSequActionInfo::extractSequAction(int msgIdx,
                                              QVector<const AutoAnsSequActionInfo *> &allCmds,
                                              QVector<const AutoAnsSequActionInfo *> &expectedCmds)
{
    auto iter = allCmds.begin();
    while(iter != allCmds.end())
    {
        if((*iter)->getMsgIdx() == msgIdx)
        {
            expectedCmds.append(*iter);
            allCmds.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

bool AutoAnsSequActionInfo::extractMsgIdxAndCmdStr(const QString &cmdExtended,
                                                   quint8 &msgIdx,
                                                   QString &pureCmd)
{
    QStringList parametersParts = cmdExtended.split(PeakCanConstants::Parser::StrSeparator);

    if(parametersParts.length() != CmdPartsNb)
    {
        qWarning() << "The command extended parameter received: " << cmdExtended << ", has not the "
                   << "expected parts length: " << CmdPartsNb;
        return false;
    }

    bool ok = false;
    msgIdx = StringHelper::toQuint8(cmdExtended[MsgIdxPartIdx], &ok);

    if(!ok)
    {
        qWarning() << "The parameter element in the command extended: " << cmdExtended << ", with "
                   << "index: " << MsgIdxPartIdx << "is not a quint8";
        return false;
    }

    pureCmd = parametersParts.mid(PureCmdPartStartIdx, PureCmdPartEndIdx + 1 - PureCmdPartStartIdx)
                  .join(PeakCanConstants::Parser::StrSeparator);

    return true;
}
