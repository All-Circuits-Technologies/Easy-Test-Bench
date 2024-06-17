// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "models/menuitemmodel.hpp"

#include <QDebug>


MenuItemModel::MenuItemModel(QObject *parent) : QObject(parent)
{
}

void MenuItemModel::setName(const QString &name)
{
    if(_name != name)
    {
        _name = name;
        emit nameChanged(_name);
    }
}

void MenuItemModel::setEnabled(bool enabled)
{
    if(_isEnabled != enabled)
    {
        _isEnabled = enabled;
        emit enabledChanged(_isEnabled);
    }
}

void MenuItemModel::itemClicked(bool hasClicked)
{
    Q_UNUSED(hasClicked)
    emit clicked();
}
