// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "models/filelistitemmodel.hpp"

#include <QQmlEngine>


FileListItemModel::FileListItemModel(QObject *parent) : QObject(parent)
{
}

void FileListItemModel::setName(const QString &name)
{
    if(_name != name)
    {
        _name = name;
        emit nameChanged(_name);
    }
}

void FileListItemModel::setUniquePath(const QString &uniquePath)
{
    if(_uniquePath != uniquePath)
    {
        _uniquePath = uniquePath;
        emit uniquePathChanged(_uniquePath);
    }
}

void FileListItemModel::setMissingFile(bool missingFile)
{
    if(_missingFile != missingFile)
    {
        _missingFile = missingFile;
        emit missingFileChanged(_missingFile);
    }
}
