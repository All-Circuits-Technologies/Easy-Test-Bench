// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "defaultbankglobalmanager.hpp"

QSharedPointer<DefaultBankGlobalManager> DefaultBankGlobalManager::_instance =
                                                        QSharedPointer<DefaultBankGlobalManager>();


DefaultBankGlobalManager::DefaultBankGlobalManager(ICommonInterface &commonInterface,
                                                   QObject *parent) :
    DefaultBankInterface(commonInterface, parent)
{
}

const QSharedPointer<DefaultBankGlobalManager> &DefaultBankGlobalManager::Create(
                                                                ICommonInterface &commonInterface)
{
    if(!_instance)
    {
        _instance = QSharedPointer<DefaultBankGlobalManager>(
                                                    new DefaultBankGlobalManager(commonInterface));
    }
    else
    {
        qWarning() << "The object has already been created, return the current instance";
    }

    return _instance;
}

const QSharedPointer<DefaultBankGlobalManager> &DefaultBankGlobalManager::Instance()
{
    if(!_instance)
    {
        qWarning() << "The object hasn't been created, return null pointer";
    }

    return _instance;
}
