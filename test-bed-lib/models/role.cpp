// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "role.hpp"

Role::Role(const QString &name, QObject* parent) :
    DbObject(parent),
    _name(name)
{
}

Role::Role(QObject* parent) :
    DbObject(parent)
{
}
