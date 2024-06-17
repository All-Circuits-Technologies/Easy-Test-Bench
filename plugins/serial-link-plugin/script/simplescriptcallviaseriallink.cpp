// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "simplescriptcallviaseriallink.hpp"

#include "managers/seriallinkbankglobalmanager.hpp"


SimpleScriptCallViaSerialLink::SimpleScriptCallViaSerialLink(QObject *parent)
    : ACallScriptViaSerialLink{staticMetaObject.className(),
                               SerialLinkBankGlobalManager::Instance(),
                               parent}
{
}

SimpleScriptCallViaSerialLink::~SimpleScriptCallViaSerialLink()
{
}
