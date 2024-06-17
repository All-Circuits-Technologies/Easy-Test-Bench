// SPDX-FileCopyrightText: 2024 Hugo Landre <hugo.landre@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "appapiccatalogmodel.hpp"


AppApiCCatalogModel::AppApiCCatalogModel(const QVector<AppApiCPluginModel> &plugins)
    : _plugins(plugins)
{}

AppApiCCatalogModel::~AppApiCCatalogModel()
{
}

AppApiCCatalogStruct AppApiCCatalogModel::toStruct() const
{
    const int count = _plugins.size();
    AppApiCPluginStruct *plugins = new AppApiCPluginStruct[count];

    for(int i = 0; i < count; i++)
    {
        plugins[i] = _plugins.at(i).toStruct();
    }

    return AppApiCCatalogStruct{plugins, count};
}

