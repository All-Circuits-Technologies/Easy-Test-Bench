// SPDX-FileCopyrightText: 2024 Hugo Landre <hugo.landre@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once


/** Module structure */
typedef struct AppApiCModuleStruct
{

    /** @brief the module name */
    char *name;

    /** @brief the module parameters format */
    char *parameters;

    /** @brief the module inputs format */
    char *inputs;

    /** @brief the module outputs format */
    char *outputs;

} AppApiCModuleStruct;

