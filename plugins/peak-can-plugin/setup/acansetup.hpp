// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "abstractpluginelements/defaultsequenceinstancemodule.hpp"

#include <QCanBusFrame>
#include <QSharedPointer>

#include "qtpeakcanlib/src/models/candeviceconfig.hpp"

#include "peakcanconstants.hpp"

class CanDeviceIntf;
class PCanIntfSetupHandle;


/** @brief This module is used to setup the CAN link (which can be used to write and read
           messages) */
class ACanSetup : public DefaultSequenceInstanceModule
{
    Q_OBJECT

    protected:
        /** @brief Class constructor
            @param moduleClassName The class name of the module
            @param hasFlexibleDataRateDefaultValue This defines the default value to use by the
                                                   module for the flexible data rate state
            @param parent The object parent */
        explicit ACanSetup(const QString &moduleClassName,
                           bool hasFlexibleDataRateDefaultValue =
                        PeakCanConstants::ModulesEntries::Params::HasFlexibleDataRateDefaultValue,
                           QObject *parent = nullptr);

    public:
        /** @brief Class destructor */
        virtual ~ACanSetup() override;

    protected:
        /** @copydoc DefaultSequenceInstanceModule::initParamsFormat
            @note @ref BusOffAutoResetParamName: boolean (This parameter instructs the PCAN driver
                                                          to automatically reset the CAN controller
                                                          of a PCAN Channel when a bus-off state is
                                                          detected.) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @brief Access the device config linked to the setup */
        CanDeviceConfig &accessDeviceConfig() { return _deviceConfig; }

        /** @brief This method is useful to add the wanted parameters
            @note If the parameter isn't added, the default value given in the constructor will be
                  used
            @note @ref FlexibleDataRateParamName : bool (Says if the CAN connection supports the
                                                         flexible data rate)
            @param paramsFormat The parameters format to fill with the wanted format
            @param addFlexibleDataRateParam Add the flexible data rate format in the module params
                                            format
            @return True if no problem occurred */
        bool addCommonParamsFormat(JsonArray &paramsFormat,
                                   bool addFlexibleDataRateParam = true) const;

    protected:
        /** @copydoc DefaultSequenceInstanceModule::initInputsFormat
            @note @ref InputPeakCanIntfName : string (The input contains the name of the peak CAN
                                                      interface) */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

        /** @copydoc DefaultSequenceInstanceModule::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

    protected slots:
        /** @brief Process the setup of the PEAK CAN channel
            @copydoc DefaultSequenceInstanceModule::processModule */
        virtual void processModule(const JsonArray &jsonObjectInputValues) override;

    private:
        /** @brief Do setup the CAN connection
            @return True if no problem occurred  */
        bool setupCan(const JsonArray &jsonObjectInputValues);

    private:
        static const constexpr char *FlexibleDataRateParamName = "hasFlexibleDataRate";
        static const constexpr char *FlexibleDataRateParamDesc = "Says if the CAN connection "
                                                                 "supports the flexible data rate";

        static const constexpr char *BusOffAutoResetParamName = "busOffAutoReset";
        static const constexpr char *BusOffAutoResetParamDesc = "This parameter instructs the PCAN "
                                                                "driver to automatically reset the "
                                                                "CAN controller of a PCAN Channel "
                                                                "when a bus-off state is detected.";
        static const constexpr bool DefaultBusOffAutoResetParam = false;

    private:
        QSharedPointer<CanDeviceIntf> _canIntf;

        CanDeviceConfig _deviceConfig;

        bool _busOffAutoResetParam{DefaultBusOffAutoResetParam};
};
