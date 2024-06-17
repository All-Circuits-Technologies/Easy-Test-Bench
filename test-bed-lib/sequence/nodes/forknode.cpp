// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "forknode.hpp"

#include <QAbstractTransition>
#include <QDebug>
#include <QSignalTransition>
#include <QState>
#include <QStateMachine>

#include "collectionutility/qhashhelper.hpp"
#include "sequence/accesskeys/asequencenodekey.hpp"
#include "sequence/accesskeys/forknodekey.hpp"
#include "sequence/accesskeys/targetingsequencenodeskey.hpp"
#include "sequence/nodes/joinnode.hpp"
#include "sequence/nodessequence.hpp"
#include "sequence/sequencenodeshelper.hpp"
#include "statemachineutility/unconditionaltransition.hpp"
#include "testbedcore/testbedglobal.hpp"


ForkNode::ForkNode(const QSharedPointer<NodesSequence> &sequenceParent, QObject *parent) :
    ASequenceNode(NodeType::ForkNode, sequenceParent, parent)
{
    setState(new QState(QState::ParallelStates, sequenceParent->accessStateMachine({}).data()));

    connect(&sequenceParent->getSequenceState(), &SequenceStateHelper::stopAsked,
            this,                                &ForkNode::onStopAskedInSequence);
}

ForkNode::~ForkNode()
{
    forgetAllAboutMe();
}

bool ForkNode::addTransitionToNode(const QSharedPointer<ASequenceNode> &targetedNode)
{
    if(targetedNode.isNull())
    {
        qWarning() << "Can't add transition to a NULL node";
        return false;
    }

    if(!targetedNode.isNull() &&
       !SequenceNodesHelper::verifyIfItsValidToTargetThisNode(targetedNode))
    {
        qWarning() << "The node: " << targetedNode->getInstanceName() << " can't be targeted";
        return false;
    }

    QState *state = getCastState();

    if(!state)
    {
        qWarning() << "Can only add a transition to a QState";
        return false;
    }

    if(_transitions.contains(targetedNode) && !removeTransitionToNode(targetedNode))
    {
        return false;
    }

    ParallelChildState childState{};

    fillParallelChildBranch(childState);

    connect(childState.child,
            &QStateMachine::started,
            this,
            [=]() { qInfo() << "StateMachine started: " << targetedNode->getInstanceName(); });
    connect(childState.child,
            &QStateMachine::finished,
            this,
            [=]() { qInfo() << "StateMachine finished: " << targetedNode->getInstanceName(); });
    connect(childState.child,
            &QStateMachine::stopped,
            this,
            [=]() { qInfo() << "StateMachine stopped: " << targetedNode->getInstanceName(); });

    connect(childState.initialChildOfChild,
            &QState::entered,
            this,
            [=]() { qInfo() << "The initial state has entered: "  << targetedNode->getInstanceName(); });
    connect(childState.initialChildOfChild,
            &QState::exited,
            this,
            [=]() { qInfo() << "The initial state has exited: " << targetedNode->getInstanceName(); });

    childState.transition = new UnconditionalTransition(childState.initialChildOfChild);

    childState.transition = targetedNode->addTransitionToThisNode(
                                                        *childState.initialChildOfChild,
                                                        *this,
                                                        {},
                                                        childState.transition);

    if(!childState.transition)
    {
        delete childState.child;
        return false;
    }

    _transitions.insert(targetedNode, childState);

    return true;
}

bool ForkNode::removeTransitionToNode(const QSharedPointer<ASequenceNode> &targetedNode)
{
    if(!_transitions.contains(targetedNode))
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

    targetedNode->removeTransitionWithThisNode(*this, {});
    qDebug() << "Decide to remove child state";
    removeChildState(_transitions[targetedNode]);
    _transitions.remove(targetedNode);

    return true;
}

QVector<QSharedPointer<ASequenceNode>> ForkNode::getLinkedNodes() const
{
    return QHashHelper::getKeys(_transitions);
}

bool ForkNode::setJoinNodeLinked(const QSharedPointer<JoinNode> &joinNode,
                                 const QSharedPointer<ForkNode> &thisForkNode)
{
    if(!joinNode)
    {
        qWarning() << "The join node given doesn't exist";
        return false;
    }

    QState *state = getCastState();

    if(!state)
    {
        qWarning() << "Can only add a transition to a QState";
        return false;
    }

    RETURN_IF_FALSE(removeTransitionToJoinNode());

    _joinNodetransition = joinNode->setForkNodeLinked(*state, thisForkNode, {});
    _joinNodeLinked = joinNode;

    return (_joinNodetransition != nullptr);
}

void ForkNode::nodeActivity()
{
    ASequenceNode::nodeActivity();

    if(!testStoppingAndPausingAtRunTime())
    {
        return;
    }

    qDebug() << "Enter Fork node activity: " << getInstanceName();
    // Nothing to do here
}

void ForkNode::forgetAllAboutIt(const QSharedPointer<ANode> &nodeToDetachWith)
{
    ASequenceNode::forgetAllAboutIt(nodeToDetachWith);

    QSharedPointer<ASequenceNode> sequenceNode = nodeToDetachWith.objectCast<ASequenceNode>();

    if(sequenceNode.isNull())
    {
        // Not concerned by the next code part
        return;
    }

    if(sequenceNode == this)
    {
        forgetAllAboutMe();
    }

    if(_transitions.contains(sequenceNode))
    {
        removeTransitionToNode(sequenceNode);
    }

    if(_joinNodeLinked == sequenceNode)
    {
        removeTransitionToJoinNode();
    }
}

void ForkNode::forgetAllAboutMe()
{
    qDebug() << "Remove forkNode elements: " << getInstanceName();
    QList<QSharedPointer<ASequenceNode>> toDelete = _transitions.keys();
    for(const QSharedPointer<ASequenceNode> &node : toDelete)
    {
        removeTransitionToNode(node);
    }

    removeTransitionToJoinNode();
}

void ForkNode::onEndOfStateMachine()
{
    if(!testStoppingAndPausingAtRunTime())
    {
        return;
    }

    bool allHasBeenStopped = true;

    QHash<QSharedPointer<ASequenceNode>, ParallelChildState>::const_iterator iter =
                                                                            _transitions.cbegin();
    for(; iter != _transitions.cend(); ++iter)
    {
        if(iter.value().child->isRunning())
        {
            allHasBeenStopped = false;
            break;
        }
    }

    if(allHasBeenStopped)
    {
        if(!_joinNodeLinked)
        {
            qWarning() << "There is no join node linked to this fork, can't proceed";
            return;
        }

        exitProcess();
    }
}

void ForkNode::onStopAskedInSequence(StopAskedType::Enum /*stopAskedType*/)
{
    for(auto citer = _transitions.cbegin(); citer != _transitions.cend(); ++citer)
    {
        if(citer.value().child->isRunning())
        {
            citer.value().child->stop();
        }
    }

    errorProcess(*this, "Stop has been asked");
}

bool ForkNode::changeParentStateWithWaterfallEffect(QState *parentState,
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

    QState *newState = new QState(QState::ParallelStates, parentState);

    if(_joinNodetransition && _joinNodetransition->targetState() != nullptr)
    {
        state->removeTransition(_joinNodetransition);
    }

    setState(newState);

    modifiedTargetedState = newState;

    if(_joinNodetransition)
    {
        QPointer<QAbstractState> targetState;
        RETURN_IF_FALSE(
                    _joinNodeLinked->changeParentStateIfDifferentWithWaterfallEffect(parentState,
                                                                                     *this,
                                                                                     targetState,
                                                                                     {}));

        _joinNodetransition->setTargetState(targetState);
        newState->addTransition(_joinNodetransition);
    }

    // Change parent state for all children

    QHash<QSharedPointer<ASequenceNode>, ParallelChildState>::const_iterator iter =
                                                                            _transitions.cbegin();
    for(; iter != _transitions.cbegin(); ++iter)
    {
        RETURN_IF_FALSE(removeTransitionToNode(iter.key()));
        RETURN_IF_FALSE(addTransitionToNode(iter.key()));
    }

    return true;
}

void ForkNode::setState(QAbstractState *state)
{
    QState *newCastState = qobject_cast<QState*>(state);

    ASequenceNode::setState(newCastState);

    if(newCastState)
    {
        connect(state, &QState::exited, this, [=]() { qInfo() << "The main state is exited"; });

        connect(newCastState, &QState::exited, this, &ASequenceNode::exit);
    }
}

bool ForkNode::removeTransitionToJoinNode()
{
    if(!_joinNodetransition)
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

    RETURN_IF_FALSE(_joinNodeLinked->removeTransitionWithThisNode(*this, {}));

    if(_joinNodetransition->targetState() != nullptr)
    {
        state->removeTransition(_joinNodetransition);
    }

    delete _joinNodetransition;
    _joinNodetransition = nullptr;

    _joinNodeLinked.clear();

    return true;
}

QState *ForkNode::getCastState() const
{
    return qobject_cast<QState*>(getState());
}

void ForkNode::fillParallelChildBranch(ParallelChildState &parallel)
{
    QStateMachine *directChild = new QStateMachine(getCastState());
    QState *initialState = new QState(directChild);

    directChild->setInitialState(initialState);

    connect(directChild, &QStateMachine::finished, this, &ForkNode::onEndOfStateMachine);
    connect(directChild,
            &QStateMachine::destroyed,
            this,
            [=](){ qDebug() << "Fork child machine destroyed"; });

    parallel.child = directChild;
    parallel.initialChildOfChild = initialState;
}

void ForkNode::removeChildState(ParallelChildState &childState)
{
    if(childState.transition->targetState() != nullptr)
    {
        childState.initialChildOfChild->removeTransition(childState.transition);
    }

    delete childState.transition;
    childState.transition = nullptr;

    delete childState.initialChildOfChild;
    childState.initialChildOfChild = nullptr;

    delete childState.child;
    childState.child = nullptr;
}
