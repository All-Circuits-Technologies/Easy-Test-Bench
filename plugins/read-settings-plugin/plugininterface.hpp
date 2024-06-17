// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QtPlugin>

#include "bankcoreshared/usebylib/iplugin.hpp"


/** @brief This class is an interface between the bank and the lib core */
class PluginInterface : public QObject, public IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID IPLUGIN_IID FILE PLUGIN_NAME ".json")
    Q_INTERFACES(IPlugin)

    public:
        /** @brief Access the bank interface
            @param commonInterface An interface useful for Bank lib to get general information or
                                   doing special action on the lib core
            @return The interface of the bank */
        virtual QSharedPointer<IBankInterface> accessBankInterface(
                                                        ICommonInterface &commonInterface) override;

};
