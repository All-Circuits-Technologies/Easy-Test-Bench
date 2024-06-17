// SPDX-FileCopyrightText: 2019 - 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "fakecommoninterface.hpp"


FakeCommonInterface::FakeCommonInterface(QObject * parent):
    ICommonInterface(parent)
{
}

FakeCommonInterface::~FakeCommonInterface()
{
}

bool FakeCommonInterface::getRoutesLinkedToTopologyModule(
                                                    const QString &bankUniqueName,
                                                    const QString &uniqueTopologyModuleName,
                                                    const QString &uniqueTopologyInstanceModuleName,
                                                    JsonArray &pinsLinks) const
{
    Q_UNUSED(bankUniqueName)
    Q_UNUSED(uniqueTopologyModuleName)
    Q_UNUSED(uniqueTopologyInstanceModuleName)
    Q_UNUSED(pinsLinks)
    return false;
}

bool FakeCommonInterface::activateRoute(const JsonObject &pinsLink)
{
    Q_UNUSED(pinsLink)
    return false;
}

bool FakeCommonInterface::displayParameterInput(const JsonObject &elemToDisplay,
                                                QString &uuidToken)
{
    Q_UNUSED(elemToDisplay)
    Q_UNUSED(uuidToken)
    return false;
}

bool FakeCommonInterface::updateDisplay(const QString &uuidToken, const JsonObject &elemToUpdate)
{
    Q_UNUSED(uuidToken)
    Q_UNUSED(elemToUpdate)
    return false;
}

bool FakeCommonInterface::cancelDisplay(const QString &uuidToken)
{
    Q_UNUSED(uuidToken)
    return false;
}

bool FakeCommonInterface::readSetting(const QString &settingStrKey, QVariant &value) const
{
    Q_UNUSED(settingStrKey)
    Q_UNUSED(value)
    return false;
}

const ISequenceLogManager *FakeCommonInterface::getSequenceLogManager(
                                                        const QString &sequenceUniqueToken) const
{
    Q_UNUSED(sequenceUniqueToken)
    return nullptr;
}

bool FakeCommonInterface::getSequenceDirectory(const QString &seqToken, QDir &seqDir) const
{
    Q_UNUSED(seqToken)
    Q_UNUSED(seqDir)
    return false;
}

bool FakeCommonInterface::getPermStoreValue(const QString &storeKey, QString &storeValue) const
{
    Q_UNUSED(storeKey)
    Q_UNUSED(storeValue)
    return false;
}

bool FakeCommonInterface::setPermStoreValue(const QString &storeKey, const QString &storeValue)
{
    Q_UNUSED(storeKey)
    Q_UNUSED(storeValue)
    return false;
}

bool FakeCommonInterface::getProjectCacheValue(const QString &seqToken,
                                               const QString &cacheKey,
                                               QVariant &cacheValue) const
{
    Q_UNUSED(seqToken)
    Q_UNUSED(cacheKey)
    Q_UNUSED(cacheValue)
    return false;
}

bool FakeCommonInterface::setProjectCacheValue(const QString &seqToken,
                                               const QString &cacheKey,
                                               const QVariant &cacheValue)
{
    Q_UNUSED(seqToken)
    Q_UNUSED(cacheKey)
    Q_UNUSED(cacheValue)
    return false;
}
