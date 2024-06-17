// SPDX-FileCopyrightText: 2024 Hugo Landre <hugo.landre@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "appapic.h"

#include "appapicmanager.hpp"


AppApiCCatalogStruct *getSequenceModules()
{
    return AppApiCManager::instance().getSequenceModules();
}

AppApiCCatalogStruct *reloadSequenceModules()
{
    return AppApiCManager::instance().reloadSequenceModules();
}

bool checkJson(char *json, char **error, char **nodeName)
{
    return AppApiCManager::instance().checkJson(json, error, nodeName);
}

bool getVersion(char **version)
{
    return AppApiCManager::instance().getVersion(version);
}

void freeCatalog(AppApiCCatalogStruct *catalog)
{
    for(int i = 0; i < catalog->count; i++)
    {
        AppApiCPluginStruct p = catalog->plugins[i];
        for(int j = 0; j < p.count; j++)
        {
            AppApiCModuleStruct m = p.modules[j];
            delete []m.name;
            delete []m.parameters;
            delete []m.inputs;
            delete []m.outputs;
        }
        delete []p.name;
        delete []p.modules;
    }

    delete []catalog->plugins;
    delete catalog;

}

void freeString(char *p)
{
    delete []p;
}
