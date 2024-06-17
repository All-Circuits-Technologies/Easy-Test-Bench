// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "projectmanager.hpp"

#include <QDebug>

#include "collectionutility/qvectorhelper.hpp"
#include "database/dbmanager.hpp"
#include "database/dbprojectasker.hpp"
#include "jsonparsers/projectfilejsonparser.hpp"
#include "models/project.hpp"
#include "project/projectfile.hpp"
#include "sequence/sequencefile.hpp"


ProjectManager::ProjectManager(DbManager &dbManager, QObject *parent) :
    QObject(parent),
    _dbProjectAsker(*new DbProjectAsker(dbManager, this))
{
}

ProjectManager::~ProjectManager()
{
}

bool ProjectManager::loadProject(const QString &projectFilePath,
                                 FileLoadingErrorHandler *errorHandler)
{
    ProjectFile *projectFile = loadProjectPriv(projectFilePath, errorHandler);

    if(projectFile == nullptr)
    {
        return false;
    }

    QSharedPointer<Project> project = QSharedPointer<Project>::create();
    project->setPath(projectFile->getAbsoluteFilePath());
    project->setLastOpened(QDateTime::currentDateTime());
    project->setName(projectFile->getName());

    return registerProject(project);
}

bool ProjectManager::loadProject(const QSharedPointer<Project> &project,
                                 FileLoadingErrorHandler *errorHandler)
{
    ProjectFile *projectFile = loadProjectPriv(project->getPath(), errorHandler);

    if(projectFile == nullptr)
    {
        return false;
    }

    project->setLastOpened(QDateTime::currentDateTime());
    project->setName(projectFile->getName());

    return registerProject(project);
}

bool ProjectManager::unloadProject(const QSharedPointer<ProjectFile> &project)
{
    _loadedProjects.removeOne(project);

    return true;
}

bool ProjectManager::unloadFirstProject()
{
    if(!_loadedProjects.isEmpty())
    {
        _loadedProjects.removeAt(0);
    }

    return true;
}

bool ProjectManager::unloadAllProjects()
{
    _loadedProjects.clear();

    return true;
}

QVector<QSharedPointer<const ProjectFile> > ProjectManager::getProjects() const
{
    return QVectorHelper::getConstCast(_loadedProjects);
}

QSharedPointer<const ProjectFile> ProjectManager::tryToGetLinkedProjectFile(
        const QString &sequenceToken) const
{
    return tryToAccessLinkedProjectFile(sequenceToken);
}

QSharedPointer<ProjectFile> ProjectManager::tryToAccessLinkedProjectFile(
        const QString &sequenceToken) const
{
    QVector<QSharedPointer<ProjectFile>>::const_iterator cpiter = _loadedProjects.cbegin();
    for(; cpiter != _loadedProjects.cend(); ++cpiter)
    {
        const QMap<QString, QSharedPointer<SequenceFile>> &sequenceFiles =
                (*cpiter)->getSequenceFiles();

        QMap<QString, QSharedPointer<SequenceFile>>::const_iterator citer = sequenceFiles.cbegin();
        for(; citer != sequenceFiles.cend(); ++citer)
        {
            if(!citer.value()->containsNodesSequence(sequenceToken))
            {
                continue;
            }

            return *cpiter;
        }
    }

    qWarning() << "No project file has been found with the unique token: " << sequenceToken;
    return {};
}

QSharedPointer<const SequenceFile> ProjectManager::tryToGetLinkedSequenceFile(
        const QString &sequenceToken) const
{
    for(const QSharedPointer<ProjectFile> &projectFile : _loadedProjects)
    {
        const QMap<QString, QSharedPointer<SequenceFile>> &sequenceFiles =
                projectFile->getSequenceFiles();

        QMap<QString, QSharedPointer<SequenceFile>>::const_iterator citer = sequenceFiles.cbegin();

        for(; citer != sequenceFiles.cend(); ++citer)
        {
            QSharedPointer<const NodesSequence> nodesSequence =
                    citer.value()->getNodesSequence(sequenceToken);

            if(!nodesSequence)
            {
                continue;
            }

            return citer.value().constCast<const SequenceFile>();
        }
    }

    qWarning() << "No sequence file has been found with the unique token: " << sequenceToken;
    return {};
}

QSharedPointer<const NodesSequence> ProjectManager::tryToGetLinkedNodesSequence(
        const QString &sequenceToken) const
{
    for(const QSharedPointer<ProjectFile> &projectFile : _loadedProjects)
    {
        const QMap<QString, QSharedPointer<SequenceFile>> &sequenceFile =
                projectFile->getSequenceFiles();

        QMap<QString, QSharedPointer<SequenceFile>>::const_iterator citer = sequenceFile.cbegin();

        for(; citer != sequenceFile.cend(); ++citer)
        {
            QSharedPointer<const NodesSequence> nodesSequence =
                    citer.value()->getNodesSequence(sequenceToken);

            if(!nodesSequence)
            {
                continue;
            }

            return nodesSequence;
        }
    }

    qWarning() << "No nodes sequence has been found with the unique token: " << sequenceToken;
    return {};
}

const QVector<QSharedPointer<Project>> &ProjectManager::accessRecentProjects()
{
    if(Q_LIKELY(_recentProjectLoaded))
    {
        return _recentProjects;
    }

    if(!_dbProjectAsker.getRecentProjects(_recentProjects))
    {
        qWarning("A problem occurred when tried to get the recent projects");
    }
    else
    {
        _recentProjectLoaded = true;
    }

    return _recentProjects;
}

bool ProjectManager::forgetProject(const QSharedPointer<Project> &project)
{
    RETURN_IF_FALSE(_dbProjectAsker.deleteProject(project));

    _recentProjects.removeOne(project);

    emit recentProjectListUpdated();
    return true;
}

ProjectFile *ProjectManager::loadProjectPriv(const QString &projectFilePath,
                                             FileLoadingErrorHandler *errorHandler)
{
    ProjectFile *projectFile = new ProjectFile(this);

    if(!projectFile->loadAndParseProjectFile(projectFilePath, errorHandler))
    {
        qWarning() << "A problem occurred when trying to load and parse the file: "
                   << projectFilePath;
        delete projectFile;
        return nullptr;
    }

    _loadedProjects.append(QSharedPointer<ProjectFile>(projectFile));

    return projectFile;
}

bool ProjectManager::registerProject(const QSharedPointer<Project> &project)
{
    RETURN_IF_FALSE(_dbProjectAsker.addProject(project));
    RETURN_IF_FALSE(_dbProjectAsker.getRecentProjects(_recentProjects));

    emit recentProjectListUpdated();
    return true;
}
