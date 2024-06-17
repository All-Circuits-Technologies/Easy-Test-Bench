// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "functions/awaitmodule.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief Allow to launch a timer in order to wait some time, the time to wait is got from a
           reference node */
class RefEntryWaitModule : public AWaitModule
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(RefEntryWaitModule)

    public:
        /** @brief Class constructor
            @param parent The parent class */
        Q_INVOKABLE explicit RefEntryWaitModule(QObject *parent = nullptr);

    protected:
        /** @see AWaitModule::initInputsFormat
            @note @ref inputTimeToWaitInMsName : number (This is the time to wait in Ms) */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

        /** @see AWaitModule::processWaiting */
        virtual bool processWaiting(const JsonArray &jsonObjectInputValues,
                                    Number &timeToWaitInMs) override;

    private:
        static const constexpr char * inputTimeToWaitInMsName = "timeToWaitInMs";
        static const constexpr char * inputTimeToWaitInMsDesc = "This is the time to wait in Ms";
};
