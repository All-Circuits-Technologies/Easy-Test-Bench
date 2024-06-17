// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "blocs/afileselectionbloc.hpp"

#include <QModelIndex>

class DbProjectAsker;
class Project;


/** @brief Backend class linked with project selection page */
class ProjectSelectionBloc : public AFileSelectionBloc
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The optional parent QObject */
        explicit ProjectSelectionBloc(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~ProjectSelectionBloc() override;

    public slots:
        /** @brief Called when open button is clicked
            @param index The index of the project selected */
        virtual void onOpenButtonClicked() override;

    public:
        /** @brief Register QML type linked to this bloc */
        static void registerQmlTypes();

    protected slots:
        /** @see AbstractBloc::initBloc */
        virtual void initBloc() override;

    protected:
        /** @brief Load a project
            @param project The project loaded */
        virtual bool loadProject(QSharedPointer<Project> &project) override;

    private slots:
        /** @brief Called user decided to remove a missing file from recent projects
            @param index The index of the project selected */
        void removeFileFromRecentProjects();

        /** @brief Called user decided to keep a missing file from recent projects
            @param index Index of the project item selected */
        void keepMissingFileInRecentProjects();

        /** @brief Update the content of the project list */
        void updateProjectList();

    private:
        /** @brief Get the unique path of each recent projects
            @param uniquePaths The unique path of the recent projects */
        void getUniquePaths(QHash<QSharedPointer<Project>, QString> &uniquePaths);

    private:
        QVector<QSharedPointer<Project>> _recentProjects;
};
