// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "testbedcore/types/sequencestatetype.hpp"
#include "testbedcore/types/stopaskedtype.hpp"


/** @brief Helper class to manage the pause and/or stop of a sequence */
class SequenceStateHelper : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The parent class */
        explicit SequenceStateHelper(QObject *parent = nullptr);

    public:
        /** @brief Get the current state of the module */
        SequenceStateType::Enum getCurrentState() const { return _currentState; }

        /** @brief Get the type of stop asked */
        StopAskedType::Enum getStopAskedType() const { return _stopAskedType; }

        /** @brief Test if a pause has been asked */
        const bool &isPauseAsked() const { return _pauseAsked; }

        /** @brief Test if the sequence is currently running */
        bool isRunning() const { return (_currentState == SequenceStateType::Running); }

        /** @brief Test if the sequence is currently stopped */
        bool isStopped() const { return (_currentState == SequenceStateType::Stopped); }

    public slots:
        /** @brief Call to notify that the sequence is running */
        void onRunning();

        /** @brief Call to notify that the sequence is in pause or is running again */
        void onPauseOrResume(bool setToPause);

        /** @brief Call to notify that the sequence has been asked to stop
            @param stopAskedType Give a contact to the stop asking */
        void onStopAsked(StopAskedType::Enum stopAskedType);

        /** @brief Call to notify that the sequence is stopped */
        void onStop();

    public:
        /** @brief Register the meta type linked to the sequence state */
        static void registerMetaTypes();

    private:
        /** @brief Set the current state of module
            @note If the current and the new states are differents, emit the signal:
                  @ref ISequenceInstanceModule::newState
            @param state The new state to set */
        void setCurrentState(SequenceStateType::Enum state);

    signals:
        /** @brief Emitted when the sequence module changes its state
            @param newState The current sequence state
            @param previousState The previous state before the current one */
        void newState(SequenceStateType::Enum newState, SequenceStateType::Enum previousState);

        /** @brief Emitted when the sequence has been asked to stop */
        void stopAsked(StopAskedType::Enum stopAskedType);

    private:
        SequenceStateType::Enum _currentState{SequenceStateType::Stopped};

        StopAskedType::Enum _stopAskedType{StopAskedType::NoStopAsked};
        bool _pauseAsked{false};
};
