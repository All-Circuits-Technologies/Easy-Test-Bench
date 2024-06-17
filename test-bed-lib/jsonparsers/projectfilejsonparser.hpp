// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "jsonparsers/ajsonparser.hpp"

class FileLoadingErrorHandler;
class ProjectFile;
class QIODevice;


/** @brief Useful class to parse the project file */
class ProjectFileJsonParser : public AJsonParser
{
    Q_OBJECT

    private:
        /** @brief Class constructor
            @param rootObject The project file JSON object to parse
            @param projectFile The projectFile class instance to fill with the informations parsed
            @param parent The class parent */
        explicit ProjectFileJsonParser(const QJsonObject &rootObject,
                                       ProjectFile &projectFile,
                                       QObject *parent = nullptr);

    public:
        /** @brief Load and parse the project file
            @param projectDevice The project device to load and parse
            @param projectFile The projectFile class instance to fill with the informations parsed
            @param errorHandler An optional file loading error handler pointer
            @return True if no problem occurs */
        static bool loadAndParseProjectFile(QIODevice &projectDevice,
                                            ProjectFile &projectFile,
                                            FileLoadingErrorHandler *errorHandler = nullptr);

    private:
        /** @brief Load the project name from root object
            @param errorHandler An optional file loading error handler pointer
            @return True if no problem occurs */
        bool loadProjectName(FileLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Create the topologyFile class instances from root object from the project file
                   informations
            @note The topology file aren't automatically loaded
            @param errorHandler An optional file loading error handler pointer
            @return True if no problem occurs */
        bool loadTopologyFiles(FileLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Create the sequenceFile class instances from root object from the project file
                   informations
            @note The sequence file nodes aren't automatically loaded
            @param errorHandler An optional file loading error handler pointer
            @return True if no problem occurs */
        bool loadSequenceFilesHeaders(FileLoadingErrorHandler *errorHandler = nullptr);

    private:
        const QJsonObject &_rootObject;
        ProjectFile &_projectFile;
};
