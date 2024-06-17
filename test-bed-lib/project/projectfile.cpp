// SPDX-FileCopyrightText: 2019 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "projectfile.hpp"

#include <QDebug>
#include <QFile>
#include <QFileInfo>

#include "core/globalmanager.hpp"
#include "database/databaseauthinfo.hpp"
#include "errors/helpers/fileloadingerrortypehelper.hpp"
#include "jsonparsers/projectfilejsonparser.hpp"
#include "project/projectcache.hpp"
#include "testbedcore/testbedglobal.hpp"
#include "topology/topologyfile.hpp"

#include "yamlutility/yamltojsonmanager.hpp"


ProjectFile::ProjectFile(QObject *parent) :
    AJsonFile(nullptr, parent),
    _projectCache(new ProjectCache(this))
{
}

void ProjectFile::addSequenceFile(const QString &fileName,
                                  const QSharedPointer<SequenceFile> &sequence)
{
    _sequenceFiles.insert(fileName, sequence);
}

void ProjectFile::addTopologyFile(const QSharedPointer<TopologyFile> &topology)
{
    _topologyFiles.append(topology);
}

bool ProjectFile::loadAndParseProjectFile(const QString &filePath,
                                          FileLoadingErrorHandler *errorHandler)
{
    QFile *projectFile = new QFile(filePath, this);

    if(!projectFile->exists())
    {
        FileLoadingErrorTypeHelper::addMissingProjectFileError(filePath, errorHandler);
        qWarning() << "The project file given doesn't exist: " << filePath;
        delete projectFile;
        return false;
    }

    const QFileInfo projectInfo(*projectFile);
    const QString suffix = projectInfo.suffix();

    if(suffix == ProjectFile::YamlSuffix)
    {
        QFile *jsonProjectFile = new QFile(this);
        if(!GlobalManager::instance().accessYamlMngr().loadYamlAndConvertToJson(*projectFile,
                                                                                *jsonProjectFile)) {
            FileLoadingErrorTypeHelper::addUnsupportedFileFormatError(suffix, errorHandler);
            delete jsonProjectFile;
            delete projectFile;
            return false;
        }

        delete projectFile;
        setAliasPath(filePath);
        projectFile = jsonProjectFile;
    }
    else if(suffix != ProjectFile::JsonSuffix)
    {
        qWarning() << "The file hasn't the expected format: " << filePath;
        FileLoadingErrorTypeHelper::addUnsupportedFileFormatError(suffix, errorHandler);
        delete projectFile;
        return false;
    }

    // Set device for reading and writting
    setFile(projectFile);

    if(!ProjectFileJsonParser::loadAndParseProjectFile(*projectFile, *this, errorHandler))
    {
        qWarning() << "A problem occurred when trying to load and parse the file: " << filePath
                   << ", with the alias: " << getAliasPath();
        setFile(nullptr);
        return false;
    }

    return true;
}

void ProjectFile::setDatabaseAuthInfo(DatabaseAuthInfo *dbAuthInfo)
{
    delete _dbAuthInfo;

    _dbAuthInfo = dbAuthInfo;
    _dbAuthInfo->setParent(this);
}
