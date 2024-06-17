// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "filebrowermodule.hpp"


FileBrowerModule::FileBrowerModule(QObject *parent) :
    AFileBrowserModule(staticMetaObject.className(), parent)
{
}

bool FileBrowerModule::getDefaultValue(const JsonArray &jsonObjectInputValues,
                                       QString &defaultValue)
{
    // Nothing to do
    Q_UNUSED(jsonObjectInputValues)
    Q_UNUSED(defaultValue)
    return true;
}

bool FileBrowerModule::saveChosenValue(const QString &value)
{
    // Nothing to do
    Q_UNUSED(value)
    return true;
}
