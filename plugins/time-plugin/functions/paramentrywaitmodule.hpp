// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "functions/awaitmodule.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"
#include "numberutility/number.hpp"


/** @brief Allow to launch a timer in order to wait some time, the time to wait is got from a
           parameter */
class ParamEntryWaitModule : public AWaitModule
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(ParamEntryWaitModule)

    public:
        /** @brief Class constructor
            @param parent The parent class */
        Q_INVOKABLE explicit ParamEntryWaitModule(QObject *parent = nullptr);

    protected:
        /** @see AWaitModule::initParamsFormat
            @note @ref timeToWaitParamName : number (Time to wait in milliseconds) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @see AWaitModule::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

        /** @see AWaitModule::processWaiting */
        virtual bool processWaiting(const JsonArray &jsonObjectInputValues,
                                    Number &timeToWaitInMs) override;

    private:
        static const constexpr char * timeToWaitParamName = "timeToWaitInMs";
        static const constexpr char * timeToWaitParamDesc = "Time to wait in milliseconds";

    private:
        Number _timeToWait;
};
