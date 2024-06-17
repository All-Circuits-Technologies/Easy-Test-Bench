// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "dbobject.hpp"


DbObject::DbObject(qint64 id, const QDateTime &lastModified, QObject *parent):
    QObject(parent),
    _id(id),
    _lastModified(lastModified)
{
}

DbObject::DbObject(QObject *parent) :
    QObject(parent),
    _id(defaultId)
{
}
