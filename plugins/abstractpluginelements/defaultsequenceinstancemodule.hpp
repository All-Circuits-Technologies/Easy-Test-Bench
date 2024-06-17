// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "bankcoreshared/usebylib/isequenceinstancemodule.hpp"
#include "testbedcore/utility/sequencestatehelper.hpp"

class DefaultBankInterface;
class ICommonInterface;


/** @brief This class gives default implementation of the @ref ISequenceInstanceModule (in order to
           be used by plugins). The default implementations of some API methods as the bank unique
           name or the bank version */
class DefaultSequenceInstanceModule : public ISequenceInstanceModule
{
    Q_OBJECT

    protected:
        /** @brief Class constructor
            @param moduleClassName The class name of the module
            @param bankLinked The bank linked to the instance module
            @param parent The class parent */
        explicit DefaultSequenceInstanceModule(
            const QString &moduleClassName,
            const QSharedPointer<DefaultBankInterface> &bankLinked,
            QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~DefaultSequenceInstanceModule() override = default;

    public:
        /** @brief Get the bank interface linked to the sequence module */
        virtual QSharedPointer<IBankInterface> getBankInterface() const override;

        /** @see ISequenceInstanceModule::getModuleName
            @note The module name is built with the high level class name, set in lower case */
        virtual bool getModuleName(QString &uniqueName) const override;

        /** @see ISequenceInstanceModule::getInstanceName
            @note The instance name is built with the module name concatenated with the hash of an
                  UUID */
        virtual bool getInstanceName(QString &instanceName) const override;

        /** @see ISequenceInstanceModule::setSequenceToken */
        virtual bool setSequenceToken(const QString &sequenceToken) override;

        /** @see ISequenceInstanceModule::getState
            @note All the state system is managed by this class, no need to do it in your module
                  implementation */
        virtual bool getState(SequenceStateType::Enum &currentState) const override;

        /** @see ISequenceInstanceModule::getModuleParametersFormat */
        virtual bool getModuleParametersFormat(JsonArray &parameters) const override;

        /** @see ISequenceInstanceModule::setModuleParameters */
        virtual bool setModuleParameters(const JsonArray &parameters, JsonArray &errors) override;

        /** @see ISequenceInstanceModule::getModuleParameters */
        virtual bool getModuleParameters(JsonArray &parameters) const override;

        /** @see ISequenceInstanceModule::launchModule
            @note Manage the initializing of several elements like the state and the need to stop or
                  pause the plugin
            @note This method has to be reimplemented in subclasses but this implementation has to
                  be called at the start of the subclass method*/
        virtual bool launchModule(const JsonArray &jsonObjectInputValues) override;

        /** @see ISequenceInstanceModule::stopModule
            @note This class sets the fact that the plugin has to stop
            @note To help the stopping of plugins give a look at the class @ref PauseAndStopHelper
                  in the testbedcore lib */
        virtual bool stopModule(StopAskedType::Enum stopAskedType) override;

        /** @see ISequenceInstanceModule::manageModulePause
            @note This class sets the fact that the plugin has to be paused or resumed
            @note To help the pause of a plugin give a look at the class @ref PauseAndStopHelper
                  in the testbedcore lib */
        virtual bool manageModulePause(bool pauseModule) override;

        /** @see ISequenceInstanceModule::getInputVariablesFormat
            @note By default return an empty format and true */
        virtual bool getInputVariablesFormat(JsonArray &inVariablesFormat) const override;

        /** @see ISequenceInstanceModule::getOutputVariablesFormat
            @note By default return an empty format and true */
        virtual bool getOutputVariablesFormat(JsonArray &outVariablesFormat) const override;

    public:
        /** @brief Return the current sequence module state */
        const SequenceStateHelper &getSequenceState() const { return _sequenceState; }

        /** @brief Get the token of the sequence linked to this module */
        const QString &getSequenceToken() const { return _sequenceToken; }

        /** @brief Access common interface */
        ICommonInterface &accessCommIntf() const;

        /** @brief Get common interface */
        const ICommonInterface &getCommIntf() const;

    public slots:
        /** @brief Called to emit a new sequence log from the sequence module
            @param log The log to emit */
        void onNewSeqLogs(const SequenceLogData &log);

    protected:
        /** @brief Get the module name */
        QString getName() const { return _moduleName; }

        /** @brief Validate and set the parameters into the module
            @see ISequenceInstanceModule::setModuleParameters
            @attention Don't forget to call this method at the start of the method if you
                       override it
            @note By default this method only validate parameters and init param format if needed
            @param parameters The parameters to test given by the test lib
            @param errors If the parameters haven't been correctly set, this array is filled with
                          informations to display to the user; and so he can understand what to
                          correct
            @return True if no problem occurs */
        virtual bool validateAndSetParameters(const JsonArray &parameters, JsonArray &errors);

        /** @brief Only called once when needed, this returns the parameters format
            @note By default return an empty json array
            @param paramsFormat The parameter format
            @return True if no problem occurs */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const;

        /** @brief Only called once when needed, this returns the inputs format
            @note By default return an empty json array
            @param inputsFormat The inputs format
            @return True if no problem occurs */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const;

        /** @brief Only called once when needed, this returns the outputs format
            @note By default return an empty json array
            @param outputsFormat The parameter format
            @return True if no problem occurs */
        virtual bool initOutputsFormat(JsonArray &outputsFormat) const;

    protected slots:
        /** @brief Called in an asynchronous way by the launchModule method
            @param jsonObjectInputValues The input values given by the test bed lib */
        virtual void processModule(const JsonArray &jsonObjectInputValues) = 0;

        /** @brief Called at the end of sequence module process to emit end log and notify the fact
                   that the module is stopped
            @param success True If the module run has properly ended
            @param outputValues The output values to set to the linked variables */
        virtual void endProcess(bool success, const JsonArray &outputValues);

    public:
        /** @brief Help to fill the parameters JsonArray error
            @param parameterName The parameter which has caused an error
            @param errorStr The error caused by the parameter
            @param errorArray The JsonArray to fill with the error */
        static void fillErrorArray(const QString &parameterName,
                                   const QString &errorStr,
                                   JsonArray &errorArray);

    private:
        /** @brief This enum is used to represent the format loading state */
        enum FormatResult
        {
            NotLoaded,
            Success,
            Error
        };

        /** @brief This structure represent the format of outputs, inputs or parameters and if the
                   format have already been loaded */
        struct FormatElement
        {
            JsonArray format{};
            FormatResult result{ NotLoaded };
        };

    private:
        /** @brief Useful generic method used to generate an unique instance name
            @param moduleName The module from whom to generate the instance name
            @return The instance name generated */
        static QString generateInstanceName(const QString &moduleName);

        /** @brief Manage the initialization of the inputs, outputs or parameters format
            @param element The element to update
            @param initMethod The method to call in order to init the initialization of the element
            @param elementName The name of the element to get its format (only used for logging)
            @return True if no problem occurs */
        static bool manageFormatElement(FormatElement &element,
                                        const std::function<bool (JsonArray &)> &initMethod,
                                        const QString &elementName);

    private:
        /** @brief Represents the hexacimal base for a number */
        static const constexpr int HexBase = 16;

    private:
        const QString _moduleName;
        const QString _instanceName;
        const QSharedPointer<DefaultBankInterface> _bankLinked;

        // Use mutable for the three elements because this is cache: we load the element the first
        // time and then it's only const get.
        mutable FormatElement _parametersFormat;
        mutable FormatElement _inputsFormat;
        mutable FormatElement _outputsFormat;

        QString _sequenceToken;

        JsonArray _parameters;

        SequenceStateHelper _sequenceState;
};
