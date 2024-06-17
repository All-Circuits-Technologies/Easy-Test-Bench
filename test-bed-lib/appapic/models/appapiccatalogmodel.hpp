// SPDX-FileCopyrightText: 2024 Hugo Landre <hugo.landre@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include <QList>

#include "appapiccatalogstruct.h"
#include "appapicpluginmodel.hpp"


/** @brief Catalog model class */
class AppApiCCatalogModel
{

    public:
        /** @brief class constuctor */
        explicit AppApiCCatalogModel() = default;

        /** class destructor */
        virtual ~AppApiCCatalogModel();

    public:
        /** class constructor
         *  @param plugins The catalog plugins */
        explicit AppApiCCatalogModel(const QVector<AppApiCPluginModel> &plugins);

    public:
        /** @brief return the catalog as a structure
         *  @return the catalog as a structure */
        AppApiCCatalogStruct toStruct() const;

    private:
        QVector<AppApiCPluginModel> _plugins;

};
