// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "inclusionnode.hpp"

#include <QDebug>
#include <QFile>

#include "errors/helpers/fileloadingerrortypehelper.hpp"
#include "errors/helpers/fileloadingerrortypehelper.hpp"
#include "errors/helpers/sequenceloadingerrorhelper.hpp"
#include "testbedcore/testbedglobal.hpp"
#include "sequence/accesskeys/inclusionnodekey.hpp"
#include "sequence/nodessequence.hpp"
#include "sequence/nodessequencemanager.hpp"
#include "sequence/sequencefile.hpp"


InclusionNode::InclusionNode(const QSharedPointer<NodesSequence> &sequenceParent, QObject *parent) :
    ASequenceContainerNode(NodeType::InclusionNode, sequenceParent, parent),
    _nodesSequenceManager(new NodesSequenceManager(this))
{
}

bool InclusionNode::setAndLoadSequenceFile(const QString &filePath,
                                           SequenceLoadingErrorHandler *errorHandler)
{
    delete _sequenceFile;
    _sequenceFile = nullptr;
    _sequenceFileName.clear();

    QFile *sequenceFileDevice = new QFile(filePath);

    if(!sequenceFileDevice->exists())
    {
        FileLoadingErrorTypeHelper::addMissingSequenceFileError(filePath, errorHandler);
        qWarning() << "The sequence file doesn't exist: " << filePath;
        delete sequenceFileDevice;
        return false;
    }

    _sequenceFile = new SequenceFile(this);

    if(!_sequenceFile->initAndSetSequenceFile(sequenceFileDevice, errorHandler))
    {
        delete _sequenceFile;
        _sequenceFile = nullptr;
        return false;
    }

    _sequenceFileName = filePath;

    return loadSequenceFile(errorHandler);
}

QSharedPointer<NodesSequence> InclusionNode::getChildNodesSequence() const
{
    if(_sequenceFile == nullptr)
    {
        qWarning() << "Can't get the nodesSequence of a null sequence file, in the inclusion node: "
                   << getInstanceName();
        return  {};
    }

    return _nodesSequenceManager->getNodesSequence();
}

bool InclusionNode::preparingSequenceBeforeStart(QString &error)
{
    if(!_sequenceFile)
    {
        qWarning() << "No proper sequence file has been given";
        error = "No sequence file path has been given or a problem occurred when it has been loaded";
        return false;
    }

    const QSharedPointer<NodesSequence> &nodesSequence = _nodesSequenceManager->getNodesSequence();

    if(_sequenceFile->isSequenceDataLoaded() && nodesSequence)
    {
        // Sequence is already loaded, no need to do it again
    }
    else if(!loadSequenceFile())
    {
        qWarning() << "An error occurred and the sequence file linked to the inclusion node: "
                   << getInstanceName() << ", can't be loaded";
        error = "The sequence file linked to this node can't be loaded";
        return false;
    }

    return true;
}

void InclusionNode::forgetAllAboutMe()
{
    _nodesSequenceManager->cleanNodesSequence();
}

bool InclusionNode::loadSequenceFile(SequenceLoadingErrorHandler *errorHandler)
{
    if(_sequenceFile == nullptr)
    {
        SequenceLoadingErrorHelper::addFailedNodesSequenceError(errorHandler, getInstanceName());
        qWarning() << "Can't load a null sequence file, from the inclusion node: "
                   << getInstanceName();
        return false;
    }

    RETURN_IF_FALSE(_sequenceFile->loadSequenceData({},errorHandler));

    RETURN_IF_FALSE(_nodesSequenceManager->setSequenceData(_sequenceFile->accessJsonArray(),
                                                           errorHandler));

    RETURN_IF_FALSE(_nodesSequenceManager->cleanAndLoadNodesSequence(errorHandler));

    return true;
}
