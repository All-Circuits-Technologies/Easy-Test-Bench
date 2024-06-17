// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "dbprojectasker.hpp"

#include "database/dbmanager.hpp"
#include "models/project.hpp"


DbProjectAsker::DbProjectAsker(DbManager &dbManager, QObject* parent):
    QObject(parent),
    _dbManager(dbManager)
{
}

bool DbProjectAsker::getRecentProjects(QVector<QSharedPointer<Project>> &projects) const
{
    QString query = QString(SelectRecentProjects).arg(RecentProjectsNumber);

    if(!_dbManager.select<Project>(query, projects))
    {
        qWarning() << "Can't get recent projects from the database";
        return false;
    }

    return true;
}

bool DbProjectAsker::updateProject(const QSharedPointer<Project> &project) const
{
    if(!_dbManager.update(project))
    {
        qWarning() << "Can't update the project in the database";
        return false;
    }

    return true;
}

bool DbProjectAsker::addProject(const QSharedPointer<Project> &project) const
{
    QSharedPointer<Project> projectInDb;
    RETURN_IF_FALSE(getProjectFromPath(project->getPath(), projectInDb));

    if(!projectInDb.isNull())
    {
        project->setId(projectInDb->getId());
        project->setLastModified(projectInDb->getLastModified());

        // The project already exist, we don't add it but update it
        return updateProject(project);
    }

    if(!_dbManager.insert(project))
    {
        qWarning() << "Can't add the project in the database";
        return false;
    }

    return true;
}

bool DbProjectAsker::deleteProject(const QSharedPointer<Project> &project) const
{
    if(!_dbManager.remove(project))
    {
        qWarning() << "Can't delete the project in the database";
        return false;
    }

    return true;
}

bool DbProjectAsker::getProjectFromPath(const QString &path, QSharedPointer<Project> &project) const
{
    QString wherePath = QString(FindByPath).arg(path);
    QString query = QString(DbManager::SelectQuery).arg(Project::tableName, wherePath);

    return _dbManager.selectOne(query, project);
}
