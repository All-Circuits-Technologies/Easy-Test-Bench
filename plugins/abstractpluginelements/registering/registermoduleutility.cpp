// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "registermoduleutility.hpp"


void RegisterModuleUtility::insertInList(const QMetaObject &metaObject)
{
    _registeredModules.insert(QString(metaObject.className()).toLower(), metaObject);
}
