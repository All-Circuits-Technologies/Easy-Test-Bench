// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "seriallinkbankglobalmanager.hpp"

#include "qtseriallinklib/seriallinkmanager.hpp"

#include "managers/autoanswerserialmanager.hpp"

QSharedPointer<SerialLinkBankGlobalManager> SerialLinkBankGlobalManager::_instance
    = QSharedPointer<SerialLinkBankGlobalManager>();


SerialLinkBankGlobalManager::SerialLinkBankGlobalManager(ICommonInterface &commonInterface,
                                                         QObject *parent)
    : DefaultBankInterface{commonInterface, parent},
    _autoAnswerManager{new AutoAnswerSerialManager(this)}
{
}

SerialLinkBankGlobalManager::~SerialLinkBankGlobalManager()
{
}

void SerialLinkBankGlobalManager::initManager()
{
    SerialLinkManager::getInstance();
}

const QSharedPointer<SerialLinkBankGlobalManager> &SerialLinkBankGlobalManager::Create(
    ICommonInterface &commonInterface)
{
    if(!_instance)
    {
        _instance = QSharedPointer<SerialLinkBankGlobalManager>(
            new SerialLinkBankGlobalManager(commonInterface));
        _instance->initManager();
    }
    else
    {
        qWarning() << "The object has already been created, return the current instance";
    }

    return _instance;
}

const QSharedPointer<SerialLinkBankGlobalManager> &SerialLinkBankGlobalManager::Instance()
{
    if(!_instance)
    {
        qWarning() << "The object hasn't been created, return null pointer";
    }

    return _instance;
}
