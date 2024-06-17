// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "jsonparsers/ajsonparser.hpp"

class ProjectFile;


/** @brief The class helps to manage th database part in the project file */
class DatabaseInfosJsonParser : public AJsonParser
{
    Q_OBJECT

    private:
        /** @brief Class constructor
            @param dbObject The object which contains the database informations
            @param projectFile The project file to fill with the database informations
            @param parent The class parent */
        explicit DatabaseInfosJsonParser(const QJsonObject &dbObject,
                                         ProjectFile &projectFile,
                                         QObject *parent = nullptr);

    public:
        /** @brief Load and parse the database information got from project file
            @param databaseInfoObject The JSON object which contains the database informations
            @param projectFile The project file to fill with the database informations
            @return True if no problem occurs */
        static bool loadAndParseDatabaseInfo(const QJsonObject &databaseInfoObject,
                                             ProjectFile &projectFile);

    private:
        /** @brief Load the database information from the dbObject and fill the project file
            @return True if no problem occurs */
        bool loadDatabaseInfo();

    private:
        const QJsonObject &_dbObject;
        ProjectFile &_projectFile;
};
