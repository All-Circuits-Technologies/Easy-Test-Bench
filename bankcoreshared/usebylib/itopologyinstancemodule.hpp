// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "jsonutility/jsontypedefs.hpp"
#include "testbedcore/testbedglobal.hpp"

class QJsonObject;


/** @brief This class represents an API of a specific instance topology module*/
class TESTBEDPLUGIN_DLLSPEC ITopologyInstanceModule : public QObject
{
    Q_OBJECT

    protected:
        /** @brief Class constructor
            @param parent Class parent */
        explicit ITopologyInstanceModule(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~ITopologyInstanceModule() override = default;

    public:
        /** @brief Get the topology module name
            @param uniqueName The unique name of the topology module
            @return True If no problem occurs */
        virtual bool getModuleName(QString &uniqueName) const = 0;

        /** @brief Get the instance name of the topology module. The instance name makes it possible
                   to do a recognition between multiples instances of the same topology module in a
                   specific bank
            @param instanceName The topology instance name
            @return True If no problem occurs */
        virtual bool getInstanceName(QString &instanceName) const = 0;

        /** @brief Get the module description format, what data is useful to display a topology
                   module
            @note The JSON array of the module description format is as follow:
                    {
                        "name": "[string]",
                        "description": "[string]",
                        "type": "basic|intermediary",
                        "category": "[string]",
                        "pins": [
                            {
                                "groupName": "[string]",
                                "elements": [
                                    {
                                        "label": "[string]",
                                        "kind": "signal|[string]",
                                        "?direction":"input|ouput|inout"
                                    }
                                ]
                            }
                        ],
                        "params": @see ISequenceInstanceModule::getModuleParametersFormat
                    }
            @param description The description format used to display the module
            @return True If no problem occurs */
        virtual bool getModuleDescriptionFormat(JsonObject &description) const = 0;

        /** @brief Set the parameters filled of the topology module
            @note The JSON array of the module description format is as follow:
                    @see ISequenceInstanceModule::setModuleParameters
            @param parameters The parameters set by an user
            @param errors If the parameters haven't been correctly set, this array is filled with
                          informations to display to the user; and so he can understand what to
                          correct
            @return True If no problem occurs */
        virtual bool setModuleParameters(const JsonArray &parameters,
                                         JsonObject &errors) = 0;
};
