// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "nodessequencemanager.hpp"

#include "errors/helpers/fileloadingerrortypehelper.hpp"
#include "jsonparsers/sequencejsonarrayloader.hpp"
#include "nodessequence.hpp"


NodesSequenceManager::NodesSequenceManager(QObject *parent) :
    QObject(parent)
{
}

NodesSequenceManager::~NodesSequenceManager()
{
    cleanNodesSequence();
}

bool NodesSequenceManager::manageSequencePause(bool setToPause)
{
    if(!_nodesSequence)
    {
        qWarning() << "NodesSequence is not loaded";
        return false;
    }

    return _nodesSequence->managePauseAndResume(setToPause);
}

bool NodesSequenceManager::startSequence()
{
    if(!_nodesSequence)
    {
        qWarning() << "NodesSequence is not loaded";
        return false;
    }

    return _nodesSequence->startSequence();
}

bool NodesSequenceManager::stopSequence(StopAskedType::Enum stopAskedType)
{
    if(!_nodesSequence)
    {
        qWarning() << "NodesSequence is not loaded";
        return false;
    }

    return _nodesSequence->stopSequence(stopAskedType);
}

void NodesSequenceManager::cleanNodesSequence()
{
    if(_nodesSequence)
    {
        _nodesSequence->unloadAll();
        _nodesSequence.clear();
    }
}

bool NodesSequenceManager::cleanAndLoadNodesSequence(SequenceLoadingErrorHandler *errorHandler)
{
    cleanNodesSequence();

    if(!SequenceJsonArrayLoader::loadAndParseSequenceFile(*this,
                                                          _nodesSequenceInitializer,
                                                          errorHandler))
    {
        FileLoadingErrorTypeHelper::addUnreadableSequenceError(errorHandler);
        qWarning() << "NodesSequence is not loaded";
        emit nodesSequenceLoaded(false);
        return false;
    }

    emit nodesSequenceLoaded(true);
    return true;
}

bool NodesSequenceManager::setSequenceData(const QJsonArray &jsonArray,
                                           SequenceLoadingErrorHandler *errorHandler)
{
    if(jsonArray.isEmpty())
    {
        FileLoadingErrorTypeHelper::addUnreadableSequenceError(errorHandler);
        qWarning() << "The jsonArray is empty";
        return false;
    }

    _nodesSequenceInitializer = jsonArray;

    return true;
}
