// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QtPlugin>

#include "bankcoreshared/usebylib/iplugin.hpp"

class BasicBankInterface;


class PluginSimpleInterface : public QObject, public IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID IPLUGIN_IID FILE PLUGIN_NAME ".json")
    Q_INTERFACES(IPlugin)

    public:
        virtual QSharedPointer<IBankInterface> accessBankInterface(
                                                        ICommonInterface &commonInterface) override;
};
