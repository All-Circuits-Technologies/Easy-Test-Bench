// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "sequencelogmanager.hpp"

#include <QDebug>
#include <QVector>


SequenceLogManager::SequenceLogManager(QObject *parent) :
    ISequenceLogManager(parent)
{
}

bool SequenceLogManager::getAllTheSequenceLogs(QVector<SequenceLogData> &sequenceLogs,
                                               SequenceLogCategory::Enums categoryfilter,
                                               LogMsgType::Enum criticityLevel,
                                               qint64 fromTimestampMSecsSinceEpoch) const
{
    // If filters are disabled, return all the values
    if(categoryfilter == SequenceLogCategory::getAllFlags() &&
       criticityLevel == LogMsgType::Debug &&
       fromTimestampMSecsSinceEpoch == -1)
    {
        sequenceLogs = _sequenceLogs;
        return true;
    }

    sequenceLogs.clear();

    QVector<SequenceLogData>::const_iterator citer = _sequenceLogs.cbegin();

    for(; citer != _sequenceLogs.cend(); ++citer)
    {
        if(citer->getTimestampMSecsSinceEpoch() >= fromTimestampMSecsSinceEpoch         &&
           LogMsgType::isEqualOrAboveCriticity(citer->getType(), criticityLevel)   &&
           categoryfilter.testFlag(citer->getCategory()))
        {
            sequenceLogs.append(*citer);
        }
    }

    return true;
}

bool SequenceLogManager::storeLog(const SequenceLogData &newLog)
{
    if(!newLog.isValid())
    {
        qWarning() << "Can't store an invalid log";
        return false;
    }

    QVector<SequenceLogData>::iterator iter = _sequenceLogs.end();

    while(iter != _sequenceLogs.begin())
    {
        // End points to an imaginary item after the last item in the vector
        if(newLog.getTimestampMSecsSinceEpoch() > (iter - 1)->getTimestampMSecsSinceEpoch())
        {
            break;
        }

        --iter;
    }

    _sequenceLogs.insert(iter, newLog);

    emit newLogAdded(newLog);

    return true;
}

void SequenceLogManager::clearLogs()
{
    _sequenceLogs.clear();
}
