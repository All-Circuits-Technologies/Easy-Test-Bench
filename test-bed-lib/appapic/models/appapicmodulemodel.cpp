// SPDX-FileCopyrightText: 2024 Hugo Landre <hugo.landre@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "appapicmodulemodel.hpp"

#include "appapic/appapicqstringcopyhelper.hpp"


AppApiCModuleModel::AppApiCModuleModel(const QString &name,
                                       const QString &parameters,
                                       const QString &inputs,
                                       const QString &outputs) :
    _name(name),
    _parameters(parameters),
    _inputs(inputs),
    _outputs(outputs)
{
}

AppApiCModuleModel::~AppApiCModuleModel()
{
}

AppApiCModuleStruct AppApiCModuleModel::toStruct() const
{
    return AppApiCModuleStruct{AppApiCQStringCopyHelper::copyQString(_name),
                               AppApiCQStringCopyHelper::copyQString(_parameters),
                               AppApiCQStringCopyHelper::copyQString(_inputs),
                               AppApiCQStringCopyHelper::copyQString(_outputs)};
}

