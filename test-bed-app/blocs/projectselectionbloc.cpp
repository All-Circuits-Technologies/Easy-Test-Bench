// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphaël Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "blocs/projectselectionbloc.hpp"

#include <QDir>
#include <QFileInfo>
#include <QQmlEngine>

#include "errors/fileloadingerrorhandler.hpp"
#include "fileutility/filehelper.hpp"
#include "models/filelistitemmodel.hpp"
#include "models/genericlistmodel.hpp"
#include "services/navigation/navigationmanager.hpp"
#include "test-bed-lib/models/project.hpp"
#include "test-bed-lib/project/projectmanager.hpp"


ProjectSelectionBloc::ProjectSelectionBloc(QObject *parent) :
    AFileSelectionBloc(new FileLoadingErrorHandler(), true, parent)
{
    connect(getPopUpModel(), &ScrollablePopUpModel::yesBtnClicked,
            this,            &ProjectSelectionBloc::removeFileFromRecentProjects);
    connect(getPopUpModel(), &ScrollablePopUpModel::noBtnClicked,
            this,            &ProjectSelectionBloc::keepMissingFileInRecentProjects);
    connect(&getProjectManager(), &ProjectManager::recentProjectListUpdated,
            this,                 &ProjectSelectionBloc::updateProjectList);
}

ProjectSelectionBloc::~ProjectSelectionBloc()
{
}

void ProjectSelectionBloc::initBloc()
{
    updateProjectList();

    AFileSelectionBloc::initBloc();
}

void ProjectSelectionBloc::updateProjectList()
{
    _recentProjects = getProjectManager().accessRecentProjects();

    QHash<QSharedPointer<Project>, QString> uniquePaths;
    getUniquePaths(uniquePaths);

    getFileListModel()->clearAll();

    for(auto citer = _recentProjects.cbegin(); citer != _recentProjects.cend(); ++citer)
    {
        FileListItemModel *projectListItemModel = new FileListItemModel(this);
        projectListItemModel->setName((*citer)->getName());
        projectListItemModel->setUniquePath(uniquePaths.value(*citer, ""));
        getFileListModel()->append(projectListItemModel);
    }
}

void ProjectSelectionBloc::getUniquePaths(QHash<QSharedPointer<Project>, QString> &uniquePaths)
{
    QVector<QSharedPointer<Project>> tmpRecentProjects(_recentProjects);

    while(!tmpRecentProjects.isEmpty())
    {
        const QSharedPointer<Project> &lastProject = tmpRecentProjects.takeLast();

        QString uniquePath;
        QSharedPointer<Project> projectWithLongest;
        QVector<QVector<QSharedPointer<Project>>::Iterator> sameNames;
        bool alwaysSameUniquePath = true;

        QVector<QSharedPointer<Project>>::Iterator iter = tmpRecentProjects.end();
        while(iter != tmpRecentProjects.begin())
        {
            --iter;

            if(lastProject->getName() != (*iter)->getName())
            {
                continue;
            }

            sameNames.append(iter);

            QString tmpUniquePath;
            FileHelper::getUniquePartOfAPath(lastProject->getPath(),
                                             (*iter)->getPath(),
                                             tmpUniquePath);

            if(uniquePath.isNull() || tmpUniquePath.length() > uniquePath.length())
            {
                if(!uniquePath.isNull() && alwaysSameUniquePath)
                {
                    alwaysSameUniquePath = false;
                }

                uniquePath = tmpUniquePath;
                projectWithLongest = (*iter);
            }
        }

        if(uniquePath.isNull())
        {
            // There is no other project with the same name, nothing to do
            continue;
        }

        uniquePaths.insert(lastProject, uniquePath);

        if(alwaysSameUniquePath)
        {
            // When the project has always the same unique path, it means that it's the one which
            // fires the longest unique path for all others projects
            projectWithLongest = lastProject;
        }

        for(QVector<QSharedPointer<Project>>::Iterator iterBis : sameNames)
        {
            QString refPath = projectWithLongest->getPath();
            if(projectWithLongest == (*iterBis))
            {
                // The project which fires the longest unique path is the current one, to generate
                // the unique path (which is always the same) we take the lastProject path
                refPath = lastProject->getPath();
            }

            FileHelper::getUniquePartOfAPath((*iterBis)->getPath(),
                                             refPath,
                                             uniquePath);

            uniquePaths.insert((*iterBis), uniquePath);
            tmpRecentProjects.erase(iterBis);
        }
    }
}

void ProjectSelectionBloc::onOpenButtonClicked()
{
    setFormButtonsEnabled(false);
    QSharedPointer<Project> project = _recentProjects.at(getCurrentlySelected());
    loadProject(project);
    setFormButtonsEnabled(true);
}

void ProjectSelectionBloc::removeFileFromRecentProjects()
{
    QSharedPointer<Project> project = _recentProjects.at(getCurrentlySelected());
    getProjectManager().forgetProject(project);
}

void ProjectSelectionBloc::keepMissingFileInRecentProjects()
{
    QObject *itemModel = getFileListModel()->at(getCurrentlySelected());
    itemModel->setProperty("missingFile", true);
}

bool ProjectSelectionBloc::loadProject(QSharedPointer<Project> &project)
{
    RETURN_IF_FALSE(AFileSelectionBloc::loadProject(project));

    getNavigationManager().goToSequenceSelectionPage();
    return true;
}

void ProjectSelectionBloc::registerQmlTypes()
{
    qmlRegisterType<ProjectSelectionBloc>("app.blocs", 1, 0, "ProjectSelectionBloc");
}
