// SPDX-FileCopyrightText: 2019 - 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
// SPDX-FileCopyrightText: 2019 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphaël Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "globalmanager.hpp"

#include <QCoreApplication>
#include <QDebug>
#include <QSettings>
#include <QTimer>
#include <QTranslator>

#include "bank/bankmanager.hpp"
#include "core/coreconstants.hpp"
#include "core/settingsmanager.hpp"
#include "database/dbmanager.hpp"
#include "jsonutility/jsonhelper.hpp"
#include "logsutility/loggingoption.hpp"
#include "logsutility/logsmanager.hpp"
#include "permission/authenticationmanager.hpp"
#include "permission/permissionmanager.hpp"
#include "project/projectmanager.hpp"
#include "testbedcore/customevents.hpp"
#include "testbedcore/logs/sequencelogcategory.hpp"
#include "testbedcore/logs/sequencelogdata.hpp"
#include "numberutility/number.hpp"
#include "testbedcore/utility/sequencestatehelper.hpp"
#include "ticutility/ticmanager.hpp"
#include "topology/topologyfile.hpp"
#include "translationutility/translationhelper.hpp"
#include "yamlutility/yamltojsonmanager.hpp"


GlobalManager* GlobalManager::_instance = nullptr;


GlobalManager::GlobalManager(QObject *parent) :
    QObject(parent)
{
    // Constructor voluntary empty to avoid cyclic redundancy during initialization of its members.
    // See init.
}

void GlobalManager::create(ADisplayUserInterface *displayUserInterface)
{
    if(Q_LIKELY(_instance == nullptr))
    {
        _instance = new GlobalManager();
        _instance->init(displayUserInterface);
    }
    else
    {
        qWarning() << "Already instanciated";
    }
}

GlobalManager &GlobalManager::instance()
{
    if(Q_UNLIKELY(!_instance))
    {
        qWarning() << "The global manager wasn't previously created ; therefore, create it";
        create();
    }

    return *_instance;
}

void GlobalManager::registerTypes()
{
    CustomEvents::registerEventTypes();
    Number::RegisterMetaTypes();
    TopologyFile::RegisterMetaTypes();
    JsonHelper::RegisterMetaTypes();
    SequenceStateHelper::registerMetaTypes();
    SequenceLogCategory::RegisterMetaTypes();
    SequenceLogData::RegisterMetaTypes();
    BankManager::RegisterMetaTypes();
    LoggingOption::RegisterMetaTypes();
    PermissionManager::RegisterMetaTypes();
    TicManager::registerMetaTypes();
}

void GlobalManager::setDefaultLibraryInformations()
{
    QString organizationName;

#ifdef ORGANIZATION_NAME
    organizationName = ORGANIZATION_NAME;
#else
    organizationName = CoreConstants::defaultOrganizationName;

    qInfo() << "No organization name has been set in the application .pro file (need to define "
            << "the global variable: ORGANIZATION_NAME), use the default one: " << organizationName;
#endif

    QCoreApplication::setOrganizationName(organizationName);

    QSettings::setDefaultFormat(QSettings::IniFormat);
}

void GlobalManager::init(ADisplayUserInterface *displayUserInterface)
{
    // Set the default library informations
    setDefaultLibraryInformations();

    // Register the meta types of objects
    registerTypes();

    _settingsManager = new SettingsManager(this);

    if(!_settingsManager->load())
    {
        qWarning() << "A problem occurred when trying to parse the settings";
    }

    // Translate the lib
    QString fileName = QString(PRO_FILE_NAME);
    QString filePath;

    if(!_settingsManager->read(KnownSettings::TranslateFilesFolderPath, filePath))
    {
        qWarning() << "Can't get the folder path of translate files";
    }
    else if(!TranslationHelper::translate(fileName, filePath, this))
    {
        qWarning() << "A problem occurred when trying to load the translate file: " << fileName
                   << ", located here: " << filePath;
    }

    _logsManager = &LogsManager::Instance();

    QString dbFilePath;
    if(!_settingsManager->read(KnownSettings::DatabaseFilePath, dbFilePath))
    {
        qWarning() << "Can't get the database file path; therefore the database will be "
                   << "unavailable";
    }

    _dbManager = new DbManager();

    if(!_dbManager->initDbManager(dbFilePath))
    {
        qCritical() << "Can't init dbManager, the database will be unavailable";
    }

    _projectManager = new ProjectManager(*_dbManager, this);
    _bankManager = new BankManager(*_settingsManager, displayUserInterface, *_dbManager, this);

    _authenticationManager = new AuthenticationManager(*_dbManager);
    _permissionManager = new PermissionManager(*_dbManager, *_authenticationManager);
    _ticManager = new TicManager(this);
    createAndInitYamlManager(*_settingsManager);

    // Unload projects when the user disconnects
    connect(_authenticationManager, &AuthenticationManager::userDisconnected,
            _projectManager,        &ProjectManager::unloadAllProjects);

    QTimer::singleShot(0, this, &GlobalManager::onEventLoopReady);
}

void GlobalManager::onEventLoopReady()
{
    initLogsManager();
    qInfo() << "Application launched";
}

void GlobalManager::initLogsManager()
{
    QVariant tmpSetting;

    if(!_settingsManager->read(KnownSettings::LogConsoleCriticity, tmpSetting))
    {
        qWarning() << "Can't read the known settings: "
                   << KnownSettings::toString(KnownSettings::LogConsoleCriticity);
    }
    else
    {
        LogMsgType::Enum criticity = tmpSetting.value<LogMsgType::Enum>();

        if(criticity == LogMsgType::Unknown)
        {
            qWarning() << "The value return by the setting: "
                       << KnownSettings::toString(KnownSettings::LogConsoleCriticity)
                       << ", is not a LogMsgType enum";
        }
        else if(!_logsManager->setLogConsoleStrategy(
                                                LoggingStrategyOption::Glob_DisplayLogContext |
                                                LoggingStrategyOption::Glob_DisplayLogLevel   |
                                                LoggingStrategyOption::Glob_DisplayDateTime,
                                                criticity))
        {
            qWarning() << "Can't set log console strategy";
        }
    }

    QVariant tmpLogFolder;
    QVariant tmpFileNamePattern;

    if(!_settingsManager->read(KnownSettings::LogAppFolderPath, tmpLogFolder))
    {
        qWarning() << "Can't read the known settings: "
                   << KnownSettings::toString(KnownSettings::LogAppFolderPath);
    }
    else if(!_settingsManager->read(KnownSettings::LogFileNamePattern, tmpFileNamePattern))
    {
        qWarning() << "Can't read the known settings: "
                   << KnownSettings::toString(KnownSettings::LogFileNamePattern);
    }
    else if(!_settingsManager->read(KnownSettings::LogFileSavingCriticity, tmpSetting))
    {
        qWarning() << "Can't read the known settings: "
                   << KnownSettings::toString(KnownSettings::LogFileSavingCriticity);
    }
    else
    {
        LogMsgType::Enum criticity = tmpSetting.value<LogMsgType::Enum>();

        if(criticity == LogMsgType::Unknown)
        {
            qWarning() << "The value return by the setting: "
                       << KnownSettings::toString(KnownSettings::LogFileSavingCriticity)
                       << ", is not a LogMsgType enum";
        }
        else if(!_logsManager->setSavingLogFileStrategy(
                                                LoggingStrategyOption::File_OneFilePerDay      |
                                                LoggingStrategyOption::Glob_DisplayLogContext  |
                                                LoggingStrategyOption::Glob_DisplayLogLevel    |
                                                LoggingStrategyOption::Glob_DisplayDateTime    |
                                                LoggingStrategyOption::File_StoreInMonthFolder,
                                                tmpLogFolder.toString(),
                                                tmpFileNamePattern.toString(),
                                                criticity))
        {
            qWarning() << "Can't set the log saving file strategy";
        }
    }
}

void GlobalManager::createAndInitYamlManager(SettingsManager &settingsMngr)
{
    _yamlManager = new YamlToJsonManager();

    QString yqProgramPath;
    QString yamlWorkerDirPath;

    if(!settingsMngr.read(KnownSettings::YqProgramPath, yqProgramPath) ||
       !settingsMngr.read(KnownSettings::YamlWorkerDirPath, yamlWorkerDirPath))
    {
        qWarning() << "A problem occurred when tried to load settings linked to YAML";
        return;
    }

    if(!_yamlManager->initManager(yqProgramPath, yamlWorkerDirPath))
    {
        qWarning() << "A problem occurred when tried to initialize the YAML manager";
    }
}
