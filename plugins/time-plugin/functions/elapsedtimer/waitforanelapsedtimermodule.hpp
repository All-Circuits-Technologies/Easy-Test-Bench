// SPDX-FileCopyrightText: 2020 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "abstractpluginelements/defaultsequenceinstancemodule.hpp"

#include <QTimer>

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"
#include "numberutility/number.hpp"

class ElapsedTimerManager;


/** @brief This class helps to manage a case: when you want to be sure that a particular time is
           really gone.
           Therefore, in a first time, we fire an elapsed timer and then do what we have to do
           In a second time, we call this module:
                * if the time we wanted to pass is gone, we go further
                * if the time we wanted to pass is not gone yet, we wait the time left before going
                    further */
class WaitForAnElapsedTimerModule : public DefaultSequenceInstanceModule
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(WaitForAnElapsedTimerModule)

    public:
        /** @brief Class constructor
            @param parent The parent class */
        Q_INVOKABLE explicit WaitForAnElapsedTimerModule(QObject *parent = nullptr);

    protected:
        /** @see DefaultSequenceInstanceModule::initParamsFormat
            @note @ref timeToWaitInMsParamName : number (Get the passed time from the elapsed
                                                         timer linked and verify that at least
                                                         this time is gone (or wait the the time
                                                         left to match this time))
                  @ref removeElapsedAtEndParamName : boolean (If true, the elapsed timer linked
                                                              is removed at the end of the
                                                              success module process) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @see DefaultSequenceInstanceModule::initInputsFormat
            @note @ref inputElapsedTimerTokenName : string (The token of the elapsed timer created
                                                            and fire) */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

        /** @see DefaultSequenceInstanceModule::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

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

        /** @brief Called at the end of the wait timer */
        void endOfTimer();

    private:
        static const constexpr char *timeToWaitInMsParamName = "timeToWait";
        static const constexpr char *timeToWaitInMsParamDesc = "Verify the elapsed timer linked "
                                                               "and wait at least this time";

        static const constexpr char *removeElapsedAtEndParamName = "removeElapsed";
        static const constexpr char *removeElapsedAtEndParamDesc = "Remove the elapsed timer at "
                                                                   "the end";

        static const constexpr char *inputElapsedTimerTokenName = "elapsedTimerToken";
        static const constexpr char *inputElapsedTimerTokenDesc = "If the elapsed timer is not "
                                                                  "already elapsed wait, the left "
                                                                  "time";

    private:
        ElapsedTimerManager &_elapsedManager;

        int _timeToWait{0};
        bool _removeElapsedAtEnd{true};

        QString _currentToken;
        QTimer _currentTimer;
};
