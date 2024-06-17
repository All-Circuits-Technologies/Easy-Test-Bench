// SPDX-FileCopyrightText: 2019 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "sequencefile.hpp"

#include <QDebug>
#include <QFile>
#include <QFileInfo>

#include "collectionutility/qhashhelper.hpp"
#include "core/globalmanager.hpp"
#include "errors/helpers/fileloadingerrortypehelper.hpp"
#include "errors/sequenceloadingerrorhandler.hpp"
#include "jsonparsers/sequencefilejsonparser.hpp"
#include "sequence/nodessequence.hpp"
#include "testbedcore/testbedglobal.hpp"
#include "waitutility/waithelper.hpp"
#include "yamlutility/yamltojsonmanager.hpp"


SequenceFile::SequenceFile(QObject *parent) :
    AJsonFile(nullptr, parent)
{
}

SequenceFile::~SequenceFile()
{
    // Because we are deleting the file, we don't wait the threads to be finished (they will destroy
    // themselves)
    QHash<SequenceThread*, SequenceThreadState>::const_iterator citer =
            _sequenceThreadList.cbegin();
    for(; citer != _sequenceThreadList.cend(); ++citer)
    {
        citer.key()->prepareAndQuit();
    }
}

bool SequenceFile::initAndSetSequenceFile(QFile *sequenceFile,
                                          FileLoadingErrorHandler *errorHandler)
{
    QFile *tmpSeqFile = sequenceFile;
    QString filePath = tmpSeqFile->fileName();

    if(!tmpSeqFile->exists())
    {
        FileLoadingErrorTypeHelper::addMissingSequenceFileError(filePath, errorHandler);
        qWarning() << "The sequence file given doesn't exist: " << filePath;
        delete tmpSeqFile;
        return false;
    }

    const QFileInfo sequenceInfo(*tmpSeqFile);
    const QString suffix = sequenceInfo.suffix();

    if(suffix == SequenceFile::YamlSuffix)
    {
        QFile *jsonSequFile = new QFile(this);
        if(!GlobalManager::instance().accessYamlMngr().loadYamlAndConvertToJson(*tmpSeqFile,
                                                                                *jsonSequFile)) {
            FileLoadingErrorTypeHelper::addUnsupportedFileFormatError(suffix, errorHandler);
            delete jsonSequFile;
            delete tmpSeqFile;
            return false;
        }

        delete tmpSeqFile;
        setAliasPath(filePath);
        tmpSeqFile = jsonSequFile;
    }
    else if(suffix != SequenceFile::JsonSuffix)
    {
        qWarning() << "The file hasn't the expected format: " << filePath;
        FileLoadingErrorTypeHelper::addUnsupportedFileFormatError(suffix, errorHandler);
        delete tmpSeqFile;
        return false;
    }

    // Set device for reading and writting
    setFile(tmpSeqFile);
    return true;
}

int SequenceFile::getSequenceThreadsNumber() const
{
    return _sequenceThreadList.count();
}

QVector<SequenceThread*> SequenceFile::accessSequenceThreads() const
{
    return QHashHelper::getKeys(_sequenceThreadList);
}

QSharedPointer<const NodesSequence> SequenceFile::findFirstLoadedNodesSequence() const
{
    QHash<SequenceThread*, SequenceThreadState>::const_iterator citer =
            _sequenceThreadList.cbegin();

    for(; citer != _sequenceThreadList.cend(); ++citer)
    {
        QSharedPointer<const NodesSequence> nodesSequence = citer.key()->getLinkedNodesSequence();

        if(nodesSequence)
        {
            return nodesSequence;
        }
    }

    return {};
}

QSharedPointer<const NodesSequence> SequenceFile::getNodesSequence(
        const QString &sequenceUniqueToken) const
{
    QHash<SequenceThread*, SequenceThreadState>::const_iterator citer =
            _sequenceThreadList.cbegin();

    for(; citer != _sequenceThreadList.cend(); ++citer)
    {
        QSharedPointer<const NodesSequence> nodesSequence = citer.key()->getLinkedNodesSequence();

        if(!nodesSequence)
        {
            // No nodes sequence linked
            continue;
        }

        if(nodesSequence->getUniqueToken() == sequenceUniqueToken)
        {
            return nodesSequence;
        }

        nodesSequence = nodesSequence->findAChildNodesSequence(sequenceUniqueToken);

        if(nodesSequence)
        {
            // if it's not null, it means that we have found the right sequence
            return nodesSequence;
        }
    }

    return {};
}

bool SequenceFile::containsNodesSequence(const QString &sequenceUniqueToken) const
{
    QHash<SequenceThread*, SequenceThreadState>::const_iterator citer =
            _sequenceThreadList.cbegin();

    for(; citer != _sequenceThreadList.cend(); ++citer)
    {
        QSharedPointer<const NodesSequence> nodesSequence = citer.key()->getLinkedNodesSequence();

        if(!nodesSequence)
        {
            // No nodes sequence linked
            continue;
        }

        if(nodesSequence->getUniqueToken() == sequenceUniqueToken)
        {
            return true;
        }

        nodesSequence = nodesSequence->findAChildNodesSequence(sequenceUniqueToken);

        if(nodesSequence)
        {
            // if it's not null, it means that we have found the right sequence
            return true;
        }
    }

    return false;
}

bool SequenceFile::loadSequenceData(const InclusionNodeKey &inclusionNodeKey,
                                    SequenceLoadingErrorHandler *errorHandler)
{
    Q_UNUSED(inclusionNodeKey)

    return loadSequenceDataPriv(errorHandler);
}

bool SequenceFile::loadSequenceDataPriv(SequenceLoadingErrorHandler *errorHandler)
{
    unloadData();

    TEST_AND_RETURN_IF_FALSE(SequenceFileJsonParser::loadSequenceFile(accessFile(), _jsonArray),
                             FileLoadingErrorTypeHelper::addInaccessibleFileError(errorHandler););

    _sequenceDataLoaded = true;

    return true;
}

bool SequenceFile::loadSequenceDataAndLaunchThreads(int nbrInstances,
                                                    SequenceLoadingErrorHandler *errorHandler)
{
    stopAndClearThreads();

    RETURN_IF_FALSE(loadSequenceDataPriv(errorHandler));

    for(int idx = 0; idx < nbrInstances; idx++)
    {
        SequenceThread * sequenceThread = new SequenceThread();

        connect(sequenceThread, &SequenceThread::ready,
                this,           &SequenceFile::onThreadReady);

        _sequenceThreadList.insert(sequenceThread, Initializing);

        sequenceThread->start();
    }

    return true;
}

bool SequenceFile::areAllThreadsRunning()
{
    if(_sequenceThreadList.isEmpty())
    {
        return false;
    }

    QHash<SequenceThread*, SequenceThreadState>::const_iterator citer =
            _sequenceThreadList.cbegin();

    for(; citer != _sequenceThreadList.cend(); ++citer)
    {
        if(citer.value() != Running)
        {
            return false;
        }
    }

    return true;
}

bool SequenceFile::areAllNodesSequenceLoaded()
{
    if(_sequenceThreadList.isEmpty())
    {
        return false;
    }

    QHash<SequenceThread*, SequenceThreadState>::const_iterator citer =
            _sequenceThreadList.cbegin();

    for(; citer != _sequenceThreadList.cend(); ++citer)
    {
        if(!citer.key()->getLinkedNodesSequence())
        {
            return false;
        }
    }

    return true;
}

void SequenceFile::onThreadReady()
{
    SequenceThread *thread = qobject_cast<SequenceThread*>(sender());

    if(!thread)
    {
        qWarning() << "The sender is not a SequenceThread";
        return;
    }

    if(!_sequenceThreadList.contains(thread))
    {
        qWarning() << "The sequence thread is not known, can't proceed further and give the data to"
                   << " parse";
        return;
    }

    _sequenceThreadList[thread] = Running;

    if(areAllThreadsRunning())
    {
        emit allThreadsRunning();
    }
}

void SequenceFile::stopAndClearThreads()
{
    // If there is no sequence thread to stop, all is already finished
    bool finished = _sequenceThreadList.isEmpty();

    auto testIfAllFinished = [&finished, this] ()
    {
        if(finished)
        {
            // Nothing to test
            return;
        }

        bool tmpFinished = true;
        QHash<SequenceThread*, SequenceThreadState>::const_iterator citer =
                                                        _sequenceThreadList.cbegin();
        for(; citer != _sequenceThreadList.cend(); ++citer)
        {
            if(!citer.key()->isFinished())
            {
                tmpFinished = false;
                break;
            }
        }

        if(tmpFinished)
        {
            finished = true;
        }
    };

    QHash<SequenceThread*, SequenceThreadState>::const_iterator citer =
            _sequenceThreadList.cbegin();
    for(; citer != _sequenceThreadList.cend(); ++citer)
    {
        connect(citer.key(), &SequenceThread::finished, this, testIfAllFinished);
        citer.key()->prepareAndQuit();
        if(citer.key()->isFinished())
        {
            testIfAllFinished();
        }
    }

    WaitHelper::pseudoWait(finished);

    unloadData();

    // The sequence thread keys aren't removed when the thread is finished
    qDeleteAll(_sequenceThreadList.keyBegin(), _sequenceThreadList.keyEnd());
    _sequenceThreadList.clear();
}

void SequenceFile::unloadData()
{
    while(_jsonArray.count())
    {
        _jsonArray.pop_back();
    }
    _sequenceDataLoaded = false;
}
