// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "testbedcore/testbedglobal.hpp"

class DbManager;
class Project;


/** @brief This asker allows to formulate queries related to the Project class in the database */
class TESTBEDLIB_DLLSPEC DbProjectAsker : QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param dbmanager A DbManager reference
            @param parent Optional Qt parentship */
        explicit DbProjectAsker(DbManager &dbManager, QObject* parent = nullptr);

    public:
        /** @brief Retrieve recently onpened projects from the database
            @param projects Vector which contains projects found
            @return True if success, False if failure */
        bool getRecentProjects(QVector<QSharedPointer<Project>> &projects) const;

        /** @brief Update a project in the database
            @param project Project to update
            @return True if success, False if failure */
        bool updateProject(const QSharedPointer<Project> &project) const;

        /** @brief Add a project in the database, update it if it is already in
            @param project Project to add
            @return True if success, False if failure */
        bool addProject(const QSharedPointer<Project> &project) const;

        /** @brief Delete a project in the database
            @param project Project to delete
            @return True if success, False if failure */
        bool deleteProject(const QSharedPointer<Project> &project) const;

    private:
        /** @brief Get a project from path
            @param path The project to get from its path
            @param project The project got from database
            @return True if success, False if failure */
         bool getProjectFromPath(const QString &path, QSharedPointer<Project> &project) const;

    private:
        static const constexpr int RecentProjectsNumber = 10;
        static const constexpr char* SelectRecentProjects = "SELECT * FROM projects "
                                                            "ORDER BY lastOpened DESC LIMIT %1";
        static const constexpr char* FindByPath = "path = '%1'";

    private:
        DbManager &_dbManager;
};
