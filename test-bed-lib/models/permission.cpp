// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "permission.hpp"


Permission::Permission(const QString &name,
                       const QString &description,
                       QObject *parent):
                       DbObject(parent),
                       _name(name),
                       _description(description)
{
}

Permission::Permission(QObject *parent) :
    DbObject(parent)
{
}
