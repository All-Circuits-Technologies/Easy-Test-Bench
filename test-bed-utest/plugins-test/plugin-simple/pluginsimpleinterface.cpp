// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "pluginsimpleinterface.hpp"

#include "basicbankinterface.hpp"


QSharedPointer<IBankInterface> PluginSimpleInterface::accessBankInterface(
                                                                ICommonInterface &commonInterface)
{
    return BasicBankInterface::Create(commonInterface);
}
