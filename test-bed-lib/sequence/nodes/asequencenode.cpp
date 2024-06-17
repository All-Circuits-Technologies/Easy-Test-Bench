// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "asequencenode.hpp"

#include <QAbstractTransition>
#include <QDebug>
#include <QSignalTransition>
#include <QState>
#include <QStateMachine>

#include "sequence/accesskeys/asequencenodekey.hpp"
#include "sequence/accesskeys/targetingsequencenodeskey.hpp"
#include "sequence/nodessequence.hpp"
#include "testbedcore/testbedglobal.hpp"
#include "testbedcore/utility/pauseandstophelper.hpp"


ASequenceNode::ASequenceNode(NodeType::Enum nodeType,
                             const QSharedPointer<NodesSequence> &sequenceParent,
                             QObject *parent) :
    ANode(nodeType, sequenceParent, parent)
{
}

ASequenceNode::~ASequenceNode()
{
    setState(nullptr);
}

QAbstractTransition *ASequenceNode::addTransitionToThisNode(
                                                    QState &state,
                                                    ASequenceNode &callerNode,
                                                    const TargetingSequenceNodesKey &targetingKey,
                                                    QAbstractTransition *transition)
{
    Q_UNUSED(targetingKey)

    if(canItBeTargeted() == TargetType::None)
    {
        qWarning() << "The node can't be targeted";
        return nullptr;
    }

    if(callerNode.getInstanceParent() != getInstanceParent())
    {
        qWarning() << "Can't link two nodes which don't have the same node parent";
        return nullptr;
    }

    QPointer<QAbstractState> pointer;

    if(!changeParentStateIfDifferentWithWaterfallEffectProtect(state.parentState(),
                                                               callerNode,
                                                               pointer))
    {
        qWarning() << "The state can't change its state parent";
        return nullptr;
    }

    if(transition == nullptr)
    {
        transition =  state.addTransition(&callerNode, &ASequenceNode::exit, _state);
    }
    else
    {
        transition->setTargetState(_state);
        state.addTransition(transition);
    }

    return transition;
}

bool ASequenceNode::removeTransitionWithThisNode(ASequenceNode &callerNode,
                                                 const TargetingSequenceNodesKey &targetingKey)
{
    Q_UNUSED(targetingKey)
    return removeTransitionWithThisNodeProtect(callerNode);
}

bool ASequenceNode::removeTransitionWithThisNodeProtect(ASequenceNode &callerNode)
{
    QPointer<QAbstractState> pointer;
    return changeParentStateIfDifferentWithWaterfallEffectProtect(
                                                getInstanceParent()->accessStateMachine({}).data(),
                                                callerNode,
                                                pointer);
}

bool ASequenceNode::changeParentStateIfDifferentWithWaterfallEffect(
                                                QState *parentStateToCompareWith,
                                                ASequenceNode &callerNode,
                                                QPointer<QAbstractState> &modifiedTargetedState,
                                                const TargetingSequenceNodesKey &targetingKey)
{
    Q_UNUSED(targetingKey)

    return changeParentStateIfDifferentWithWaterfallEffectProtect(parentStateToCompareWith,
                                                                  callerNode,
                                                                  modifiedTargetedState);
}

void ASequenceNode::nodeActivity()
{
    _isActive = true;
}

bool ASequenceNode::testStoppingAndPausingAtRunTime()
{
    if(!PauseAndStopHelper::managePauseAndStop(accessInstanceParent()->getSequenceState()))
    {
        errorProcess(*this, "Stop has been asked or a problem occurred when pausing/stopping");
        return false;
    }

    return true;
}

void ASequenceNode::exitProcess()
{
    if(_isActive)
    {
        _isActive = false;
        emit exit();
    }
}

void ASequenceNode::errorProcess(const ANode &guiltyNode, const QString &errorTxt)
{
    if(_isActive)
    {
        _isActive = false;
        emit error(guiltyNode, errorTxt);
    }
}

bool ASequenceNode::changeParentStateIfDifferentWithWaterfallEffectProtect(
                                                    QState *parentStateToCompareWith,
                                                    ASequenceNode &callerNode,
                                                    QPointer<QAbstractState> &modifiedTargetedState)
{
    if(parentStateToCompareWith == _state->parentState())
    {
        return true;
    }

    return changeParentStateWithWaterfallEffect(parentStateToCompareWith,
                                                callerNode,
                                                modifiedTargetedState);
}

void ASequenceNode::setState(QAbstractState *state)
{
    delete _state;

    _state = state;

    if(_state != nullptr)
    {
        connect(_state, &QAbstractState::entered, this, &ASequenceNode::nodeActivity);
    }
}
