// SPDX-FileCopyrightText: 2019 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "file/ajsonfile.hpp"

#include <QSharedPointer>
#include <QVector>
#include <QMap>

class DatabaseAuthInfo;
class FileLoadingErrorHandler;
class ProjectCache;
class SequenceFile;
class TopologyFile;


/** @brief Represents the project file */
class ProjectFile : public AJsonFile
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The parent class */
        explicit ProjectFile(QObject *parent = nullptr);

        /** @brief Get the project name */
        const QString &getName() const { return _name; }

        /** @brief Get the SequenceFile QHash */
        const QMap<QString, QSharedPointer<SequenceFile>> &getSequenceFiles() const
        { return _sequenceFiles; }

        /** @brief Set the project name
            @param name Name to be set */
        void setName(const QString &name) { _name = name; }

        /** @brief Add a sequence file
            @param fileName Name of the sequence
            @param sequence The sequence file to add */
        void addSequenceFile(const QString &fileName, const QSharedPointer<SequenceFile> &sequence);

        /** @brief Add a topology file
            @param topology The topology file to add */
        void addTopologyFile(const QSharedPointer<TopologyFile> &topology);

        /** @brief Load and parse the project file from its path
            @param filePath The path of the project file
            @param errorHandler An optional file loading error handler pointer
            @return True if no problem occurs */
        bool loadAndParseProjectFile(const QString &filePath,
                                     FileLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Set database authentication information
            @note If a dbAuthInfo already exist, delete the previous one
            @note Set the parent of the instance given to this object
            @param dbAuthInfo The database auth info*/
        void setDatabaseAuthInfo(DatabaseAuthInfo *dbAuthInfo);

        /** @brief Access the database authentication information */
        DatabaseAuthInfo *accessDatabaseAuthInfo() { return _dbAuthInfo; }

        /** @brief Access the cache linked to the project
            @note The cache lives with the project file */
        ProjectCache &accessCache() { return *_projectCache; }

        /** @brief Get the cache linked to the project
            @note The cache lives with the project file */
        const ProjectCache &getCache() const { return *_projectCache; }

    private:
        /** @brief The yaml suffix of the project file */
        static const constexpr char* YamlSuffix = "ytb";

        /** @brief The json suffix of the project file */
        static const constexpr char* JsonSuffix = "tb";

    private:
        QString _name{""};
        DatabaseAuthInfo *_dbAuthInfo{nullptr};
        ProjectCache *_projectCache{nullptr};

        // Use a QMap in order to keep the same order in the display of the sequences list
        QMap<QString, QSharedPointer<SequenceFile>> _sequenceFiles;
        QVector<QSharedPointer<TopologyFile>> _topologyFiles;
};
