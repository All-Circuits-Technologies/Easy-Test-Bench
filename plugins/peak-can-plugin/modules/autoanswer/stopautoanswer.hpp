// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "abstractpluginelements/defaultsequenceinstancemodule.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief This is the stop auto answer module. This module allows to stop the automatic
           answering process in background.
    @note The process can be started by calling the @see StartAutoAnswer module */
class StopAutoAnswer : public DefaultSequenceInstanceModule
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(StopAutoAnswer)

    public:
        /** @brief Class constructor
            @param parent The parent instance class */
        Q_INVOKABLE explicit StopAutoAnswer(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~StopAutoAnswer() override;

    protected:
        /** @copydoc DefaultSequenceInstanceModule::initInputsFormat
            @note @ref PeakCanLibConstants::ModulesEntries::Inputs::InputPeakCanIntfName : string
                                            (The input contains the name of the peak CAN
                                             interface) */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

    protected slots:
        /** @brief Execute the task
            @copydoc DefaultSequenceInstanceModule::processModule */
        virtual void processModule(const JsonArray &jsonObjectInputValues) override;

    private:
        /** @brief Called to stop the auto answer process
            @param jsonObjectInputValues The module input values
            @return True if no problem occurred */
        bool stopAutoAnswer(const JsonArray &jsonObjectInputValues);
};
