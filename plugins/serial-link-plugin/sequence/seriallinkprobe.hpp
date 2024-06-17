// SPDX-FileCopyrightText: 2019 - 2020 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2020 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "abstractpluginelements/defaultsequenceinstancemodule.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"
#include "serial-link-lib/serialpluginlibconstants.hpp"

class QSerialPortInfo;


/** @brief This sequence module helps detecting a serial port.

    @note You can test availability of a serial port given a set of search rules, and you receive a
          boolean telling you if such serial port is plugged as well as many details about this
          serial port.

    @note A serial port not found is not always an error (see parameter mandatory:bool).
          The "found" boolean output will be false in this case. */
class SerialLinkProbe : public DefaultSequenceInstanceModule
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(SerialLinkProbe)

    public:
        /** @brief Class constructor
            @param parent The parent instance */
        Q_INVOKABLE explicit SerialLinkProbe(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~SerialLinkProbe() override;

    protected slots:
        /** @brief Execute the task
            @copydoc AbstractSerialLinkModule::processModule */
        virtual void processModule(const JsonArray &jsonObjectInputValues) override;

    protected:
        /** @see DefaultSequenceInstanceModule::initParamsFormat
            @note @ref SerialPluginLibConstants::Json::mandatory : boolean (should process die if
                                                                            not found (default to
                                                                            yes))
                  @ref SerialPluginLibConstants::Json::portName : string (serial port name to find
                                                                          (ex "COM5"))

                  @ref SerialPluginLibConstants::Json::serialNumber : string (USB serial number to
                                                                              find)
                  @ref SerialPluginLibConstants::Json::usbVid : number (USB Vendor ID (0x0000 to
                                                                        not filter on this))
                  @ref SerialPluginLibConstants::Json::usbPid : number (USB Product ID (0x0000 to
                                                                        not filter on this))
                  @ref SerialPluginLibConstants::Json::description : string (USB description)
                  @ref SerialPluginLibConstants::Json::manufacturer : string (USB manufacturer)
                  @ref SerialPluginLibConstants::Json::location : string (system location) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @see DefaultSequenceInstanceModule::initOutputsFormat
            @note @ref SerialPluginLibConstants::Json::found : boolean (is serial found)
                  @ref SerialPluginLibConstants::Json::portName : string (serial port name to find
                                                                          (ex "COM5"))

                  @ref SerialPluginLibConstants::Json::serialNumber : string (USB serial number to
                                                                              find)
                  @ref SerialPluginLibConstants::Json::usbVid : number (USB Vendor ID (0x0000 to
                                                                        not filter on this))
                  @ref SerialPluginLibConstants::Json::usbPid : number (USB Product ID (0x0000 to
                                                                        not filter on this))
                  @ref SerialPluginLibConstants::Json::description : string (USB description)
                  @ref SerialPluginLibConstants::Json::manufacturer : string (USB manufacturer)
                  @ref SerialPluginLibConstants::Json::location : string (system location) */
        virtual bool initOutputsFormat(JsonArray &outputsFormat) const override;

        /** @see DefaultSequenceInstanceModule::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

    private:
        /** @brief Update the plugin output with the serial port information got
            @param portInfo The serial port information to get and fill output object
            @param output The output to fill with serial port info
            @return True if no problem occurred */
        bool updateOutput(const QSerialPortInfo &portInfo, bool found, JsonArray &output);

    private:
        static constexpr const bool DefaultMandatoryParam = true;
        static constexpr const quint16 DefaultUsbVid = 0;
        static constexpr const quint16 DefaultUsbPid = 0;

    private:
        bool _mandatory {DefaultMandatoryParam};
        QString _name;
        QString _serialNumber;
        quint16 _usbVid {DefaultUsbVid};
        quint16 _usbPid {DefaultUsbPid};
        QString _description;
        QString _manufacturer;
        QString _location;

        quint16 _nthElement{SerialPluginLibConstants::Defaults::NthElement};
};
