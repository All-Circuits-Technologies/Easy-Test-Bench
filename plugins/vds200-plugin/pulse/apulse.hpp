// SPDX-FileCopyrightText: 2021 - 2022 Damien Manceau <damien.manceau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "abstractpluginelements/defaultsequenceinstancemodule.hpp"

#include "vds200-plugin/pulse/pulseflag.hpp"

#include "numberutility/number.hpp"

class APulseCmd;
class AResponse;
class WriteProcess;
class StartCmd;
class StopCmd;


/** @brief This module is helpful to setup & generate pulse*/
class APulse : public DefaultSequenceInstanceModule
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parametersFlag The parameter pulse flags
            @param moduleClassName The module class name
            @param parent The parent instance class */
        explicit APulse(const QVector<PulseFlag::Flag> &parametersFlag,
                        const QString &moduleClassName,
                        const QString &pulseName,
                        QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~APulse() override;

    protected:
        /** @see DefaultSequenceInstanceModule::initParamsFormat */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @see DefaultSequenceInstanceModule::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

        /** @see DefaultSequenceInstanceModule::initInputsFormat
            @note @ref inputInterfaceIdName : string (The unique if of the interface to use) */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

        /** @brief Set the instance of specific pulse to the APulse Class
            @note This function allow to use APulseCmd parameters with spécific pulse parameters */
        void setAPulseCmd(APulseCmd *aPulseCmd) { _aPulseCmd = aPulseCmd; };

        /** @brief Use to setup the pulse with sequence file parameters
            @note This function will format parameters as cmd & send this cmd to the VDS200
            @return return a boolean value. if false => An error occured */
        bool setupPulse();

        /** @brief Send a command to start pulse
            @return return a boolean value. if false => An error occured */
        bool startPulse();

        /** @brief Send a command to stop pulse & terminate the visa communication
            @return return a boolean value. if false => An error occured */
        bool terminatePulse();

        /** @brief Send a command to identify the instrument in use
            @note This function send a command in order to receive response from the instrument
            @return return a boolean value. if false => An error occured */
        bool askInstrumentId();

    private:
        /** @brief Manage the trigger enum selection from sequence file
            @note This function allow to choose between auto / manual trigger
            @param parameters The format of the module parameters, which is possible to set
            @param paramName The name of the object searched
            @param errors The array to store errors
            @param trigger The enum selected in file sequence converted to quint16
            @return return a boolean value. if false => An error occured */
        bool manageTriggerSelection(const JsonArray &parameters,
                                    const char *paramName,
                                    JsonArray &errors,
                                    quint16 &trigger);

        /** @brief Manage the object connection between the writeProcess and @ref Apulse
            @param toConnect True to connect, false to disconnect the signals/slots */
        void manageConnection(bool toConnect);

        /** @brief Check if the response received is the correct id response
            @param response The AResponse received from the writeProcess*/
        bool checkIfIdn(const QSharedPointer<AResponse>& response);

        /** @brief Check if the response received is an information response from the vds200
            @param response The AResponse received from the writeProcess*/
        bool checkIfInfo(const QSharedPointer<AResponse> &response);

        /** @brief Manage Number parameters from sequence file
            @note This function abstract integer | double | float to use it as simple number
            @param paramName The name of the object
            @param paramDesc The description of the object
            @param min The minimum value that should not be exeeded in sequence file
            @param max The maximum value that
            @param step The step value
            @param paramsFormat The parameters list to fill with the given parameter
            @return return a boolean value. if false => An error occured */
        bool initNumberParams(const QString &paramName,
                              const QString &paramDesc,
                              const Number &min,
                              const Number &max,
                              const Number &step,
                              JsonArray &paramsFormat) const;

        /** @brief Manage flag parameter from sequence file
            @note This function abstract integer | double | float to use it as simple number
            @param paramFlag The flag parameter to test if needed
            @param paramName The name of the object
            @param paramDesc The description of the object
            @param parameters The format of the module parameters, which is possible to set
            @param min The minimum value that should not be exeeded in sequence file
            @param max The maximum value that should not be exeeded in sequence file
            @param step The step value
            @param paramsFormat The parameters list to fill with the given parameter
            @return return a boolean value. if false => An error occured */
        bool initNbParamsWithTestFlags(PulseFlag::Flag paramFlag,
                                       const QString &paramName,
                                       const QString &paramDesc,
                                       const Number &min,
                                       const Number &max,
                                       const Number &step,
                                       JsonArray &paramsFormat) const;

        /** @brief Validate and save the flag parameter in the pulse cmd
            @param parameters The parameters set by sequence file to parse
            @param paramFlag The param flag to deal with
            @param paramName The parameter name to get
            @param setter The method to call in order to set the parameter value to Pulse command
            @param errors The array to store errors
            @param unitFactor The factor to use on the parameter value after having adding the
                              offset and before saving the value
                              If invalid, nothing is done
            @param offset The offset to add to the value before applying the unit factor and saving
                          the value
                          If invalid, nothing is done
            @return return a boolean value. if false => An error occured */
        bool validateAndSetFlagParam(const JsonArray &parameters,
                                     PulseFlag::Flag paramFlag,
                                     const QString &paramName,
                                     void (APulseCmd::*setter)(quint16),
                                     JsonArray &errors,
                                     const Number &unitFactor = Number(),
                                     const Number &offset = Number());

    protected slots:
        /** @see DefaultSequenceInstanceModule::processModule */
        virtual void processModule(const JsonArray &jsonObjectInputValues) override;

        /** @brief Called when a signal response is received from the instrument
            @param reponse the qSharedPointer of AResponse instance created in writeProcess*/
        void onResponseReceived(const QSharedPointer<AResponse> &response);

        /** @brief Called when a signal error is received from the instrument */
        void onError();

    private:
        /** @brief Used to symbolize the current step of the process */
        enum ProcessStep {
            AskForIdn,
            SetupPulse,
            StartPulse
        };

    private:
        //Inputs:
        static const constexpr char *InputInterfaceIdName = "interfaceId";
        static const constexpr char *InputInterfaceIdDesc = "The unique if of the interface to use";

        //Parameters:
        static const constexpr char *VbParamName = "vb";
        static const constexpr char *VbParamDesc = "The start voltage level";

        static const constexpr char *VsParamName = "vs";
        static const constexpr char *VsParamDesc = "The voltage variation vs";

        static const constexpr char *VaParamName = "va";
        static const constexpr char *VaParamDesc = "The voltage variation va";

        static const constexpr char *VpParamName = "vp";
        static const constexpr char *VpParamDesc = "The voltage amplitude vp";

        static const constexpr char *T6ParamName = "t6";
        static const constexpr char *T6ParamDesc = "The time duration t6";

        static const constexpr char *T7ParamName = "t7";
        static const constexpr char *T7ParamDesc = "The time duration t7";

        static const constexpr char *TfParamName = "tf";
        static const constexpr char *TfParamDesc = "The time duration tf";

        static const constexpr char *T8ParamName = "t8";
        static const constexpr char *T8ParamDesc = "The time duration t8";

        static const constexpr char *InterruptionParamName = "interruption";
        static const constexpr char *InterruptionParamDesc = "The interruption duration";

        static const constexpr char *NbCyclesParamName = "nbCycles";
        static const constexpr char *NbCyclesParamDesc = "the number of pulse to generate";

        static const constexpr char *TriggerParamName = "trigger";
        static const constexpr char *TriggerParamDesc = "The trigger value";

        static const constexpr char *CurrentParamName = "current";
        static const constexpr char *CurrentParamDesc = "the value of current to set";

        static const constexpr char *TmoParamName = "timeout";
        static const constexpr char *TmoParamDesc = "the timeout value";

        //bounds:
        static const constexpr quint16 VoltStartMinDv = 20;
        static const constexpr quint16 VoltStartMaxDv = 300;
        static const constexpr quint16 VoltStartStepDv = 1;

        static const constexpr quint16 VoltLevelMinDv = -280;
        static const constexpr quint16 VoltLevelMaxDv = 280;
        static const constexpr quint16 VoltLevelStepDv = 1;

        static const constexpr quint16 AmplMinDv = 10;
        static const constexpr quint16 AmplMaxDv = 40;
        static const constexpr quint16 AmplStepDv = 1;

        static const constexpr quint16 TimeMinMs = 5;
        static const constexpr quint16 TimeMaxMs = 999;
        static const constexpr quint16 TimeStepMs = 1;

        static const constexpr quint16 TimeMinS = 1;
        static const constexpr quint16 TimeMaxS = 999;
        static const constexpr quint16 TimeStepS = 1;

        static const constexpr quint16 NbCycleMin = 1;
        static const constexpr quint16 NbCycleMax = 30000;
        static const constexpr quint16 NbCycleStep = 1;

        static const constexpr quint16 CurrentMin = 1;
        static const constexpr quint16 CurrentMax = 35;
        static const constexpr quint16 CurrentStep = 1;

        static const constexpr quint16 TmoMinMs = -1;
        static const constexpr quint16 TmoMaxMs = std::numeric_limits<quint16>::max();
        static const constexpr quint16 TmoStepMs = 1;

        static const constexpr quint16 UnitFactor = 10;

    private:
        APulseCmd *_aPulseCmd{nullptr};
        WriteProcess *_writeProcess{nullptr};

        PulseFlag::Flags _paramsFlag;
        QString _currentPulseName;
        ProcessStep _processStep;
        bool _connectedToComProcess{false};
        qint32 _timeout{-1};
};
