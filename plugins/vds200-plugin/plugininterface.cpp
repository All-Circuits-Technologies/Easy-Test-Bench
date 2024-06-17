// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "plugininterface.hpp"

#include "abstractpluginelements/defaultbankglobalmanager.hpp"


QSharedPointer<IBankInterface> PluginInterface::accessBankInterface(
                                                                ICommonInterface &commonInterface)
{
    return DefaultBankGlobalManager::Create(commonInterface);
}
