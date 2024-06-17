// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "abstractpluginelements/defaultsequenceinstancemodule.hpp"

#include <QTimer>

class Number;


/** @brief Base class for all the modules linked to wait operations */
class AWaitModule : public DefaultSequenceInstanceModule
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param moduleClassName The class name of the module
            @param parent The parent class */
        explicit AWaitModule(const QString &moduleClassName,
                             QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~AWaitModule() override = default;

    protected:
        /** @brief This method has to be overrides by derived classes and allow to give the time
                   to wait in Milliseconds.
            @param jsonObjectInputValues The input values got from the reference nodes linked to the
                                         module
            @param timeToWait The time to wait in millisecond, information returned by the derived
                              classes
            @return True if no problem occurs */
        virtual bool processWaiting(const JsonArray &jsonObjectInputValues,
                                    Number &timeToWaitInMs) = 0;

    protected slots:
        /** @see DefaultSequenceInstanceModule::processModule */
        virtual void processModule(const JsonArray &jsonObjectInputValues) override;

    private slots:
        /** @brief Called when the module sequence state change
            @param newState The current state
            @param previousState The previous state before the current one*/
        void onNewModuleState(SequenceStateType::Enum newState,
                              SequenceStateType::Enum previousState);

        /** @brief Called when a stop has been asked for the module */
        void onStopAsked();

        /** @brief Called when the timer raises a timeout (that's not occured when the timer is
                   stopped) */
        void onTimeout();

    private:
        QTimer _timer;
};
