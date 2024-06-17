// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QSharedPointer>

#include "statisticsutility/mixins/mixinprocessstats.hpp"

class AutoAnsMsgToSend;
class AutoAnswerParams;
class QElapsedTimer;
class MessageReceptionProcess;
class SequenceLogData;
class SerialLinkIntf;


/** @brief This is the process linked to the auto answer with serial link */
class AutoAnsSerialLinkProcess : public QObject, public MixinProcessStats
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The parent instance */
        explicit AutoAnsSerialLinkProcess(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~AutoAnsSerialLinkProcess() override;

    public:
        /** @brief Initialize the process
            @param interfaceName The name of the serial port to manage with
            @param params The module parameters to use */
        bool initProcess(const QString &interfaceName,
                         const AutoAnswerParams *params);

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
            @note When pause, we keep receiving the messages but we do nothing about them
            @param pause True to activate the pause, false to resume */
        void manageEchoingPause(bool pause) { _pause = pause; }

    private slots:
        /** @brief Called when a new data is received from the linked serial link
            @param message The data received */
        void onMessageReceived(const QByteArray &message);

    private:
        /** @brief Update the statistics linked to the auto answer process
            @note The method increments the linked counter by one
            @param msg The message key to use to identify the statistic
            @param stats The categories to update with the statistics received */
        void updateMsgStats(const QString& msg, CounterStatsCategory *stats);

        /** @brief Send the given messages
            @note In this method we expect that @ref _serialIntf and @ref _autoAnsParams are not
                  null
            @param msgReceptElapsTimer This is the elapsed timer started from the message reception
                                       This is useful to calculate the time to wait between
                                       reception and sending.
            @param msgsToSend The messages to send
            @return True if no problem occurred */
        bool sendMsgsLinkedToExpectedMsgs(const QElapsedTimer &msgReceptElapsTimer,
                                          QVector<AutoAnsMsgToSend *> &msgsToSend);

        /** @brief Manage the opening of serial port
            @return True if no problem occurred */
        bool openSerialPort();

        /** @brief Manage the closing of serial port */
        void closeSerialPort();

        /** @brief Clear the received lines after having tests them on reception.
            @note This method removes lines which don't need to be tested anymore */
        void clearReceivedLinesAfterMessageReception();

    signals:
        /** @brief Emitted when a new log has to be emitted by the linked sequence module */
        void newLog(const SequenceLogData &log);

    private:
        static const constexpr char *StatsGlobalInfo = "Automatic command answer statistics";
        static const constexpr char *SentStatsCounterKey = "msgSendCounter";
        static const constexpr char *SentStatsTitle = "Message sent";
        static const constexpr char *ReceivedStatsTitle = "Message received";
        static const constexpr char *ReceivedStatsCounterKey = "msgReceivedCounter";

    private:
        QSharedPointer<SerialLinkIntf> _serialIntf;
        QMetaObject::Connection _connect;

        bool _pause{false};
        bool _serialPortOpenedByMe{false};

        const AutoAnswerParams *_autoAnsParams{nullptr};
        const MessageReceptionProcess *_receptionProcess{nullptr};

        QString _tmpBuffer;
        QStringList _receivedLines;

        /** @brief This is a pointer to the command sent statistics
            @note The class doesn't own the instance but the mixin */
        CounterStatsCategory *_msgSentStats{nullptr};

        /** @brief This is a pointer to the command received statistics
            @note The class doesn't own the instance but the mixin */
        CounterStatsCategory *_msgReceivedStats{nullptr};
};
