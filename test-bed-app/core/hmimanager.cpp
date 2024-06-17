// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "core/hmimanager.hpp"

#include "blocs/blocsregistrator.hpp"
#include "core/configurationmanager.hpp"
#include "logsutility/logmsgtype.hpp"
#include "models/modelsregistrator.hpp"
#include "services/servicesregistrator.hpp"
#include "services/sizemanager.hpp"
#include "services/navigation/navigationmanager.hpp"
#include "test-bed-lib/core/globalmanager.hpp"
#include "translationutility/translationhelper.hpp"
#include "types/typesregistrator.hpp"

#include <QtDebug>
#include <QScreen>
#include <QGuiApplication>

HmiManager* HmiManager::_instance = nullptr;


HmiManager::HmiManager(QObject *parent) :
    QObject(parent)
{
    // Constructor voluntary empty to avoid cyclic redundancy during initialization of its members.
    // See init.
}

void HmiManager::create()
{
    if(Q_LIKELY(_instance == nullptr))
    {
        _instance = new HmiManager();
        _instance->init();
    }
    else
    {
        qWarning() << "Already instanciated";
    }
}

HmiManager &HmiManager::instance()
{
    if(Q_UNLIKELY(!_instance))
    {
        qWarning() << "The hmi manager wasn't previously created ; therefore, create it";
        create();
    }

    return *_instance;
}

void HmiManager::registerMetaTypes()
{
    BlocsRegistrator::registerMetaTypes();
    ModelsRegistrator::registerMetaTypes();
    ServicesRegistrator::registerMetaTypes();
    TypesRegistrator::registerMetaTypes();
    LogMsgType::registerMetaType();
}

void HmiManager::registerQmlTypes()
{
    BlocsRegistrator::registerQmlTypes();
    ModelsRegistrator::registerQmlTypes();
    ServicesRegistrator::registerQmlTypes();
    TypesRegistrator::registerQmlTypes();
}

void HmiManager::init()
{
    _displayAppInterface = new AppInterfaceManager();
    GlobalManager::create(_displayAppInterface);
    _settingsMngr = &GlobalManager::instance().accessSettingsMngr();

    initTranslations();

    registerMetaTypes();

    _navigationManager = new NavigationManager(this);

    QRect  screenSize = QGuiApplication::primaryScreen()->geometry();
    int height = screenSize.height();
    int width = screenSize.width();

    _sizeManager = new SizeManager(height,
                                   width,
                                   SizeManager::MockupHeight,
                                   SizeManager::MockupWidth,
                                   this);

    _configurationManager = new ConfigurationManager();

    // Because we register some managers, we register QML types after the managers creation
    registerQmlTypes();
}

void HmiManager::initTranslations()
{
    QString fileName = QString(PRO_FILE_NAME);
    QString filePath;

    if(!_settingsMngr->read(KnownSettings::TranslateFilesFolderPath, filePath))
    {
        qWarning() << "Can't get the folder path of translate files";
    }
    else if(!TranslationHelper::translate(fileName, filePath, this))
    {
        qWarning() << "A problem occurred when trying to load the translate file: " << fileName
                   << ", located here: " << filePath;
    }
}
