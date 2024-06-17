// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "joinnode.hpp"

#include <QAbstractTransition>
#include <QDebug>
#include <QFinalState>
#include <QSignalTransition>
#include <QState>
#include <QStateMachine>

#include "sequence/accesskeys/targetingsequencenodeskey.hpp"
#include "sequence/nodes/forknode.hpp"
#include "sequence/nodessequence.hpp"
#include "testbedcore/testbedglobal.hpp"


JoinNode::JoinNode(const QSharedPointer<NodesSequence> &sequenceParent, QObject *parent) :
    ATargetingNode(NodeType::JoinNode, sequenceParent, parent)
{
}

QAbstractTransition *JoinNode::addTransitionToThisNode(
                                                    QState &state,
                                                    ASequenceNode &callerNode,
                                                    const TargetingSequenceNodesKey &targetingKey,
                                                    QAbstractTransition *transition)
{
    Q_UNUSED(targetingKey)

    if(callerNode.getInstanceParent() != getInstanceParent())
    {
        qWarning() << "Can't link two nodes which don't have the same node parent";
        return nullptr;
    }

    if(!_forkNode)
    {
        qWarning() << "The fork node hasn't been set, can't add transition to this node";
        return nullptr;
    }

    QFinalState *finalState = _finalNodes.value(&callerNode, nullptr);

    // If a final state already exist, delete it
    delete finalState;

    QState *targetingStateParent = state.parentState();

    QStateMachine *childMachine = qobject_cast<QStateMachine*>(targetingStateParent);

    // If The parent of this state is not a state machine, it means that it's not managed by a fork
    // So we create a finalState but attached to nothing (doesn't matter, because when the fork will
    // be linked to the chain this will be created again)
    finalState = new QFinalState(childMachine);
    connect(finalState, &QFinalState::destroyed,
            this,       [=]() { qDebug() << "Final state destroyed"; });

    _finalNodes[&callerNode] = finalState;

    if(transition == nullptr)
    {
        transition = state.addTransition(&callerNode, &ASequenceNode::exit, finalState);
    }
    else
    {
        transition->setTargetState(finalState);
        state.addTransition(transition);
    }

    return transition;
}

bool JoinNode::removeTransitionWithThisNodeProtect(ASequenceNode &callerNode)
{
    // For primitive types like int and double, as well as for pointer types, the C++ language
    // doesn't specify any initialization; in those cases, Qt's containers automatically initialize
    // the value to 0.
    delete _finalNodes.take(&callerNode);

    return true;
}

QAbstractTransition *JoinNode::setForkNodeLinked(QState &forkState,
                                                 const QSharedPointer<ForkNode> &forkNode,
                                                 const ForkNodeKey &forkNodeKey)
{
    Q_UNUSED(forkNodeKey)
    return setForkNodeLinked(forkState, forkNode);
}

QAbstractTransition *JoinNode::setForkNodeLinked(QState &forkState,
                                                 const QSharedPointer<ForkNode> &forkNode)
{
    _forkNode = forkNode;

    return ASequenceNode::addTransitionToThisNode(forkState, *forkNode, {});
}

bool JoinNode::removeForkNodeLinked()
{
    _forkNode.clear();
    return ASequenceNode::removeTransitionWithThisNodeProtect(*this);
}

void JoinNode::nodeActivity()
{
    ATargetingNode::nodeActivity();

    qDebug() << "Enter Join node activity: " << getInstanceName();

    if(!testStoppingAndPausingAtRunTime())
    {
        return;
    }

    exitProcess();
}

void JoinNode::forgetAllAboutIt(const QSharedPointer<ANode> &nodeToDetachWith)
{
    ATargetingNode::forgetAllAboutIt(nodeToDetachWith);

    if(nodeToDetachWith == _forkNode)
    {
        removeForkNodeLinked();
    }
}

bool JoinNode::changeParentStateIfDifferentWithWaterfallEffectProtect(
                                                    QState *parentStateToCompareWith,
                                                    ASequenceNode &callerNode,
                                                    QPointer<QAbstractState> &modifiedTargetedState)
{
    if((&callerNode == _forkNode || &callerNode == this) &&
       parentStateToCompareWith == getState()->parentState())
    {
        return true;
    }

    return changeParentStateWithWaterfallEffect(parentStateToCompareWith,
                                                callerNode,
                                                modifiedTargetedState);
}

bool JoinNode::changeParentStateWithWaterfallEffect(QState *parentState,
                                                    ASequenceNode &callerNode,
                                                    QPointer<QAbstractState> &modifiedTargetedState)
{
    if(&callerNode == _forkNode || &callerNode == this)
    {
        // The caller is the fork node linked or itself, act normally
        return ATargetingNode::changeParentStateWithWaterfallEffect(parentState,
                                                                    callerNode,
                                                                    modifiedTargetedState);
    }

    QFinalState *finalState = _finalNodes.value(&callerNode, nullptr);

    if(finalState == nullptr)
    {
        qWarning() << "The caller node: " << callerNode.getInstanceName() << " isn't the fork "
                   << "node linked to the join node and is not in the fork loop";
        return false;
    }

    _finalNodes.remove(&callerNode);
    delete finalState;
    modifiedTargetedState = nullptr;

    QStateMachine *childMachine = qobject_cast<QStateMachine*>(parentState);

    if(childMachine == nullptr)
    {
        qWarning() << "The parent of this state is not a state machine, so it's not managed by a "
                   << "fork";
        return false;
    }

    if(getInstanceParent()->getStateMachine() == parentState)
    {
        // The node is exiting the fork
        return true;
    }

    finalState = new QFinalState(childMachine);

    _finalNodes[&callerNode] = finalState;

    modifiedTargetedState = finalState;

    return true;
}
