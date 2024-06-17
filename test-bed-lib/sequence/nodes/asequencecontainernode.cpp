// SPDX-FileCopyrightText: 2020 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "asequencecontainernode.hpp"

#include "testbedcore/parsers/jsonparsersconstants.hpp"
#include "testbedcore/testbedglobal.hpp"
#include "sequence/accesskeys/asequencecontainernodekey.hpp"
#include "sequence/logs/sequencelogmanager.hpp"
#include "sequence/nodessequence.hpp"
#include "sequence/referencenodes/variablenode.hpp"


ASequenceContainerNode::ASequenceContainerNode(NodeType::Enum nodeType,
                                               const QSharedPointer<NodesSequence> &sequenceParent,
                                               QObject *parent) :
    ATargetingNode(nodeType, sequenceParent, parent)
{
}

void ASequenceContainerNode::onChildNodesSequenceFinished(bool success)
{
    QSharedPointer<NodesSequence> childNode = getChildNodesSequence();

    manageConnectionToSequence(false, childNode);

    if(!testStoppingAndPausingAtRunTime())
    {
        return;
    }

    QString errorStr;
    if(!dealWithOutputWriting(childNode, success, errorStr))
    {
        errorProcess(*this, errorStr);
        return;
    }

    exitProcess();
}

void ASequenceContainerNode::nodeActivity()
{
    ATargetingNode::nodeActivity();

    if(!testStoppingAndPausingAtRunTime())
    {
        return;
    }

    QString errorStr;
    if(!preparingSequenceBeforeStart(errorStr))
    {
        qWarning() << "An error occurred while preparing the sequence to start";
        errorProcess(*this, errorStr);
        return;
    }

    QSharedPointer<NodesSequence> nodesSequence = getChildNodesSequence();

    if(!nodesSequence)
    {
        qWarning() << "The child nodes sequence given is null, can't start the process";
        errorProcess(*this, "The child nodes sequence of the container node is null, can't start "
                          "the node activity");
        return;
    }

    manageConnectionToSequence(true, nodesSequence);

    if(!nodesSequence->startSequence())
    {
        manageConnectionToSequence(false, nodesSequence);
        qWarning() << "An error occurred when tryied to start the sequence: "
                   << nodesSequence->getInstanceName() << ", child of: " << getInstanceName();
        errorProcess(*this, "The nodesSequence linked to this node can't be started");
    }
}

void ASequenceContainerNode::forgetAllAboutIt(const QSharedPointer<ANode> &nodeToDetachWith)
{
    ATargetingNode::forgetAllAboutIt(nodeToDetachWith);

    if(nodeToDetachWith.data() == this)
    {
        forgetAllAboutMe();
    }
}

void ASequenceContainerNode::manageConnectionToSequence(
                                                    bool connectNode,
                                                    const QSharedPointer<NodesSequence> &childNode)
{
    const SequenceLogManager &logManager = childNode->getSequenceLogManager();

    if(connectNode)
    {
        connect(childNode.data(), &NodesSequence::finished,
                this,             &ASequenceContainerNode::onChildNodesSequenceFinished);
        connect(&logManager, &SequenceLogManager::newLogAdded,
                this,        &ASequenceContainerNode::newLog);
        connect(&getInstanceParent()->getSequenceState(), &SequenceStateHelper::newState,
                this,                                     &ASequenceContainerNode::onNewState);
        connect(&getInstanceParent()->getSequenceState(), &SequenceStateHelper::stopAsked,
                this,                                     &ASequenceContainerNode::onStopAsked);
    }
    else
    {
        disconnect(childNode.data(), &NodesSequence::finished,
                   this,             &ASequenceContainerNode::onChildNodesSequenceFinished);
        disconnect(&logManager, &SequenceLogManager::newLogAdded,
                   this,        &ASequenceContainerNode::newLog);
        disconnect(&getInstanceParent()->getSequenceState(), &SequenceStateHelper::newState,
                   this,                                     &ASequenceContainerNode::onNewState);
        disconnect(&getInstanceParent()->getSequenceState(), &SequenceStateHelper::stopAsked,
                   this,                                     &ASequenceContainerNode::onStopAsked);
    }
}

bool ASequenceContainerNode::dealWithOutputWriting(const QSharedPointer<NodesSequence> &childNode,
                                                   bool childSequenceSuccess,
                                                   QString &error)
{
    if(_failIfAnErrorOccured && !childSequenceSuccess)
    {
        // An error occurred and we raise an error if fail, there we process it
        error = "An error occurred on the nodesSequence linked: " +
                childNode->getSequenceStringError();
        qWarning() << error;
        return false;
    }

    QSharedPointer<VariableNode> errorOccuredVar;

    for(const OutNodePort &outNode : childNode->getReferencesHolder().getOutputs())
    {
        if(outNode.getName() == JsonParsersConstants::Sequence::SequenceSuccessOutput)
        {
            errorOccuredVar = outNode.getOutputNode();
            break;
        }
    }

    if(_failIfAnErrorOccured && !errorOccuredVar)
    {
        // The variable is not compulsory if the child sequence fails in case of error
        return true;
    }

    if(!errorOccuredVar)
    {
        error = "Miss the output: " +
                QString(JsonParsersConstants::Sequence::SequenceSuccessOutput) +
                "in the container node: " + getInstanceName() + ", this can't happen";
        qWarning() << error;
        return false;
    }

    if(!errorOccuredVar->setVariableValue(ASequenceContainerNodeKey(), childSequenceSuccess))
    {
        error = "A problem occurred when trying to write the boolean in the output: " +
                QString(JsonParsersConstants::Sequence::SequenceSuccessOutput) +
                "of the container node: " + getInstanceName();
        qWarning() << error;
        return false;
    }

    return true;
}

bool ASequenceContainerNode::preparingSequenceBeforeStart(QString &error)
{
    Q_UNUSED(error)
    // Nothing to do here
    return true;
}
void ASequenceContainerNode::onNewState(SequenceStateType::Enum newState,
                                        SequenceStateType::Enum previousState)
{
    QSharedPointer<NodesSequence> tmpChildNode = getChildNodesSequence();

    if(tmpChildNode.isNull())
    {
        return;
    }

    if((newState == SequenceStateType::Running) && (previousState == SequenceStateType::InPause))
    {
        tmpChildNode->managePauseAndResume(false);
    }
    else if(newState == SequenceStateType::InPause)
    {
        tmpChildNode->managePauseAndResume(true);
    }
}

void ASequenceContainerNode::onStopAsked(StopAskedType::Enum stopAskedType)
{
    QSharedPointer<NodesSequence> tmpChildNode = getChildNodesSequence();

    if(tmpChildNode.isNull())
    {
        return;
    }

    tmpChildNode->stopSequence(stopAskedType);
}
