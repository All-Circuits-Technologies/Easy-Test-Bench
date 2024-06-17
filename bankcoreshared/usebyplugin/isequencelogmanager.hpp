// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "logsutility/logmsgtype.hpp"
#include "testbedcore/logs/sequencelogdata.hpp"


/** @brief Interface which defines the logs manager of a sequence. This class allowes to get, from
           a plugin, the logs attached to a sequence */
class ISequenceLogManager : public QObject
{
    Q_OBJECT

    protected:
        /** @brief Class constructor
            @param parent The parent of the class */
        explicit ISequenceLogManager(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~ISequenceLogManager() override = default;

    public:
        /** @brief Get all the sequence logs stored from the building of the sequence
            @note The sequence logs are ordered from the oldest to the newest (idx 0 is the oldest)
            @param sequenceLogs The logs of the sequence currently stored, and filtered thanks to
                                the next params
            @param categoryFilter Only returns the logs linked to the sequence log categories chosen
            @param criticityLevel Only returns the logs above or equals to the criticity level given
            @param fromTimestampMSecsSinceEpoch Only returns the logs with a timestamp above or
                                                equals to the timestamp given
                                                If -1, all are returned
            @return True If no problem occurs */
        virtual bool getAllTheSequenceLogs(QVector<SequenceLogData> &sequenceLogs,
                                           SequenceLogCategory::Enums categoryfilter =
                                                            SequenceLogCategory::getAllFlags(),
                                           LogMsgType::Enum criticityLevel = LogMsgType::Debug,
                                           qint64 fromTimestampMSecsSinceEpoch = -1) const = 0;

    signals:
        /** @brief Emitted when a new log has been added to the sequence
            @param newLog The new log message and its detail */
        void newLogAdded(const SequenceLogData &newLog);
};
