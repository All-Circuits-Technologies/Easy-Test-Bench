// SPDX-FileCopyrightText: 2024 Hugo Landre <hugo.landre@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "appapicpluginmodel.hpp"

#include "appapic/appapicqstringcopyhelper.hpp"


AppApiCPluginModel::AppApiCPluginModel(const QString &name,
                                       const QVector<AppApiCModuleModel> &modules) :
    _name(name),
    _modules(modules)
{
}

AppApiCPluginModel::~AppApiCPluginModel()
{
}

AppApiCPluginStruct AppApiCPluginModel::toStruct() const
{
    const int count = _modules.size();
    AppApiCModuleStruct *modules = new AppApiCModuleStruct[count];

    for(int i = 0; i < count; i++)
    {
        modules[i] = _modules.at(i).toStruct();
    }
    return AppApiCPluginStruct{AppApiCQStringCopyHelper::copyQString(_name), modules, count};
}

