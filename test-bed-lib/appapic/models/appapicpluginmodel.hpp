// SPDX-FileCopyrightText: 2024 Hugo Landre <hugo.landre@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QList>

#include "appapicmodulemodel.hpp"
#include "appapicpluginstruct.h"


/** @brief Plugin model class */
class AppApiCPluginModel
{

    public:
        /** @brief class constructor
         *  @param name The plugin name
         *  @param modules The plugin modules */
        explicit AppApiCPluginModel(const QString &name,
                                    const QVector<AppApiCModuleModel> &modules);

        /** class destructor */
        virtual ~AppApiCPluginModel();

    public:
        /** @brief return the plugin as a structure
         *  @return the plugin as a structure */
        AppApiCPluginStruct toStruct() const;

    private:
        QString _name;
        QVector<AppApiCModuleModel> _modules;
};
