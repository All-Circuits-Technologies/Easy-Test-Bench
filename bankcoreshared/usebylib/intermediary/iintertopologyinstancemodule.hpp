// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "jsonutility/jsontypedefs.hpp"
#include "testbedcore/testbedglobal.hpp"
#include "usebylib/itopologyinstancemodule.hpp"


/** @brief Extend the @ref ITopologyInstanceModule for intermerdiary topology modules,
           like: matrixes and multiplexers */
class TESTBEDPLUGIN_DLLSPEC IInterTopologyInstanceModule : public ITopologyInstanceModule
{
    Q_OBJECT

    protected:
        /** @brief Class constructor
            @param parent Class parent */
        explicit IInterTopologyInstanceModule(QObject *parent);

        /** @brief Class destructor */
        virtual ~IInterTopologyInstanceModule() override = default;

    public:
        /** @brief Get internal bindings of the module
            @note The json array returned is as follow:
                    [
                        {
                            "uniqueKey": "0-Hi:slot",
                            "fromPin": "0-Hi",
                            "toPin": "slot"
                        }
                    ]
                  The unique key is useful to activate an internal link, it's the key to send in
                  the @see IInterTopologyInstanceModule::activateLink method
            @param bindings The bindings which lies input and output pins
            @return True If no problem occurs */
        virtual bool getInternalBindings(JsonArray &bindings) const = 0;

        /** @brief Activate an internal link thanks to the unique key given.
            @note To get the unique key see the method
                  @see IInterTopologyInstanceModule::getInternalBindings
            @param uniqueKey The unique key to use in order to activate a link
            @return True If no problem occurs */
        virtual bool activateLink(const QString &uniqueKey) = 0;
};
