// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "bankmanager.hpp"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QLibrary>
#include <QPluginLoader>
#include <QTranslator>

#include "bank/displayuitoken.hpp"
#include "bankcoreshared/usebylib/iplugin.hpp"
#include "collectionutility/qhashhelper.hpp"
#include "core/coreconstants.hpp"
#include "core/settingsmanager.hpp"
#include "testbedcore/testbedglobal.hpp"
#include "translationutility/translationhelper.hpp"


BankManager::BankManager(const SettingsManager &settingsMngr,
                         ADisplayUserInterface *displayUserInterface,
                         DbManager &dbManager,
                         QObject *parent) :
    QObject(parent),
    _commonInterface(new BankCommonInterface(displayUserInterface, dbManager, this)),
    _settingsMngr(settingsMngr)
{
    reloadBanks();
}

BankManager::~BankManager()
{
    cleanBanksList();
}

QVector<QString> BankManager::getBankNames() const
{
    return QHashHelper::getKeys(_bankInterfaces);
}

QSharedPointer<IBankInterface> BankManager::getBankInterface(const QString &name) const
{
    if(!_bankInterfaces.contains(name))
    {
        return {};
    }

    return _bankInterfaces.value(name).interface;
}

bool BankManager::reloadBanks()
{
    RETURN_IF_FALSE(cleanBanksList());

    QString banksPath;
    QString translateFilesPath;

    RETURN_IF_FALSE(_settingsMngr.read(KnownSettings::PluginPath, banksPath));
    RETURN_IF_FALSE(_settingsMngr.read(KnownSettings::TranslateFilesFolderPath,
                                       translateFilesPath));

    const QString baseForRelativeBanksPathes = QCoreApplication::applicationDirPath();
    QDir appDir(baseForRelativeBanksPathes);

    if(!appDir.cd(banksPath))
    {
        qWarning() << "Can't find the banks folder: " << banksPath << ", from path: "
                   << appDir.path();
        // If the folder doesn't exist, it's not an error
        return true;
    }

    QFileInfoList infoList = appDir.entryInfoList(QDir::Files);
    for(auto citer = infoList.cbegin(); citer != infoList.cend(); ++citer)
    {
        if(!QLibrary::isLibrary(citer->fileName()))
        {
            // We are looking for a library and so we are skipping all the non-library files
            continue;
        }

        BankData bankData;

        qDebug() << "Plugin to load: " << citer->absoluteFilePath();

        bankData.pluginLoader = new QPluginLoader(citer->absoluteFilePath(), this);

        if(bankData.pluginLoader->metaData().value(
                    CoreConstants::Bank::pluginDebugKey).toBool() == CoreConstants::isReleaseMode)
        {
            // Prevent to load debug plugins in release program build (and reversed)
            delete bankData.pluginLoader;
            continue;
        }

        bankData.pluginLoader->setLoadHints(QLibrary::ResolveAllSymbolsHint);

        // (QtDoc): The plugin is loaded if necessary when calling instance()
        IPlugin* plugin = qobject_cast<IPlugin*>(bankData.pluginLoader->instance());

        if(!plugin)
        {
            qWarning() << "Can't load the library: " << citer->fileName() << ", no plugin"
                       << " interface detected: " << bankData.pluginLoader->errorString();
            bankData.pluginLoader->unload();
            bankData.pluginLoader->deleteLater();
            continue;
        }

        QSharedPointer<IBankInterface> bankItf = plugin->accessBankInterface(*_commonInterface);

        if(!bankItf)
        {
            qWarning() << "Can't load the library: " << citer->fileName() << ", no bank"
                       << " interface detected";
            bankData.pluginLoader->unload();
            bankData.pluginLoader->deleteLater();
            continue;
        }

        bankData.interface = bankItf;

        QString bankName;

        if(!bankData.interface->getUniqueName(bankName))
        {
            qWarning() << "A problem occurred when tried to load the library: "
                       << citer->fileName() << " ; can't get the unique bank name";
            bankData.pluginLoader->unload();
            bankData.pluginLoader->deleteLater();
            continue;
        }

        if(!TranslationHelper::translate(bankName, translateFilesPath, this))
        {
            qWarning() << "A problem occurred when trying to load the translate file: " << bankName
                       << ", located here: " << translateFilesPath;
        }


        _bankInterfaces.insert(bankName, bankData);
    }

    return true;
}

void BankManager::RegisterMetaTypes()
{
    DisplayUiToken::RegisterMetaTypes();
}

bool BankManager::cleanBanksList()
{
    QHash<QString, BankData>::iterator iter = _bankInterfaces.begin();

    while(iter != _bankInterfaces.end())
    {
        iter.value().interface.clear();
        iter.value().pluginLoader->unload();
        iter.value().pluginLoader->deleteLater();
        iter = _bankInterfaces.erase(iter);
    }

    return true;
}
