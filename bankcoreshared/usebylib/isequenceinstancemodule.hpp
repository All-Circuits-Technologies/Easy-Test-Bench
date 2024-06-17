// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "jsonutility/jsontypedefs.hpp"
#include "testbedcore/logs/sequencelogdata.hpp"
#include "testbedcore/testbedglobal.hpp"
#include "testbedcore/types/sequencestatetype.hpp"
#include "testbedcore/types/stopaskedtype.hpp"

class IBankInterface;


/** @brief This class represents an API of a specific instance sequence module */
class TESTBEDPLUGIN_DLLSPEC ISequenceInstanceModule : public QObject
{
    Q_OBJECT

    protected:
        /** @brief Class constructor
            @param parent Class parent */
        explicit ISequenceInstanceModule(QObject *parent = nullptr);

    public:
        /** @brief Class destructor */
        virtual ~ISequenceInstanceModule() override = default;

    public:
        /** @brief Get the bank interface linked to the sequence module
            @return The bank interface linked to the sequence module */
        virtual QSharedPointer<IBankInterface> getBankInterface() const = 0;

        /** @brief Get the sequence module name
            @param uniqueName The unique name of the sequence module
            @return True If no problem occurs */
        virtual bool getModuleName(QString &uniqueName) const = 0;

        /** @brief Get the instance name of the sequence module. The instance name makes it possible
                   to do a recognition between multiples instances of the same sequence module in a
                   specific bank
            @param instanceName The sequence instance name
            @return True If no problem occurs */
        virtual bool getInstanceName(QString &instanceName) const = 0;

        /** @brief Set the token of the sequence linked to this module, each module is built in a
                   sequence, and this is its token
            @param sequenceToken The sequence token
            @return True If no problem occurs */
        virtual bool setSequenceToken(const QString &sequenceToken) = 0;

        /** @brief Launch the sequence module with the current variables values connected to this
                   module.
            @note The method is asynchronous,
                    - to stop the module to run @see ISequenceInstanceModule::stopModule
                    - to pause or unpause the module @see ISequenceInstanceModule::manageModulePause
                    - when the module has stopped, the following signal is emitted:
                      @see ISequenceInstanceModule::moduleFinished
            @note The json array of the input values is as follow:
                    [
                        {
                            "name": "[string]",
                            "value": "[string|bool|integer]"
                        },
                        {
                            "name": "[string]",
                            "values": [
                                "[string|bool|integer]"
                            ]
                        }
                    ]
            @param jsonObjectInputValues The input variables values
            @return True If no problem occurs */
        virtual bool launchModule(const JsonArray &jsonObjectInputValues) = 0;

        /** @brief Stop the module
            @param stopAskedType Precise who ask for a stop (useful to display logs with context)
            @return True If no problem occurs */
        virtual bool stopModule(StopAskedType::Enum stopAskedType) = 0;

        /** @brief Pause or unpause the running module, it's not a stop
            @param pauseModule True to pause the module activities
                               False to unpause a paused module
            @return True If no problem occurs */
        virtual bool manageModulePause(bool pauseModule) = 0;

        /** @brief Get the current state of the sequence module
            @param currentState The sequence module current state
            @return True If no problem occurs */
        virtual bool getState(SequenceStateType::Enum &currentState) const = 0;

        /** @brief Get the module parameters format
            @note The json array of the parameters format is as follow:
                    [{
                        "name": "[string]",
                        "type": "string",
                        "description": "[string]",
                        "?minLength": "[integer]",
                        "?maxLength": "[integer]",
                        "?patternRegex": "[string]",
                        "?clearFieldIfError": "[bool]",
                        "?timeoutInSec": "[integer]",
                        "?imagePath": "[string]",
                        "?defaultValues": [
                            "[string]"
                        ],
                        "?hide": "[bool]",
                        "?required": "[bool]"
                    },
                    {
                        "name": "[string]",
                        "type": "stringList",
                        "description": "[string]",
                        "?minListLength": "[integer]",
                        "?maxListLength": "[integer]",
                        "?minLength": "[integer]",
                        "?maxLength": "[integer]",
                        "?patternRegex": "[string]",
                        "?timeoutInSec": "[integer]",
                        "?imagePath": "[string]",
                        "?defaultValues": "[string]",
                        "?hide": "[bool]",
                        "?required": "[bool]"
                    },
                    {
                        "name": "[string]",
                        "type": "bool",
                        "description": "[string]",
                        "?timeoutInSec": "[integer]",
                        "?imagePath": "[string]",
                        "?defaultValue": "[bool]",
                        "?hide": "[bool]",
                        "?required": "[bool]"
                    },
                    {
                        "name": "[string]",
                        "type": "number",
                        "description": "[string]",
                        "nbAfterCommaToDisplay": "[integer]",
                        "?min": "[number]",
                        "?max": "[number]",
                        "?step": "[integer]",
                        "?unit": "[A|V|W|°C|Ω|s|Hz|ADC(bits)|lm|F|H||K|string<exp></exp><sub></sub>]",
                        "?unitPowerOf10": "[integer]",
                        "?timeoutInSec": "[integer]",
                        "?imagePath": "[string]",
                        "?defaultValue": "[integer]",
                        "?hide": "[bool]",
                        "?required": "[bool]"
                    },
                    {
                        "name": "[string]",
                        "type": "date",
                        "description": "[string]",
                        "?min": "[string]",
                        "?max": "[string]",
                        "?timeoutInSec": "[integer]",
                        "?imagePath": "[string]",
                        "?defaultValue": "[string]",
                        "?hide": "[bool]",
                        "?required": "[bool]"
                    },
                    {
                        "name": "[string]",
                        "type": "time",
                        "description": "[string]",
                        "?min": "[integer]",
                        "?max": "[integer]",
                        "?timeoutInSec": "[integer]",
                        "?imagePath": "[string]",
                        "?defaultValue": "[integer]",
                        "?hide": "[bool]",
                        "?required": "[bool]"
                    },
                    {
                        "name": "[string]",
                        "type": "dateTime",
                        "description": "[string]",
                        "?min": "[integer]",
                        "?max": "[integer]",
                        "?timeoutInSec": "[integer]",
                        "?imagePath": "[string]",
                        "?defaultValue": "[integer]",
                        "?hide": "[bool]",
                        "?required": "[bool]"
                    },
                    {
                        "name": "[string]",
                        "type": "select",
                        "description": "[string]",
                        "options": [{
                            "key": "[string]",
                            "value": "[string]",
                            "?hide": "[bool]"
                        }],
                        "?optionDefault": [
                            "[string]"
                        ],
                        "?multiple": "[bool]",
                        "?timeoutInSec": "[integer]",
                        "?imagePath": "[string]",
                        "?hide": "[bool]",
                        "?required": "[bool]"
                    },
                    {
                        "name": "[string]",
                        "type": "checkBoxList",
                        "description": "[string]",
                        "list": [{
                            "key": "[string]",
                            "?hide": "[bool]"
                        }],
                        "?checkedList": [
                            "[string]"
                        ],
                        "?timeoutInSec": "[integer]",
                        "?imagePath": "[string]",
                        "?hide": "[bool]",
                        "?required": "[bool]"
                    },
                    {
                        "name": "[string]",
                        "type": "radioList",
                        "description": "[string]",
                        "list": [{
                            "key": "[string]",
                            "?hide": "[bool]"
                        }],
                        "?timeoutInSec": "[integer]",
                        "?imagePath": "[string]",
                        "?defaultChecked": "[string]",
                        "?hide": "[bool]",
                        "?required": "[bool]"
                    },
                    {
                        "name": "[string]",
                        "type": "radioListButtons",
                        "description": "[string]",
                        "list": [{
                            "key": "[string]",
                            "label": "[string]",
                            "?keyShortcut": "[string:oneLetter]",
                            "?fontColor": "[string]",
                            "?backgroundColor": "[string]",
                            "?hide": "[bool]"
                        }],
                        "?timeoutInSec": "[integer]",
                        "?imagePath": "[string]",
                        "?defaultChecked": "[string]",
                        "?hide": "[bool]",
                        "?required": "[bool]"
                    },
                    {
                        "name": "[string]",
                        "type": "okButton",
                        "description": "[string]",
                        "key": "[string]",
                        "label": "[string]",
                        "?keyShortcut": "[string:oneLetter]",
                        "?fontColor": "[string]",
                        "?backgroundColor": "[string]",
                        "?timeoutInSec": "[integer]",
                        "?imagePath": "[string]",
                        "?defaultChecked": "[bool]",
                        "?hide": "[bool]",
                        "?required": "[bool]"
                    },
                    {
                        "name": "[string]",
                        "type": "color",
                        "description": "[string]",
                        "?timeoutInSec": "[integer]",
                        "?imagePath": "[string]",
                        "?defaultValue": "[string]",
                        "?hide": "[bool]",
                        "?required": "[bool]"
                    },
                    {
                        "name": "[string]",
                        "type": "selectFile",
                        "description": "[string]",
                        "?selectDir": "[bool]",
                        "?filesExtensions": [
                            "[string]"
                        ],
                        "?currentDirectory": "[string]",
                        "?timeoutInSec": "[integer]",
                        "?imagePath": "[string]",
                        "?defaultValue": "[string]",
                        "?hide": "[bool]",
                        "?required": "[bool]"
                    }]
            @param parameters The format of the module parameters, which is possible to set
            @return True If no problem occurs */
        virtual bool getModuleParametersFormat(JsonArray &parameters) const = 0;

        /** @brief Set the parameters filled of the sequence module
            @note The json array of the parameters values is as follow:
                    [
                        {
                            "name": "[string]",
                            "value": "[bool|string|integer]"
                        },
                        {
                            "name": "[string]",
                            "values": [
                                "[bool|string|integer]"
                            ]
                        }
                    ]
            @note The json array of the errors when filling the parameter is as follow:
                    [{
                        "name": "[string]",
                        "errorLabel": "[string]"
                    }]
            @param parameters The parameters set by an user
            @param errors If the parameters haven't been correctly set, this array is filled with
                          informations to display to the user; and so he can understand what to
                          correct
            @return True If no problem occurs */
        virtual bool setModuleParameters(const JsonArray &parameters,
                                         JsonArray &errors) = 0;

        /** @brief Get the sequence module parameters filled by an user
            @note The json format of the parameters is given in the set method:
                  @see ISequenceInstanceModule::setModuleParameters
            @return True If no problem occurs */
        virtual bool getModuleParameters(JsonArray &parameters) const = 0;

        /** @brief Get the json format of the module inputs, it allowes to know what variables can
                   be linked to the module
            @note The json array of the input variables format is as follow:
                    [
                        {
                            "name": "[string]",
                            "typeManaged": "string",
                            "?description": "[string]",
                            "?defaultValue": "[string]"
                        },
                        {
                            "name": "[string]",
                            "typeManaged": "bool",
                            "?description": "[string]",
                            "?defaultValue": "[bool]"
                        },
                        {
                            "name": "[string]",
                            "typeManaged": "number",
                            "?description": "[string]",
                            "?defaultValue": "[integer]",
                            "?unit": "[A|V|W|°C|Ω|s|Hz|ADC(bits)|lm|F|H||K|string<exp></exp><sub></sub>]",
                            "?unitPowerOf10": "[integer]"
                        },
                        {
                            "name": "[string]",
                            "typeManaged": "list<bool>",
                            "?description": "[string]",
                            "?defaultValues": [
                                "[bool]"
                            ]
                        },
                        {
                            "name": "[string]",
                            "typeManaged": "list<string>",
                            "?description": "[string]",
                            "?defaultValues": [
                                "[string]"
                            ]
                        },
                        {
                            "name": "[string]",
                            "typeManaged": "list<number>",
                            "?description": "[string]",
                            "?defaultValues": [
                                "[number]"
                            ],
                            "?unit": "[A|V|W|°C|Ω|s|Hz|ADC(bits)|lm|F|H||K|string<exp></exp><sub></sub>]",
                            "?unitPowerOf10": "[integer]"
                        }
                    ]
            @param inVariablesFormat The format of the input variables which can be linked with the
                                     sequence module
            @return True If no problem occurs */
        virtual bool getInputVariablesFormat(JsonArray &inVariablesFormat) const = 0;

        /** @brief Get the JSON format of the module outputs, it allowes to know what variables can
                   be linked to the module
            @note The json array of the output variables is the same as the inputs, therefore:
                    @see ISequenceInstanceModule::getInputVariablesFormat
            @param outVariablesFormat The format of the output variables which can be linked with
                                      the sequence module
            @return True If no problem occurs */
        virtual bool getOutputVariablesFormat(JsonArray &outVariablesFormat) const = 0;

    signals:
        /** @brief Emitted when the sequence module changes its state
            @param currentState The current sequence state
            @param previousState The previous sequence state (the state before the current one) */
        void newState(SequenceStateType::Enum currentState, SequenceStateType::Enum previousState);

        /** @brief Emitted when the sequence module is stopped, with the output values
            @param success True If the module run has properly ended
            @param jsonObjectOutputValues The output values to set to the linked variables */
        void moduleFinished(bool success, const JsonArray &jsonObjectOutputValues);

        /** @brief Emitted when the sequence module wants to generate a new log, which will be
                   attached to the sequence
            @param log The log message and detail */
        void newLog(const SequenceLogData &log);
};
