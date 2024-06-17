// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "bankcoreshared/usebyplugin/isequencelogmanager.hpp"


/** @brief Store all the logs attached to a specific sequence, the class can be called by plugins
           in order to get the stored logs */
class SequenceLogManager : public ISequenceLogManager
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The class parent */
        explicit SequenceLogManager(QObject *parent = nullptr);

    public:
        /** @see ISequenceLogManager::getAllTheSequenceLogs */
        virtual bool getAllTheSequenceLogs(QVector<SequenceLogData> &sequenceLogs,
                                           SequenceLogCategory::Enums categoryfilter =
                                                                SequenceLogCategory::getAllFlags(),
                                           LogMsgType::Enum criticityLevel = LogMsgType::Debug,
                                           qint64 fromTimestampMSecsSinceEpoch = -1) const override;

    public slots:
        /** @brief Call to store the log
            @note The log is inserted in the list in a way to respect the list order
            @param newLog The log to store
            @return True If no problem occurs */
        bool storeLog(const SequenceLogData &newLog);

        /** @brief Clear and remove all the stored logs */
        void clearLogs();

    private:
        QVector<SequenceLogData> _sequenceLogs;
};
