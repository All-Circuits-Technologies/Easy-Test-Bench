// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "plugininterface.hpp"

#include "bankglobalmanager.hpp"


QSharedPointer<IBankInterface> PluginInterface::accessBankInterface(
                                                                ICommonInterface &commonInterface)
{
    return BankGlobalManager::Create(commonInterface);
}
