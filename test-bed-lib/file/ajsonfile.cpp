// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "ajsonfile.hpp"

#include <QDebug>
#include <QFile>
#include <QFileInfo>


AJsonFile::AJsonFile(QFile *jsonFile, QObject *parent) :
    QObject(parent)
{
    setFile(jsonFile);
}

QString AJsonFile::getAbsoluteFilePath() const
{
    if(_file == nullptr)
    {
        return {};
    }

    if(!_aliasPath.isEmpty())
    {
        return QFileInfo(_aliasPath).absoluteFilePath();
    }

    return QFileInfo(*_file).absoluteFilePath();
}

QString AJsonFile::getAbsoluteDirPath() const
{
    if(_file == nullptr)
    {
        return {};
    }

    if(!_aliasPath.isEmpty())
    {
        return QFileInfo(_aliasPath).absolutePath();
    }

    return QFileInfo(*_file).absolutePath();
}

void AJsonFile::setFile(QFile *file)
{
    delete _file;

    _file = file;

    if(_file)
    {
        _file->setParent(this);
    }
}
