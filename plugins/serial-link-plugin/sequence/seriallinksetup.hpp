// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2020 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "abstractpluginelements/defaultsequenceinstancemodule.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"
#include "serial-link-lib/serialpluginlibconstants.hpp"

#include <QSharedPointer>

class SerialLinkIntf;

/** @brief This sequence abstract module let user setup/initialize a serial port.

    @note Setup options may increase in the future.
          Minimal set is baudrate (first), then flow control in my opinion.

    @attention Initializing a serial port is mandatory before using it. */
class SerialLinkSetup : public DefaultSequenceInstanceModule
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(SerialLinkSetup)

    public:
        /** @brief Class constructor
            @param parent The parent class */
        Q_INVOKABLE explicit SerialLinkSetup(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~SerialLinkSetup() override;

    public:
        /** @see DefaultSequenceInstanceModule::initParamsFormat
            @note @ref SerialPluginLibConstants::Json::baudrate : string (wanted baudrate (9600,
                                                                          115200, etc)
                                                                          [default=9600])
                  @ref SerialPluginLibConstants::Json::swFlowCtl : boolean (wanted software flow
                                                                            control (XON/XOFF)
                                                                            [default=off])
                  @ref SerialPluginLibConstants::Json::hwFlowCtl : boolean (wanted hardware flow
                                                                            control (RTS/CTS)
                                                                            [default=off]) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @see DefaultSequenceInstanceModule::initInputsFormat
            @note @ref SerialPluginLibConstants::Json::portName : string (serial port name to create
                                                                          or setup (ex "COM5")) */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

        /** @see DefaultSequenceInstanceModule::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

    protected slots:
        /** @brief Execute the task
            @copydoc AbstractSerialLinkModule::processModule */
        virtual void processModule(const JsonArray &jsonObjectInputValues) override;

    private:
        /** @brief Setup the link thanks to the input data given
            @param jsonObjectInputValues The input values to parse in order to setup the link
            @return True if no problem occurred */
        bool setupLink(const JsonArray &jsonObjectInputValues);

    private:
        int _baudrate {SerialPluginLibConstants::Defaults::Baudrate};
        bool _swFlowCtl {SerialPluginLibConstants::Defaults::SwFlowCtl};
        bool _hwFlowCtl {SerialPluginLibConstants::Defaults::HwFlowCtl};

        QSharedPointer<SerialLinkIntf> _serialLink;
};
