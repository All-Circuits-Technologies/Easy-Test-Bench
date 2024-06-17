// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "abstractpluginelements/defaultsequenceinstancemodule.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief This module stops the automatic answer linked to a particular serial link.
    @note This module has to be combined with the @ref StartSerialAutoAnswer module */
class StopSerialAutoAnswer : public DefaultSequenceInstanceModule
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(StopSerialAutoAnswer)

    public:
        /** @brief Class constructor
            @param parent The parent instance class */
        Q_INVOKABLE explicit StopSerialAutoAnswer(QObject *parent = nullptr);

        /** @brief The class destructor */
        virtual ~StopSerialAutoAnswer() override;

    protected:
        /** @copydoc DefaultSequenceInstanceModule::initInputsFormat
            @note @ref SerialPluginLibConstants::Json::PortName : string (Serial port name
                                                                          (ex: COM1)) */
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
