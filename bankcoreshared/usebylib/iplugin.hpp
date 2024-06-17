// SPDX-FileCopyrightText: 2019 - 2020 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QSharedPointer>

#include "testbedcore/testbedglobal.hpp"

class IBankInterface;
class ICommonInterface;


/** @brief This class is an interface between the bank and the lib core */
class TESTBEDPLUGIN_DLLSPEC IPlugin
{
    public:
        /** @brief Class destructor */
        virtual ~IPlugin() = default;

    public:
        /** @brief Access the bank interface
            @param commonInterface An interface useful for Bank lib to get general information or
                                   doing special action on the lib core
            @return The interface of the bank */
        virtual QSharedPointer<IBankInterface> accessBankInterface(
                                                            ICommonInterface &commonInterface) = 0;
};

#define IPLUGIN_IID "testbed.plugin.IPlugin"
Q_DECLARE_INTERFACE(IPlugin, IPLUGIN_IID)
