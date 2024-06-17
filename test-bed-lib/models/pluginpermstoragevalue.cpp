// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "pluginpermstoragevalue.hpp"


PluginPermStorageValue::PluginPermStorageValue(QObject *parent) : DbObject(parent)
{
}

PluginPermStorageValue::PluginPermStorageValue(const QString &key,
                                               const QString &value,
                                               QObject *parent) :
    DbObject(parent),
    _key(key),
    _value(value)
{
}

PluginPermStorageValue::PluginPermStorageValue(qint64 id,
                                               const QString &key,
                                               const QString &value,
                                               const QString &note,
                                               const QDateTime &lastModified,
                                               QObject *parent) :
    DbObject(id, lastModified, parent),
    _key(key),
    _value(value),
    _note(note)
{
}
