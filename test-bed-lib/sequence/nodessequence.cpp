// SPDX-FileCopyrightText: 2019 - 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "nodessequence.hpp"

#include <QStateMachine>
#include <QUuid>

#include "collectionutility/qhashhelper.hpp"
#include "sequence/accesskeys/nodessequencekey.hpp"
#include "sequence/checksequencevalidity.hpp"
#include "sequence/nodes/asequencenode.hpp"
#include "sequence/nodes/groupnode.hpp"
#include "sequence/nodes/inclusionnode.hpp"
#include "sequence/referencenodes/areferencenode.hpp"
#include "sequence/referencenodes/constantnode.hpp"
#include "sequence/referencenodes/variablenode.hpp"
#include "testbedcore/logs/sequencelogformatter.hpp"
#include "testbedcore/testbedglobal.hpp"


NodesSequence::NodesSequence(QObject *parent) :
    ANode(NodeType::NodesSequence, QSharedPointer<NodesSequence>(), parent),
    _uniqueToken(QUuid::createUuid().toString())
{
    _stateMachine = QSharedPointer<QStateMachine>(new QStateMachine());
    connect(_stateMachine.data(), &QStateMachine::finished,
            this,                 &NodesSequence::onStateMachineFinished);

    connect(this,                   &NodesSequence::newLog,
            &_sequenceLogManager,   &SequenceLogManager::storeLog);
}

NodesSequence::~NodesSequence()
{
    unloadAll();
}

QSharedPointer<NodesSequence> NodesSequence::createNodesSequence()
{
    QSharedPointer<NodesSequence> nodesSequence =
                                                QSharedPointer<NodesSequence>(new NodesSequence());
    nodesSequence->_refToThisObject = nodesSequence.toWeakRef();

    return nodesSequence;
}

bool NodesSequence::startSequence()
{
    if(_sequenceState.isRunning())
    {
        qWarning() << "Can't start the sequence, it's already running";
        return false;
    }

    QString error;
    QSharedPointer<ANode> nodeWithError;
    if(!CheckSequenceValidity::isValid(_refToThisObject.toStrongRef(), error, nodeWithError))
    {
        _errorString = error;
        _guiltyNode = nodeWithError;
        return false;
    }

    RETURN_IF_FALSE(applyExternalRefValuesToInternal());

    qInfo() << "Start sequence, linked to the node: " << getInstanceName();

    _sequenceState.onRunning();

    _guiltyNode.clear();
    _errorString.clear();

    _stateMachine->start();

    return (_stateMachine->error() == QStateMachine::NoError);
}

bool NodesSequence::stopSequence(StopAskedType::Enum stopAskedType)
{
    qInfo() << "Stop sequence, linked to the node: " << getInstanceName();

    if(stopAskedType == StopAskedType::AskedByUser)
    {
        _sequenceLogManager.storeLog(SequenceLogData(LogMsgType::Critical,
                                                     SequenceLogCategory::ApplicationScope,
                                                     "The sequence has been stopped by user"));
    }

    _sequenceState.onStopAsked(stopAskedType);
    _stateMachine->stop();
    return true;
}

bool NodesSequence::managePauseAndResume(bool setToPause)
{
    _sequenceState.onPauseOrResume(setToPause);
    return true;
}

void NodesSequence::addSequenceNode(const QSharedPointer<ASequenceNode> &node)
{
    if(_sequenceNodes.contains(node->getInstanceName()))
    {
        removeSequenceNode(node);
    }

    _sequenceNodes.insert(node->getInstanceName(), node);

    connect(node.data(),   &ASequenceNode::error,
            this,          &NodesSequence::onSequenceError);
    connect(node.data(),            &NodesSequence::newLog,
            &_sequenceLogManager,   &SequenceLogManager::storeLog);
}

void NodesSequence::removeSequenceNode(const QSharedPointer<ASequenceNode> &node)
{
    QHash<QString, QSharedPointer<ASequenceNode>>::const_iterator iter =
                std::find_if(_sequenceNodes.cbegin(),
                             _sequenceNodes.cend(),
                             [&](const QSharedPointer<ASequenceNode> &elem)
                             {
                                 return elem->getInstanceName() == node->getInstanceName();
                             });

    if(iter != _sequenceNodes.cend())
    {
        emit hasToDie(node);
        _sequenceNodes.erase(iter);
    }
}
void NodesSequence::addReferenceNode(const QSharedPointer<AReferenceNode> &referenceNode)
{
    _referenceNodes.insert(referenceNode->getInstanceName(), referenceNode);

    connect(referenceNode.data(),   &NodesSequence::newLog,
            &_sequenceLogManager,   &SequenceLogManager::storeLog);
}

void NodesSequence::removeReferenceNode(const QSharedPointer<AReferenceNode> &referenceNode)
{
    QHash<QString, QSharedPointer<AReferenceNode>>::const_iterator iter =
                std::find_if(_referenceNodes.cbegin(),
                             _referenceNodes.cend(),
                             [&](const QSharedPointer<AReferenceNode> &elem)
                             {
                                 return elem->getInstanceName() == referenceNode->getInstanceName();
                             });


    if(iter != _referenceNodes.cend())
    {
        emit hasToDie(referenceNode);
        _referenceNodes.erase(iter);
    }
}

QSharedPointer<ASequenceNode> NodesSequence::getSequenceNode(const QString &uniqueName) const
{
    return _sequenceNodes.value(uniqueName);
}

QVector<QString> NodesSequence::getSequenceNodesName() const
{
    return QHashHelper::getKeys(_sequenceNodes);
}

QSharedPointer<AReferenceNode> NodesSequence::getReferenceNode(const QString &uniqueName) const
{
    return _referenceNodes.value(uniqueName);
}

QSharedPointer<VariableNode> NodesSequence::getVariableNode(const QString &uniqueName) const
{
    const QSharedPointer<AReferenceNode> &refNode = _referenceNodes.value(uniqueName);

    if(refNode.isNull() || refNode->getNodeType() != NodeType::VariableNode)
    {
        return {};
    }

    return refNode.objectCast<VariableNode>();
}

void NodesSequence::unloadAll()
{
    qDebug() << "Unload all";
    QHash<QString, QSharedPointer<ASequenceNode>>::iterator iter = _sequenceNodes.begin();

    while(iter != _sequenceNodes.end())
    {
        qDebug() << "Has to die: " << iter.value()->getInstanceName();
        emit hasToDie(iter.value());
        iter = _sequenceNodes.erase(iter);
    }

    QHash<QString, QSharedPointer<AReferenceNode>>::iterator iterRef = _referenceNodes.begin();

    while(iterRef != _referenceNodes.end())
    {
        emit hasToDie(iterRef.value());
        iterRef = _referenceNodes.erase(iterRef);
    }

    _guiltyNode.clear();
}

const QSharedPointer<QStateMachine> &NodesSequence::accessStateMachine(
                                                                const ASequenceNodeKey &seqNodeKey)
{
    Q_UNUSED(seqNodeKey)

    return _stateMachine;
}

QSharedPointer<const QStateMachine> NodesSequence::getStateMachine() const
{
    return _stateMachine.constCast<const QStateMachine>();
}

QVector<QSharedPointer<ASequenceNode> > NodesSequence::getSequenceNodes() const
{
    return QHashHelper::getValues(_sequenceNodes);
}

QVector<QSharedPointer<AReferenceNode> > NodesSequence::getReferenceNodes() const
{
    return QHashHelper::getValues(_referenceNodes);
}

QSharedPointer<NodesSequence> NodesSequence::findAChildNodesSequence(
                                                                const QString &uniqueToken) const
{
    if(uniqueToken == _uniqueToken)
    {
        return _refToThisObject.toStrongRef();
    }

    QHash<QString, QSharedPointer<ASequenceNode>>::const_iterator citer = _sequenceNodes.cbegin();

    for(; citer != _sequenceNodes.cend(); ++citer)
    {
        QSharedPointer<NodesSequence> nodesSeq;

        if(citer.value()->getNodeType() == NodeType::GroupNode)
        {
            QSharedPointer<GroupNode> groupNode = citer.value().objectCast<GroupNode>();

            if(!groupNode)
            {
                continue;
            }

            nodesSeq = groupNode->getChildNodesSequence();
        }
        else if(citer.value()->getNodeType() == NodeType::InclusionNode)
        {
            QSharedPointer<InclusionNode> inclusionNode = citer.value().objectCast<InclusionNode>();

            if(!inclusionNode)
            {
                continue;
            }

            nodesSeq = inclusionNode->getChildNodesSequence();
        }

        if(!nodesSeq)
        {
            // The linked node sequence doesn't exist
            continue;
        }

        if(nodesSeq->getUniqueToken() == uniqueToken)
        {
            return nodesSeq;
        }

        nodesSeq = nodesSeq->findAChildNodesSequence(uniqueToken);

        if(nodesSeq)
        {
            // If it's not null it means that the nodes sequence has been found in a child sequence
            // of the child sequence itself
            return nodesSeq;
        }
    }

    return {};
}

void NodesSequence::onStateMachineFinished()
{
    if(_sequenceState.isStopped())
    {
        // No need to go after
        return;
    }

    if(_stateMachine->error() != QStateMachine::NoError)
    {
        onSequenceError(*this, _stateMachine->errorString());
    }
    else if(_stateMachine->isRunning())
    {
        qWarning() << "The state machine has been said as finished but it's still running";

        onSequenceError(*this, "The sequence is finished but the QStateMachine is still running");
    }
    else if(!applyInternalVarValuesToExternal())
    {
        onSequenceError(*this, "A problem occurred when applying the values from internal"
                               " variable to external");
    }
    else
    {
        _sequenceState.onStop();

        emit finished(true);
    }
}

void NodesSequence::onSequenceError(const ANode &guiltyNode, const QString &error)
{
    if(_stateMachine->isRunning())
    {
        _stateMachine->stop();
    }

    if(_sequenceState.isStopped())
    {
        // No need to go after
        return;
    }

    bool allHasBeenStopped = true;

    for(auto citer = _sequenceNodes.cbegin(); citer != _sequenceNodes.cend(); ++citer)
    {
        if(citer.value()->isActive())
        {
            allHasBeenStopped = false;
            break;
        }
    }

    if(_guiltyNode.isNull())
    {
        qWarning() << "An error occurred in the sequence process";

        _sequenceLogManager.storeLog(SequenceLogData(
                                            LogMsgType::Critical,
                                            SequenceLogCategory::ApplicationScope,
                                            tr("(TR) ==================== The sequence has ended "
                                               "in error: %1 ====================").arg(error)));

        _sequenceLogManager.storeLog(SequenceLogFormatter::formatABusinessProblem(
                                                QString("The sequence has ended in error: %1")
                                                                                    .arg(error)));

        _errorString = error;
        _guiltyNode = findGuiltyNode(guiltyNode);
    }

    if(allHasBeenStopped)
    {
        _sequenceState.onStop();

        emit finished(false);
    }
    else
    {
        _sequenceState.onStopAsked(StopAskedType::EndOfProcess);
    }
}

QSharedPointer<ANode> NodesSequence::findGuiltyNode(const ANode &node)
{
    if(&node == this)
    {
        return _refToThisObject.toStrongRef();
    }

    NodeType::Enum nodeType = node.getNodeType();

    if(nodeType == NodeType::UnknownNode)
    {
        return {};
    }

    if(nodeType == NodeType::ConstantNode || nodeType == NodeType::VariableNode)
    {
        return getReferenceNode(node.getInstanceName());
    }

    return getSequenceNode(node.getInstanceName());
}

bool NodesSequence::applyExternalRefValuesToInternal()
{
    qInfo() << "Apply the reference input values from external source, to internal source";

    QString error;
    QSharedPointer<NodesSequence> refToThis = _refToThisObject.toStrongRef();
    if(!CheckSequenceValidity::isValid(_referencesHolder, refToThis, error))
    {
        _errorString = error;
        _guiltyNode = refToThis;
        return false;
    }

    for(InNodePort &inNode : _referencesHolder.accessInputs())
    {
        const QSharedPointer<AReferenceNode> &extRef = inNode.getInputNode();

        if(extRef.isNull())
        {
            continue;
        }

        const QSharedPointer<AReferenceNode> &intRef = inNode.getInternalInputNode();

        if(intRef->getNodeType() == NodeType::ConstantNode)
        {
            QSharedPointer<ConstantNode> constNode = intRef.objectCast<ConstantNode>();

            if(constNode.isNull())
            {
                qWarning() << "Couldn't cast the reference: " << intRef->getInstanceName()
                           << ", to a constant node";
                return false;
            }

            RETURN_IF_FALSE(constNode->setConstantValue(extRef->getValue()));
        }
        else if (intRef->getNodeType() == NodeType::VariableNode)
        {
            QSharedPointer<VariableNode> varNode = intRef.objectCast<VariableNode>();

            if(varNode.isNull())
            {
                qWarning() << "Couldn't cast the reference: " << intRef->getInstanceName()
                           << ", to a variable node";
                return false;
            }

            RETURN_IF_FALSE(varNode->setVariableValue(NodesSequenceKey(), extRef->getValue()));
        }
    }

    return true;
}

bool NodesSequence::applyInternalVarValuesToExternal()
{
    qInfo() << "Apply the variable output values from internal source, to external source";

    for(OutNodePort &outNode : _referencesHolder.accessOutputs())
    {
        const QSharedPointer<VariableNode> &extVar = outNode.getOutputNode();

        if(extVar.isNull())
        {
            continue;
        }

        const QSharedPointer<VariableNode> &intVar = outNode.getInternalOutputNode();

        QSharedPointer<VariableNode> varNode = intVar.objectCast<VariableNode>();

        if(varNode.isNull())
        {
            // Some output nodes are allowed to not have internal binding, this is verify before
            // start; therefore, if we are here, it means that it's allowed
            continue;
        }

        RETURN_IF_FALSE(extVar->setVariableValue(NodesSequenceKey(), varNode->getValue()));
    }

    return true;
}
