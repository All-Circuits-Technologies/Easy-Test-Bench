// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "projectcache.hpp"


ProjectCache::ProjectCache(QObject *parent) : QObject(parent)
{
}

QVariant ProjectCache::getValue(const QString &key) const
{
    return _projectCache.value(key, QVariant());
}

void ProjectCache::setValue(const QString &key, const QVariant &value)
{
    _projectCache[key] = value;
}
