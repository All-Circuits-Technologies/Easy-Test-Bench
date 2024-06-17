// SPDX-FileCopyrightText: 2019 - 2020 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "bankglobalmanager.hpp"

#include "functions/elapsedtimer/elapsedtimermanager.hpp"

QSharedPointer<BankGlobalManager> BankGlobalManager::_instance
                                                              = QSharedPointer<BankGlobalManager>();


BankGlobalManager::BankGlobalManager(ICommonInterface &commonInterface, QObject *parent) :
    DefaultBankInterface(commonInterface, parent)
{
}

const QSharedPointer<BankGlobalManager> &BankGlobalManager::Create(ICommonInterface &commonInterface)
{
    if(!_instance)
    {
        _instance = QSharedPointer<BankGlobalManager>(new BankGlobalManager(commonInterface));
        _instance->initManagers();
    }
    else
    {
        qWarning() << "The object has already been created, return the current instance";
    }

    return _instance;
}

const QSharedPointer<BankGlobalManager> &BankGlobalManager::Instance()
{
    if(!_instance)
    {
        qWarning() << "The object hasn't been created, return null pointer";
    }

    return _instance;
}

ElapsedTimerManager &BankGlobalManager::accessElapsedTimerManager()
{
    return *_elapsedTimerManager;
}

const ElapsedTimerManager &BankGlobalManager::getElapsedTimerManager() const
{
    return *_elapsedTimerManager;
}

void BankGlobalManager::initManagers()
{
    _elapsedTimerManager = new ElapsedTimerManager(this);
}
