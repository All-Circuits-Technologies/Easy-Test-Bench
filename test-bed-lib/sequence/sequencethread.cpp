// SPDX-FileCopyrightText: 2019 - 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "sequencethread.hpp"

#include <QDebug>
#include <QJsonArray>
#include <QTimer>

#include "errors/sequenceloadingerrorhandler.hpp"
#include "nodessequence.hpp"
#include "nodessequencemanager.hpp"
#include "threadutility/concurrent/threadconcurrentrun.hpp"


SequenceThread::SequenceThread(QObject *parent) :
    BaseThread(parent)
{
}

SequenceThread::~SequenceThread()
{
}

QSharedPointer<const NodesSequence> SequenceThread::getLinkedNodesSequence() const
{
    if(waitForThread() && (_nodesSeqManager != nullptr))
    {
        return _nodesSeqManager->getNodesSequence().objectCast<const NodesSequence>();
    }

    return {};
}

void SequenceThread::cleanNodesSequence()
{
    if(waitForThread())
    {
        ThreadConcurrentRun::run(*_nodesSeqManager, &NodesSequenceManager::cleanNodesSequence);
    }
}

bool SequenceThread::cleanAndLoadNodesSequence(SequenceLoadingErrorHandler *errorHandler)
{
    if(waitForThread())
    {
        return ThreadConcurrentRun::run(*_nodesSeqManager,
                                        &NodesSequenceManager::cleanAndLoadNodesSequence,
                                        errorHandler);
    }

    return false;
}

void SequenceThread::prepareAndQuit()
{
    bool quicklyQuit = true;

    // Wait the nodes sequence destroying to quit the thread (to prevent race problems)
    if(_nodesSeqManager)
    {
        QSharedPointer<NodesSequence> nodesSequence = _nodesSeqManager->getNodesSequence();
        if(nodesSequence)
        {
            quicklyQuit = false;
            connect(nodesSequence.data(), &NodesSequence::destroyed,
                    this,                 &SequenceThread::quit);
        }
    }

    QTimer::singleShot(0, _nodesSeqManager, &NodesSequenceManager::cleanNodesSequence);

    if(quicklyQuit)
    {
        QTimer::singleShot(0, this, &SequenceThread::quit);
    }
}

bool SequenceThread::startSequence()
{
    if(waitForThread())
    {
        return ThreadConcurrentRun::run(*_nodesSeqManager, &NodesSequenceManager::startSequence);
    }

    return false;
}

bool SequenceThread::manageSequencePause(bool setToPause)
{
    if(waitForThread())
    {
        return ThreadConcurrentRun::run(*_nodesSeqManager,
                                        &NodesSequenceManager::manageSequencePause,
                                        setToPause);
    }

    return false;
}

bool SequenceThread::stopSequence(StopAskedType::Enum stopAskedType)
{
    if(waitForThread())
    {
        return ThreadConcurrentRun::run(*_nodesSeqManager,
                                        &NodesSequenceManager::stopSequence,
                                        stopAskedType);
    }

    return false;
}

void SequenceThread::onNodesSeqFinished(bool success)
{
    if(success)
    {
        emit newLog({ LogMsgType::Info,
                      SequenceLogCategory::ApplicationScope,
                      tr("(TR) Sequence finished") });
    }
    else
    {
        emit newLog({ LogMsgType::Warning,
                      SequenceLogCategory::ApplicationScope,
                      tr("(TR) The sequence finished wrongly") });
    }

    emit nodeSeqFinished(success);
}

bool SequenceThread::setSequenceData(const QJsonArray &jsonArray,
                                     SequenceLoadingErrorHandler *errorHandler)
{
    if(waitForThread())
    {
        return ThreadConcurrentRun::run(*_nodesSeqManager,
                                        &NodesSequenceManager::setSequenceData,
                                        jsonArray,
                                        errorHandler);
    }

    return false;
}

void SequenceThread::run()
{
    _nodesSeqManager = new NodesSequenceManager();

    // On thread delete, delete the NodesSequenceManager
    connect(this,            &SequenceThread::finished,
            _nodesSeqManager,&NodesSequenceManager::deleteLater);

    // Relay NodesSequence loading confirmation
    connect(_nodesSeqManager,&NodesSequenceManager::nodesSequenceLoaded,
            this,            &SequenceThread::onNodesSequenceLoaded);

    BaseThread::run();
}

void SequenceThread::onNodesSequenceLoaded(bool loadSuccess)
{
    if(!loadSuccess || !_nodesSeqManager->getNodesSequence())
    {
        emit newLog({ LogMsgType::Warning,
                      SequenceLogCategory::ApplicationScope,
                      tr("(TR) The loading of the sequence failed.") });
        emit nodesSequenceLoaded(false);
        return;
    }

    connect(_nodesSeqManager->getNodesSequence().data(), &NodesSequence::finished,
            this,                                        &SequenceThread::onNodesSeqFinished);

    connect(getSeqLogManager(), &SequenceLogManager::newLogAdded, this, &SequenceThread::newLog);

    emit newLog({ LogMsgType::Info,
                  SequenceLogCategory::ApplicationScope,
                  tr("(TR) Sequence loaded") });
    emit nodesSequenceLoaded(true);
}

const SequenceLogManager *SequenceThread::getSeqLogManager() const
{
    if(!_nodesSeqManager || !_nodesSeqManager->getNodesSequence())
    {
        return nullptr;
    }

    return &_nodesSeqManager->getNodesSequence()->getSequenceLogManager();
}
