// SPDX-FileCopyrightText: 2019 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QJsonArray>
#include <QSharedPointer>
#include <QVector>

#include "file/ajsonfile.hpp"
#include "sequencethread.hpp"
#include "testbedcore/testbedglobal.hpp"

class InclusionNodeKey;
class NodesSequence;
class QFile;
class FileLoadingErrorHandler;


/** @brief Represents a sequence file */
class TESTBEDLIB_DLLSPEC SequenceFile : public AJsonFile
{
    Q_OBJECT

    private:
        /** @brief States of the SequenceThread */
        enum SequenceThreadState
        {
            Initializing = 0,
            Running,
            Stopped
        };

    public:
        /** @brief Class constructor
            @param sequenceFile The file to get the sequence informations from
            @param parent The parent class */
        explicit SequenceFile(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~SequenceFile() override;

    public:
        /** @brief Init and set the sequence file
            @param sequenceFile the sequence file to read
            @param errorHandler The error handler to use and fill, if no null and an error occurred
            @return True if a problem occurred */
        bool initAndSetSequenceFile(QFile *sequenceFile,
                                    FileLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Get the number of SequenceThreads */
        int getSequenceThreadsNumber() const;

        /** @brief Access the sequence threads */
        QVector<SequenceThread *> accessSequenceThreads() const;

        /** @brief Find the first loaded NodesSequence in all the launched threads
            @return The first NodesSequence found and loaded, or a NULL SharedPointer, if none node
                    has been found */
        QSharedPointer<const NodesSequence> findFirstLoadedNodesSequence() const;

        /** @brief Get the nodes sequence linked to the sequence unique token given
            @param sequenceUniqueToken The token to get the nodes sequence
            @return The nodes sequence */
        QSharedPointer<const NodesSequence> getNodesSequence(
                                                        const QString &sequenceUniqueToken) const;

        /** @brief Test if a nodes sequence exist with the sequence unique token given
            @param sequenceUniqueToken The token to get the nodes sequence
            @return True if the nodes sequence is linked to the sequence file  */
        bool containsNodesSequence(const QString &sequenceUniqueToken) const;

        /** @brief Load the sequence file and get the QJsonArray from the file
            @param inclusionNodeKey The key only allowes authorized class instance to call this
                                    method
            @param errorHandler An optional sequence loading error handler pointer
            @return True if no problem occurs */
        bool loadSequenceData(const InclusionNodeKey &inclusionNodeKey,
                              SequenceLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Load the sequence file and launch the QThread
            @param nbrInstances Number of instances of the sequence to load
            @param errorHandler An optional sequence loading error handler pointer
            @return True if no problem occurs */
        bool loadSequenceDataAndLaunchThreads(int nbrInstances,
                                              SequenceLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Say if the sequence is loaded */
        bool isSequenceDataLoaded() const { return _sequenceDataLoaded; }

        /** @brief Test if all threads are running
            @note Returns False if the SequenceThreads list is empty
            @return True if all threads are running, else false */
        bool areAllThreadsRunning();

        /** @brief Test if all the NodesSequence linked to the threads have been loaded
            @note Returns False if the SequenceThreads list is empty
            @return True if all the NodesSequences are loaded */
        bool areAllNodesSequenceLoaded();

        /** @brief Return the QJsonArray */
        QJsonArray &accessJsonArray() { return _jsonArray; }

        /** @brief Stop the threads, when the threads is finished, its deleted */
        void stopAndClearThreads();

    private slots:
        /** @brief Called when a thread go on exec() */
        void onThreadReady();

    signals:
        /** @brief Emitted when all the sequence file threads are running */
        void allThreadsRunning();

    private:
        /** @brief Load the sequence file and get the QJsonArray from the file
            @param errorHandler An optional sequence loading error handler pointer
            @return True if no problem occurs */
        bool loadSequenceDataPriv(SequenceLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Remove data in the sequence file QJsonArray */
        void unloadData();

    private:
        /** @brief The YAML suffix of the sequence file */
        static const constexpr char* YamlSuffix = "ytbs";

        /** @brief The JSON suffix of the sequence file */
        static const constexpr char* JsonSuffix = "tbs";

    private:
        QVector<QSharedPointer<NodesSequence>> _nodesSequence;
        QHash<SequenceThread*, SequenceThreadState> _sequenceThreadList;
        bool _sequenceDataLoaded{false};
        QJsonArray _jsonArray;
};
