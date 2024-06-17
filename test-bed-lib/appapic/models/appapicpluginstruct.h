// SPDX-FileCopyrightText: 2024 Hugo Landre <hugo.landre@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "appapicmodulestruct.h"


/** Plugin structure */
typedef struct AppApiCPluginStruct
{

    /** @brief the plugin name */
    char *name;

    /** @brief the modules */
    AppApiCModuleStruct *modules;

    /** @brief the number of modules */
    int count;

} AppApiCPluginStruct;

