// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "threadutility/basethread.hpp"

#include "qtpeakcanlib/src/pcanapi/pcanbusitf.hpp"

class AutoAnswerProcess;
class CanMsgSeqParams;
class SequenceLogData;


/** @brief This is the thread used to receive the CAN messages and answering if necessary
    @note This is compulsory to not be stuck when trying to send messages */
class AutoAnsProcessThread : public BaseThread
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The parent instance class */
        explicit AutoAnsProcessThread(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~AutoAnsProcessThread() override;

    public:
        /** @brief Initialize the process
            @note Necessary to be called to start echoing
            @note The method is threadsafe
            @warning The method is called in another thread, it means that the event loop of the
                     caller thread is processing while the method is called.
            @note This method ensure thread uncoupling but requires an event loop
            @param canIntfKey The interface key to use in order to communication by CAN
            @param msgSeqParams The module parameters used to initialize the process
            @param manageStats True to display stats at the end of the process
            @return True if no problem occurred */
        bool initProcess(PCanBusItf::Enum canIntfKey,
                         const CanMsgSeqParams &msgSeqParams,
                         bool manageStats = true);

        /** @brief Manage the echoing process
            @note The method is threadsafe
            @warning The method is called in another thread, it means that the event loop of the
                     caller thread is processing while the method is called.
            @note This method ensure thread uncoupling but requires an event loop
            @param start True of start the process and false to stop it
            @return True if no problem occurred */
        bool manageEchoing(bool start);

        /** @brief Start the echoing process
            @note The method is threadsafe
            @warning The method is called in another thread, it means that the event loop of the
                     caller thread is processing while the method is called.
            @note This method ensure thread uncoupling but requires an event loop
            @return True if no problem occurred */
        bool startEchoing();

        /** @brief Stop the echoing process
            @note The method is threadsafe
            @warning The method is called in another thread, it means that the event loop of the
                     caller thread is processing while the method is called.
            @note This method ensure thread uncoupling but requires an event loop
            @return True if no problem occurred */
        bool stopEchoing();

        /** @brief Manage the echoing pause
            @note When we are paused, we still receive the elements but don't answer to it
            @note The method is threadsafe
            @warning The method is called in another thread, it means that the event loop of the
                     caller thread is processing while the method is called.
            @note This method ensure thread uncoupling but requires an event loop
            @param pause True to pause the echoing process
            @return True if no problem occurred */
        bool manageEchoingPause(bool pause);

    public slots:
        /** @brief Call to stop the thread
            @return True if no problem occurs */
        virtual bool stopThread() override;

    protected:
        /** @see BaseThread::run */
        virtual void run() override;

    signals:
        /** @brief Emitted when we need to send a new log to the test bed core lib
            @param log The log to send to the test bed lib */
        void newLog(const SequenceLogData &log);

    private:
        AutoAnswerProcess *_process{nullptr};
};
