// SPDX-FileCopyrightText: 2024 Hugo Landre <hugo.landre@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "appapicmanager.hpp"
#include "appapicqstringcopyhelper.hpp"

#include <windows.h>

#include <QCoreApplication>

#include "concurrent/threadconcurrentrun.hpp"

#include "appapicjsoncheckerservice.hpp"
#include "appapicmodulegetterservice.hpp"
#include "appapic/models/appapiccatalogstruct.h"
#include "appapic/models/appapiccatalogmodel.hpp"
#include "core/globalmanager.hpp"

AppApiCManager *AppApiCManager::_instance = nullptr;


AppApiCManager::~AppApiCManager()
{
    delete _moduleGetter;
    delete _jsonChecker;
}

AppApiCManager::AppApiCManager()
{
}


AppApiCManager &AppApiCManager::instance()
{
    if(!_instance)
    {
        _instance = new AppApiCManager();
        _instance->init();
    }
    return *_instance;
}

void AppApiCManager::init()
{
    unsigned long ThreadID;
    void *th = CreateThread(NULL, 0, qtThreadFunc, this, 0, &ThreadID);

    _ready = _semReady.tryAcquire(1, ThreadReadyTimeoutInMs);
}

unsigned long AppApiCManager::qtThreadFunc(void *params)
{
    int argc = Argc;
    char *argv[] = {(char*)Argv[0]};
    QCoreApplication app(argc, argv);
    GlobalManager::create();

    AppApiCManager *manager = (AppApiCManager *)params;
    manager->_moduleGetter = new AppApiCModuleGetterService();
    manager->_jsonChecker = new AppApiCJsonCheckerService();

    manager->_semReady.release();
    return app.exec();
}

AppApiCCatalogStruct *AppApiCManager::getSequenceModules()
{
    if (!_ready)
    {
        /** AppApiCManager did not initialize correctly */
        qWarning() << "AppApiCManager didn't initialize correctly";
        return nullptr;
    }
    _mutex.lock();
    AppApiCCatalogModel c = ThreadConcurrentRun::run(*_moduleGetter, &AppApiCModuleGetterService::getSequenceModules);
    _mutex.unlock();
    return new AppApiCCatalogStruct(c.toStruct());
}

AppApiCCatalogStruct *AppApiCManager::reloadSequenceModules()
{
    if (!_ready)
    {
        qWarning() << "AppApiCManager didn't initialize correctly";
        return nullptr;
    }
    _mutex.lock();
    AppApiCCatalogModel c = ThreadConcurrentRun::run(*_moduleGetter, &AppApiCModuleGetterService::reloadSequenceModules);
    _mutex.unlock();
    return new AppApiCCatalogStruct(c.toStruct());
}

bool AppApiCManager::checkJson(char *json, char **error, char **nodeName)
{
    if (!_ready || !json || !error || !nodeName)
    {
        qWarning() << "AppApiCManager didn't initialize correctly or parameters are nullptr";
        return false;
    }
    return _jsonChecker->checkJson(json, error, nodeName);
}

bool AppApiCManager::getVersion(char **version)
{
    if (!_ready || !version)
    {
        qWarning() << "AppApiCManager didn't initialize correctly or parameter is nullptr";
        return false;
    }
    *version = AppApiCQStringCopyHelper::copyQString(Version);

    return true;
}
