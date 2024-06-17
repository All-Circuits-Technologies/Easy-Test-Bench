// SPDX-FileCopyrightText: 2024 Hugo Landre <hugo.landre@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "appapicpluginstruct.h"


/** @brief Catalog structure */
typedef struct AppApiCCatalogStruct
{

    /** @brief the plugins */
    AppApiCPluginStruct *plugins;

    /** @brief the number of plugins */
    int count;

}AppApiCCatalogStruct;

