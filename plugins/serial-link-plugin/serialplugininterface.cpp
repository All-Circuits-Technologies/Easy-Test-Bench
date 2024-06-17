// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "serialplugininterface.hpp"

#include "managers/seriallinkbankglobalmanager.hpp"


QSharedPointer<IBankInterface> SerialPluginInterface::accessBankInterface(
                                                                ICommonInterface &commonInterface)
{
    return SerialLinkBankGlobalManager::Create(commonInterface);
}
