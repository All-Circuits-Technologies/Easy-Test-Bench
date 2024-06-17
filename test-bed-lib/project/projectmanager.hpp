// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QSharedPointer>
#include <QVector>

#include "testbedcore/testbedglobal.hpp"

class DbManager;
class DbProjectAsker;
class FileLoadingErrorHandler;
class ISequenceLogManager;
class NodesSequence;
class ProjectFile;
class Project;
class SequenceFile;


/** @brief Manager to help the loading and unloading of projects */
class TESTBEDLIB_DLLSPEC ProjectManager : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param dbManager The database manager
            @param parent The parent class */
        explicit ProjectManager(DbManager &dbManager, QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~ProjectManager() override;

    public:
        /** @brief Load a project from path
            @param projectFilePath The project file path
            @param errorHandler An optional file loading error handler pointer
            @return True if no problem occurs */
        bool loadProject(const QString &projectFilePath,
                         FileLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Load a project file from the project given
            @param project The database representation of the project
            @param errorHandler An optional file loading error handler pointer
            @return True if no problem occurs */
        bool loadProject(const QSharedPointer<Project> &project,
                         FileLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Unload a project
            @param project The project to unload and forget
            @return True if no problem occurs */
        bool unloadProject(const QSharedPointer<ProjectFile> &project);

        /** @brief Access the projects loaded */
        const QVector<QSharedPointer<ProjectFile>> &accessProjects() { return _loadedProjects; }

        /** @brief Get the projects loaded */
        QVector<QSharedPointer<const ProjectFile>> getProjects() const;

        /** @brief Try to get the project file linked to the nodes sequence attached to the token
                   given
            @param sequenceToken The token of the sequence to find in projects
            @return The project file which have the nodes sequence linked to the token given, or
                    nullptr if not found */
        QSharedPointer<const ProjectFile> tryToGetLinkedProjectFile(
                                                                const QString &sequenceToken) const;

        /** @brief Try to get the access file linked to the nodes sequence attached to the token
                   given
            @param sequenceToken The token of the sequence to find in projects
            @return The project file which have the nodes sequence linked to the token given, or
                    nullptr if not found */
        QSharedPointer<ProjectFile> tryToAccessLinkedProjectFile(
                                                                const QString &sequenceToken) const;

        /** @brief Try to get the sequence file linked to the nodes sequence attached to the token
                   given
            @param sequenceToken The token of the sequence to find in projects
            @return The sequence file which have the nodes sequence linked to the token given, or
                    nullptr if not found */
        QSharedPointer<const SequenceFile> tryToGetLinkedSequenceFile(
                                                                const QString &sequenceToken) const;

        /** @brief Try to get the nodes sequence linked to the token given
            @param sequenceToken The token of the sequence to find
            @return The nodes sequence linked to the token, or nullptr if no sequence has been
                    found */
        QSharedPointer<const NodesSequence> tryToGetLinkedNodesSequence(
                                                                const QString &sequenceToken) const;

        /** @brief Access recent projects */
        const QVector<QSharedPointer<Project>> &accessRecentProjects();

        /** @brief Ask to forget the given project
            @note The project is forgotten in the database */
        bool forgetProject(const QSharedPointer<Project> &project);

    public slots:
        /** @brief Unload the first project loaded
            @return True if no problem occurs */
        bool unloadFirstProject();

        /** @brief Unload all the projects loaded
            @return True if no problem occurs */
        bool unloadAllProjects();

    private:
        /** @brief Manage the loading of project
            @param projectFilePath The path of the project file
            @param errorHandler An optional file loading error handler pointer
            @return The project file loaded or nullptr if a problem occurred */
        ProjectFile *loadProjectPriv(const QString &projectFilePath,
                                     FileLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Manage the registering in database of the project
            @param project The project to register
            @return True if success */
        bool registerProject(const QSharedPointer<Project> &project);

    signals:
        /** @brief Emitted when the list of recent projects is updated */
        void recentProjectListUpdated();

    private:
        DbProjectAsker &_dbProjectAsker;

        QVector<QSharedPointer<ProjectFile>> _loadedProjects;
        QVector<QSharedPointer<Project>> _recentProjects;
        bool _recentProjectLoaded{false};
};
