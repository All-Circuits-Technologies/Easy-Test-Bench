// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "peakcanplugininterface.hpp"

#include "peakcanbankglobalmanager.hpp"


QSharedPointer<IBankInterface> PeakCanPluginInterface::accessBankInterface(
                                                                ICommonInterface &commonInterface)
{
    return PeakCanBankGlobalManager::Create(commonInterface);
}
