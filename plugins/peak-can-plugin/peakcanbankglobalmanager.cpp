// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "peakcanbankglobalmanager.hpp"

#include "qtpeakcanlib/src/canmanager.hpp"

#include "managers/autoanswerpeakcanmanager.hpp"

QSharedPointer<PeakCanBankGlobalManager> PeakCanBankGlobalManager::_instance
    = QSharedPointer<PeakCanBankGlobalManager>();


PeakCanBankGlobalManager::PeakCanBankGlobalManager(ICommonInterface &commonInterface,
                                                   QObject *parent) :
    DefaultBankInterface(commonInterface, parent),
    _autoAnswerManager{new AutoAnswerPeakCanManager(this)}
{
}

PeakCanBankGlobalManager::~PeakCanBankGlobalManager()
{
}

void PeakCanBankGlobalManager::initManager()
{
    CanManager::getInstance();
}

const QSharedPointer<PeakCanBankGlobalManager> &PeakCanBankGlobalManager::Create(
                                                                ICommonInterface &commonInterface)
{
    if(!_instance)
    {
        _instance = QSharedPointer<PeakCanBankGlobalManager>(
                                                    new PeakCanBankGlobalManager(commonInterface));
    }
    else
    {
        qWarning() << "The object has already been created, return the current instance";
    }

    return _instance;
}

const QSharedPointer<PeakCanBankGlobalManager> &PeakCanBankGlobalManager::Instance()
{
    if(!_instance)
    {
        qWarning() << "The object hasn't been created, return null pointer";
    }

    return _instance;
}
