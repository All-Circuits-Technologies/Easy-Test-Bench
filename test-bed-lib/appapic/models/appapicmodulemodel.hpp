// SPDX-FileCopyrightText: 2024 Hugo Landre <hugo.landre@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QString>

#include "appapicmodulestruct.h"


/** @brief Module model class */
class AppApiCModuleModel
{

    public:
        /** @brief class constuctor */
        explicit AppApiCModuleModel() = default;

        /** class destructor */
        virtual ~AppApiCModuleModel();

    public:
        /** @brief class constructor
         *  @param name The module name
         *  @param parameters The module parameters format
         *  @param inputs The module inputs format
         *  @param outputs The module outputs format */
        explicit AppApiCModuleModel(const QString &name,
                                    const QString &parameters,
                                    const QString &inputs,
                                    const QString &outputs);

    public:
        /** @brief return the module as a structure
         *  @return the module as a structure */
        AppApiCModuleStruct toStruct() const;

    private:
        QString _name;
        QString _parameters;
        QString _inputs;
        QString _outputs;
};
