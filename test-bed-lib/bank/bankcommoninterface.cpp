// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "bankcommoninterface.hpp"

#include <QDir>

#include "bank/adisplayuserinterface.hpp"
#include "bank/displayuitoken.hpp"
#include "core/globalmanager.hpp"
#include "core/knownsettings.hpp"
#include "core/settingsmanager.hpp"
#include "database/dbpluginpermstorageasker.hpp"
#include "models/pluginpermstoragevalue.hpp"
#include "project/projectcache.hpp"
#include "project/projectfile.hpp"
#include "project/projectmanager.hpp"
#include "sequence/logs/sequencelogmanager.hpp"
#include "sequence/nodessequence.hpp"
#include "sequence/sequencefile.hpp"
#include "testbedcore/testbedglobal.hpp"
#include "threadutility/concurrent/threadconcurrentrun.hpp"


BankCommonInterface::BankCommonInterface(ADisplayUserInterface *userInterface,
                                         DbManager &dbManager,
                                         QObject *parent) :
    ICommonInterface(parent),
    _userInterface(userInterface),
    _dbPluginCacheAsker(new DbPluginPermStorageAsker(dbManager, this))
{
}

bool BankCommonInterface::getRoutesLinkedToTopologyModule(
                                                    const QString &bankUniqueName,
                                                    const QString &uniqueTopologyModuleName,
                                                    const QString &uniqueTopologyInstanceModuleName,
                                                    JsonArray &pinsLinks) const
{
    GetRoutesLinkedResults result = ThreadConcurrentRun::run(
                                        *this,
                                         &BankCommonInterface::getRoutesLinkedToTopologyModulePriv,
                                         bankUniqueName,
                                         uniqueTopologyModuleName,
                                         uniqueTopologyInstanceModuleName,
                                         pinsLinks);
    pinsLinks = result.pinsLinks;

    return result.success;
}

bool BankCommonInterface::activateRoute(const JsonObject &pinsLink)
{
    return ThreadConcurrentRun::run(*this, &BankCommonInterface::activateRouteImpl, pinsLink);
}

bool BankCommonInterface::displayParameterInput(const JsonObject &elemToDisplay,
                                                QString &uuidToken)
{
    if(!_userInterface)
    {
        qWarning() << "No class has been set to display the user interface, can't proceed";
        return false;
    }

    QSharedPointer<DisplayUiToken> token = _userInterface->displayWidget(elemToDisplay);

    if(!token)
    {
        qWarning() << "A problem occurred when trying to create a widget to display";
        return false;
    }

    uuidToken = token->getUuid();
    _tokensList.insert(uuidToken, token);

    connect(token.data(), &DisplayUiToken::finished,
            this,         &BankCommonInterface::onTokenFinished);

    return true;
}

bool BankCommonInterface::updateDisplay(const QString &uuidToken, const JsonObject &elemToUpdate)
{
    if(!_tokensList.contains(uuidToken))
    {
        qWarning() << "The token given has no gui linked, we can't update it";
        return false;
    }

    _userInterface->updateWidget(_tokensList[uuidToken], elemToUpdate);
    return true;
}

bool BankCommonInterface::getPermStoreValue(const QString &storeKey, QString &storeValue) const
{
    QSharedPointer<PluginPermStorageValue> permStoreValue;
    RETURN_IF_FALSE(_dbPluginCacheAsker->getValue(storeKey, permStoreValue));

    if(permStoreValue)
    {
        storeValue = permStoreValue->getValue();
    }
    else
    {
        storeValue = QString();
    }

    return true;
}

bool BankCommonInterface::setPermStoreValue(const QString &storeKey, const QString &storeValue)
{
    return _dbPluginCacheAsker->setValue(storeKey, storeValue);
}

bool BankCommonInterface::getProjectCacheValue(const QString &seqToken,
                                               const QString &cacheKey,
                                               QVariant &cacheValue) const
{
    GetProjectCache result =  ThreadConcurrentRun::run(
                                                    *this,
                                                    &BankCommonInterface::getProjectCacheValuePriv,
                                                    seqToken,
                                                    cacheKey);

    cacheValue = result.cacheValue;

    return result.success;
}

bool BankCommonInterface::setProjectCacheValue(const QString &seqToken,
                                               const QString &cacheKey,
                                               const QVariant &cacheValue)
{
    return ThreadConcurrentRun::run(*this,
                                    &BankCommonInterface::setProjectCacheValueImpl,
                                    seqToken,
                                    cacheKey,
                                    cacheValue);
}

bool BankCommonInterface::readSetting(const QString &settingStrKey, QVariant &value) const
{
    KnownSettings::Enum knownSetting = KnownSettings::parseFromString(settingStrKey);

    if(knownSetting == KnownSettings::UnknownSetting)
    {
        qWarning() << "The setting key: " << settingStrKey << ", is unknown by the Setting manager";
        return false;
    }

    return GlobalManager::instance().getSettingsMngr().read(knownSetting, value);
}

const ISequenceLogManager *BankCommonInterface::getSequenceLogManager(
                                                        const QString &sequenceUniqueToken) const
{
    return ThreadConcurrentRun::run(*this,
                                    &BankCommonInterface::getSequenceLogManagerImpl,
                                    sequenceUniqueToken);
}

bool BankCommonInterface::cancelDisplay(const QString &uuidToken)
{
    if(!_tokensList.contains(uuidToken))
    {
        qWarning() << "The token given has no gui linked, we can't cancel it";
        return false;
    }

    _tokensList[uuidToken]->cancelDisplay();
    return true;
}

void BankCommonInterface::onTokenFinished(bool success,
                                          const QString &uuidToken,
                                          const JsonArray &valuesSet)
{
    emit guiResult(success, uuidToken, valuesSet);

    _tokensList.remove(uuidToken);
}

bool BankCommonInterface::getSequenceDirectory(const QString &seqToken, QDir &seqDir) const
{
    QSharedPointer<const SequenceFile> sequenceFile =
                            ThreadConcurrentRun::run(GlobalManager::instance().accessProjectMngr(),
                                                     &ProjectManager::tryToGetLinkedSequenceFile,
                                                     seqToken);

    if(!sequenceFile)
    {
        return false;
    }

    QString absDirPath = sequenceFile->getAbsoluteDirPath();

    if(absDirPath.isNull())
    {
        qWarning() << "The sequence file linked to the token: " << seqToken << ", has no file "
                   << "linked";
        return false;
    }

    seqDir = QDir(absDirPath);

    if(!seqDir.exists(absDirPath))
    {
        qWarning() << "The sequence file directory: " << absDirPath << ", linked to the sequence "
                   << "token: " << seqToken <<  ", doesn't exist";
        return false;
    }

    return true;
}

BankCommonInterface::GetRoutesLinkedResults
                            BankCommonInterface::getRoutesLinkedToTopologyModulePriv(
                                                    const QString &bankUniqueName,
                                                    const QString &uniqueTopologyModuleName,
                                                    const QString &uniqueTopologyInstanceModuleName,
                                                    const JsonArray &pinsLinks) const
{
    GetRoutesLinkedResults result { false, pinsLinks };

    result.success = getRoutesLinkedToTopologyModuleImpl(bankUniqueName,
                                                         uniqueTopologyModuleName,
                                                         uniqueTopologyInstanceModuleName,
                                                         result.pinsLinks);

    return result;
}

BankCommonInterface::GetProjectCache BankCommonInterface::getProjectCacheValuePriv(
                                                                    const QString &seqToken,
                                                                    const QString &cacheKey) const
{
    GetProjectCache result { false, QVariant() };

    result.success = getProjectCacheValueImpl(seqToken, cacheKey, result.cacheValue);

    return result;
}

bool BankCommonInterface::getRoutesLinkedToTopologyModuleImpl(
                                                const QString &bankUniqueName,
                                                const QString &uniqueTopologyModuleName,
                                                const QString &uniqueTopologyInstanceModuleName,
                                                JsonArray &pinsLinks) const
{
    Q_UNUSED(bankUniqueName)
    Q_UNUSED(uniqueTopologyModuleName)
    Q_UNUSED(uniqueTopologyInstanceModuleName)
    Q_UNUSED(pinsLinks)
    NOT_IMPLEMENTED();
    return false;
}

bool BankCommonInterface::activateRouteImpl(const JsonObject &pinsLink)
{
    Q_UNUSED(pinsLink)
    NOT_IMPLEMENTED();
    return false;
}

bool BankCommonInterface::getProjectCacheValueImpl(const QString &seqToken,
                                                   const QString &cacheKey,
                                                   QVariant &cacheValue) const
{
    QSharedPointer<const ProjectFile> projectFile =
                    GlobalManager::instance().getProjectMngr().tryToGetLinkedProjectFile(seqToken);

    if(projectFile.isNull())
    {
        // The sequence is null, can't process
        return false;
    }

    cacheValue = projectFile->getCache().getValue(cacheKey);
    return true;
}

bool BankCommonInterface::setProjectCacheValueImpl(const QString &seqToken,
                                                   const QString &cacheKey,
                                                   const QVariant &cacheValue)
{
    QSharedPointer<ProjectFile> projectFile =
                GlobalManager::instance().getProjectMngr().tryToAccessLinkedProjectFile(seqToken);

    if(projectFile.isNull())
    {
        // The sequence is null, can't process
        return false;
    }

    projectFile->accessCache().setValue(cacheKey, cacheValue);
    return true;
}

const ISequenceLogManager *BankCommonInterface::getSequenceLogManagerImpl(
                                                        const QString &sequenceUniqueToken) const
{
    QSharedPointer<const NodesSequence> nodesSequence = GlobalManager::instance()
                            .accessProjectMngr().tryToGetLinkedNodesSequence(sequenceUniqueToken);

    if(!nodesSequence)
    {
        return nullptr;
    }

    return &nodesSequence->getSequenceLogManager();
}
