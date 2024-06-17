// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "projectfilejsonparser.hpp"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonObject>

#include "core/coreconstants.hpp"
#include "errors/helpers/fileloadingerrorcausehelper.hpp"
#include "errors/helpers/fileloadingerrorcausehelper.hpp"
#include "errors/helpers/fileloadingerrortypehelper.hpp"
#include "jsonparsers/databaseinfosjsonparser.hpp"
#include "jsonutility/jsonhelper.hpp"
#include "project/projectfile.hpp"
#include "sequence/sequencefile.hpp"
#include "testbedcore/parsers/jsonparsersconstants.hpp"
#include "testbedcore/testbedglobal.hpp"
#include "topology/topologyfile.hpp"


ProjectFileJsonParser::ProjectFileJsonParser(const QJsonObject &rootObject,
                                             ProjectFile &projectFile,
                                             QObject *parent) :
    AJsonParser(parent),
    _rootObject(rootObject),
    _projectFile(projectFile)
{
}

bool ProjectFileJsonParser::loadAndParseProjectFile(QIODevice &projectDevice,
                                                    ProjectFile &projectFile,
                                                    FileLoadingErrorHandler *errorHandler)
{
    QJsonObject jsonObject;

    TEST_AND_RETURN_IF_FALSE(loadJsonFileAndGetObject(projectDevice, jsonObject),
                           FileLoadingErrorTypeHelper::addInaccessibleFileError(errorHandler););

    ProjectFileJsonParser projectParser(jsonObject, projectFile);

    TEST_AND_RETURN_IF_FALSE(projectParser.loadProjectName(errorHandler),
                           FileLoadingErrorTypeHelper::addUnreadableProjectError(errorHandler););

    TEST_AND_RETURN_IF_FALSE(projectParser.loadTopologyFiles(errorHandler),
                           FileLoadingErrorTypeHelper::addUnreadableProjectError(errorHandler););

    TEST_AND_RETURN_IF_FALSE(projectParser.loadSequenceFilesHeaders(errorHandler),
                           FileLoadingErrorTypeHelper::addUnreadableProjectError(errorHandler););

    return true;
}

bool ProjectFileJsonParser::loadProjectName(FileLoadingErrorHandler *errorHandler)
{
    QString value;

    TEST_AND_RETURN_IF_FALSE(JsonHelper::getString(_rootObject,
                                          JsonParsersConstants::Project::ProjectName,
                                          value),
                           FileLoadingErrorCauseHelper::addUnavailableAttributeError(
                                                      JsonParsersConstants::Project::ProjectName,
                                                      errorHandler););

    _projectFile.setName(value);

    return true;
}

bool ProjectFileJsonParser::loadTopologyFiles(FileLoadingErrorHandler *errorHandler)
{
    QStringList topologyFilesPath;

    TEST_AND_RETURN_IF_FALSE(JsonHelper::getStringList(_rootObject,
                                              JsonParsersConstants::Project::TopologyFilesPathName,
                                              topologyFilesPath),
                    FileLoadingErrorCauseHelper::addUnavailableAttributeError(
                                              JsonParsersConstants::Project::TopologyFilesPathName,
                                              errorHandler););

    for(const QString &topologyFilePath : topologyFilesPath)
    {
        /* Insert unloaded TopologyFile into array.
         * Each of them can be loaded later using TopologyFileJsonParser
         */
        _projectFile.addTopologyFile(QSharedPointer<TopologyFile>(
                                         new TopologyFile(topologyFilePath)));
    }

    return true;
}

bool ProjectFileJsonParser::loadSequenceFilesHeaders(FileLoadingErrorHandler *errorHandler)
{
    QStringList sequenceFilesPath;

    TEST_AND_RETURN_IF_FALSE(JsonHelper::getStringList(_rootObject,
                                              JsonParsersConstants::Project::SequenceFilesPathName,
                                              sequenceFilesPath),
                    FileLoadingErrorCauseHelper::addUnavailableAttributeError(
                                              JsonParsersConstants::Project::SequenceFilesPathName,
                                              errorHandler););

    QString projectAbsDirPath = _projectFile.getAbsoluteDirPath();
    for(const QString &sequenceFilePath : sequenceFilesPath)
    {
        QFileInfo fileDetails(projectAbsDirPath, sequenceFilePath);

        if(!CoreConstants::sequenceFileSuffixes.contains(fileDetails.suffix()))
        {
            FileLoadingErrorCauseHelper::addSuffixError(errorHandler);
            qWarning() << "The suffix of the sequence file is not known, can't load it: "
                       << sequenceFilePath;
            return false;
        }

        // Get the projectFile folder name, get in, add the sequenceFile name
        QString sequenceFullPath = fileDetails.absoluteFilePath();

        QFile *sequenceFile = new QFile(sequenceFullPath);

        if(!sequenceFile->exists())
        {
            FileLoadingErrorCauseHelper::addMissingSequenceFileError(sequenceFilePath,
                                                                     errorHandler);
            qWarning() << "The sequence file doesn't exist: " << sequenceFilePath;
            delete sequenceFile;
            return false;
        }

        SequenceFile *tmpSequFile = new SequenceFile();

        if(!tmpSequFile->initAndSetSequenceFile(sequenceFile, errorHandler))
        {
            delete tmpSequFile;
            return false;
        }

        _projectFile.addSequenceFile(QFileInfo(tmpSequFile->getAbsoluteFilePath()).baseName(),
                                     QSharedPointer<SequenceFile>(tmpSequFile));
    }

    return true;
}
