// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "threadutility/basethread.hpp"

#include <QObject>

class AutoAnsSerialLinkProcess;
class AutoAnswerParams;
class SequenceLogData;


/** @brief This thread manages the auto answering process linked to a specific serial link */
class AutoAnswerSerialLinkThread : public BaseThread
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The parent instance class */
        explicit AutoAnswerSerialLinkThread(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~AutoAnswerSerialLinkThread() override;

    public:
        /** @brief Initialize the process
            @note Necessary to be called to start echoing
            @note The method is threadsafe
            @warning The method is called in another thread, it means that the event loop of the
                     caller thread is processing while the method is called.
            @note This method ensure thread uncoupling but requires an event loop
            @param portName The name of the serial port to manage with
            @param autoAnsParams The module parameters to use */
        bool initProcess(const QString &portName,
                         const AutoAnswerParams *autoAnsParams);

        /** @brief Manage the echoing process
            @note Necessary to be called to start echoing
            @note The method is threadsafe
            @warning The method is called in another thread, it means that the event loop of the
                     caller thread is processing while the method is called.
            @note This method ensure thread uncoupling but requires an event loop
            @param start True of start the process and false to stop it
            @return True if no problem occurred */
        bool manageEchoing(bool start);

        /** @brief Start the echoing process
            @note Necessary to be called to start echoing
            @note The method is threadsafe
            @warning The method is called in another thread, it means that the event loop of the
                     caller thread is processing while the method is called.
            @note This method ensure thread uncoupling but requires an event loop
            @return True if no problem occurred */
        bool startEchoing();

        /** @brief Stop the echoing process
            @note Necessary to be called to start echoing
            @note The method is threadsafe
            @warning The method is called in another thread, it means that the event loop of the
                     caller thread is processing while the method is called.
            @note This method ensure thread uncoupling but requires an event loop
            @return True if no problem occurred */
        bool stopEchoing();

        /** @brief Manage the echoing pause
            @note When pause, we keep receiving the messages but we do nothing about them
            @note Necessary to be called to start echoing
            @note The method is threadsafe
            @warning The method is called in another thread, it means that the event loop of the
                     caller thread is processing while the method is called.
            @note This method ensure thread uncoupling but requires an event loop
            @param pause True to activate the pause, false to resume */
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
        AutoAnsSerialLinkProcess *_process{nullptr};
};
