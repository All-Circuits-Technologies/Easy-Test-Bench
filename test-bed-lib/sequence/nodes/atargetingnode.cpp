// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "atargetingnode.hpp"

#include <QAbstractTransition>
#include <QDebug>
#include <QState>
#include <QStateMachine>

#include "sequence/accesskeys/asequencenodekey.hpp"
#include "sequence/accesskeys/targetingsequencenodeskey.hpp"
#include "sequence/nodessequence.hpp"
#include "sequence/sequencenodeshelper.hpp"
#include "testbedcore/testbedglobal.hpp"


ATargetingNode::ATargetingNode(NodeType::Enum nodeType,
                               const QSharedPointer<NodesSequence> &sequenceParent,
                               QObject *parent) :
    ASequenceNode(nodeType, sequenceParent, parent)
{
    setState(new QState(sequenceParent->accessStateMachine({}).data()));
}

ATargetingNode::~ATargetingNode()
{
    removeTransitionToNode();
}

bool ATargetingNode::addTransitionToNode(const QSharedPointer<ASequenceNode> &targetingNode)
{
    if(canTargetNode() == TargetType::None)
    {
        qWarning() << "The node: " << getInstanceName() << " can't target an another node";
        return false;
    }

    if(!targetingNode)
    {
        qWarning() << "The targeting node can't be null";
        return false;
    }

    RETURN_IF_FALSE(SequenceNodesHelper::verifyIfItsValidToTargetThisNode(targetingNode));

    QState *state = getCastState();

    if(!state)
    {
        qWarning() << "Can only add a transition to a QState";
        return false;
    }

    RETURN_IF_FALSE(removeTransitionToNode());

    _transition = targetingNode->addTransitionToThisNode(*state, *this, {});
    _targetedNode = targetingNode;

    return (_transition != nullptr);
}

bool ATargetingNode::removeTransitionToNode()
{
    if(!_transition)
    {
        // No transition to remove
        return true;
    }

    QState *state = getCastState();

    if(!state)
    {
        qWarning() << "Can only manage transitions with QState";
        return false;
    }

    RETURN_IF_FALSE(_targetedNode->removeTransitionWithThisNode(*this, {}));

    if(_transition->targetState() != nullptr)
    {
        state->removeTransition(_transition);
    }

    delete _transition;
    _transition = nullptr;

    _targetedNode.clear();

    return true;
}

bool ATargetingNode::changeParentStateWithWaterfallEffect(
                                                    QState *parentState,
                                                    ASequenceNode &callerNode,
                                                    QPointer<QAbstractState> &modifiedTargetedState)
{
    Q_UNUSED(callerNode)

    QState *state = getCastState();

    if(!state)
    {
        qWarning() << "Can only manage transitions with QState";
        return false;
    }

    QState *newState = new QState(parentState);

    if(_targetedNode && _transition->targetState() != nullptr)
    {
        state->removeTransition(_transition);
    }

    setState(newState);

    modifiedTargetedState = newState;

    if(!_targetedNode)
    {
        // No transition to remove
        return true;
    }

    QPointer<QAbstractState> targetState;
    RETURN_IF_FALSE(_targetedNode->changeParentStateIfDifferentWithWaterfallEffect(parentState,
                                                                                   *this,
                                                                                   targetState,
                                                                                   {}));

    _transition->setTargetState(targetState);

    if(!targetState)
    {
        // The new parent can't be targeted, remove the transition to node
        return removeTransitionToNode();
    }

    newState->addTransition(_transition);

    return true;
}

void ATargetingNode::forgetAllAboutIt(const QSharedPointer<ANode> &nodeToDetachWith)
{
    ASequenceNode::forgetAllAboutIt(nodeToDetachWith);

    if(nodeToDetachWith == _targetedNode)
    {
        removeTransitionToNode();
    }
}

QState *ATargetingNode::getCastState() const
{
    return qobject_cast<QState*>(getState());
}
