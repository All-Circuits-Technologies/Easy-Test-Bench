// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QSharedPointer>

#include "qtpeakcanlib/src/pcanapi/pcanbusitf.hpp"
#include "statisticsutility/mixins/mixinprocessstats.hpp"

class AutoAnsExpectedMessage;
class AutoAnsSequenceInfo;
class CanDeviceIntf;
class CanMsgSeqParams;
class QCanBusFrame;
class SequenceLogData;


/** @brief The auto answer process */
class AutoAnswerProcess : public QObject, public MixinProcessStats
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The class parent instance */
        explicit AutoAnswerProcess(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~AutoAnswerProcess() override;

    public:
        /** @brief Initialize the process
            @note Necessary to be called to start echoing
            @param canIntfKey The interface key to use in order to communication by CAN
            @param msgSeqParams The module parameters used to initialize the process
            @param manageStats True to display stats at the end of the process
            @return True if no problem occurred */
        bool initProcess(PCanBusItf::Enum canIntfKey,
                         const CanMsgSeqParams &msgSeqParams,
                         bool manageStats = true);

        /** @brief Manage the echoing process
            @param start True of start the process and false to stop it
            @return True if no problem occurred */
        bool manageEchoing(bool start);

        /** @brief Start the echoing process
            @return True if no problem occurred */
        bool startEchoing();

        /** @brief Stop the echoing process
            @return True if no problem occurred */
        bool stopEchoing();

        /** @brief Manage the echoing pause
            @param pause True to pause the echoing process
            @return True if no problem occurred */
        void manageEchoingPause(bool pause) { _pause = pause; }

    private slots:
        /** @brief Called when new CAN frames are received
            @param frames The received frames */
        void onFramesReceived(const QVector<QCanBusFrame> &frames);

    private:
        /** @brief Call to update the statistics linked to auto answer process
            @note If the user has activated the statistics the method update the given @ref stats
                  param
            @param cmd The cmd to increment in stats
            @param stats The statistics information to update */
        void updateCmdStats(const QCanBusFrame& cmd, CounterStatsCategory *stats);

        /** @brief Emit the statistics informations to logs, if the user has enabled the
                   statistics */
        void emitStatsLogs();

        /** @brief Send the commands linked to the expected messages
            @param expectedMsgs The expected messages to get their linked commands and send them
            @return True if no problem occurred */
        bool sendCmdsLinkedToExpectedMsgs(
            const QVector<const AutoAnsExpectedMessage*> &expectedMsgs);

    signals:
        /** @brief Emitted when we need to send a new log to the test bed core lib
            @param log The log to send to the test bed lib */
        void newLog(const SequenceLogData &log);

    private:
        static const constexpr char *StatsGlobalInfo = "Automatic command answer statistics";
        static const constexpr char *SentStatsCounterKey = "cmdSendCounter";
        static const constexpr char *SentStatsTitle = "Command sent";
        static const constexpr char *ReceivedStatsTitle = "Command received";
        static const constexpr char *ReceivedStatsCounterKey = "cmdReceivedCounter";

    private:
        QSharedPointer<CanDeviceIntf> _canIntf;
        QMetaObject::Connection _connect;

        bool _pause{false};

        const AutoAnsSequenceInfo *_sequenceInfo{nullptr};

        /** @brief This is a pointer to the command sent statistics
            @note The class doesn't own the instance but the mixin */
        CounterStatsCategory *_cmdSentStats{nullptr};

        /** @brief This is a pointer to the command received statistics
            @note The class doesn't own the instance but the mixin */
        CounterStatsCategory *_cmdReceivedStats{nullptr};
};
