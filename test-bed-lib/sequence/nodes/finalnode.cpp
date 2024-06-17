// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "finalnode.hpp"

#include <QStateMachine>
#include <QFinalState>

#include "sequence/accesskeys/asequencenodekey.hpp"
#include "sequence/nodessequence.hpp"
#include "testbedcore/testbedglobal.hpp"


FinalNode::FinalNode(const QSharedPointer<NodesSequence> &sequenceParent, QObject *parent) :
    ASequenceNode(NodeType::FinalNode, sequenceParent, parent)
{
    setState(new QFinalState(sequenceParent->accessStateMachine({}).data()));
}

void FinalNode::nodeActivity()
{
    ASequenceNode::nodeActivity();

    qDebug() << "Enter final node activity: " << getInstanceName();

    if(!testStoppingAndPausingAtRunTime())
    {
        return;
    }

    // Nothing to do more

    exitProcess();
}

QFinalState *FinalNode::getCastState() const
{
    return qobject_cast<QFinalState*>(getState());
}

bool FinalNode::changeParentStateWithWaterfallEffect(
                                                    QState *parentState,
                                                    ASequenceNode &callerNode,
                                                    QPointer<QAbstractState> &modifiedTargetedState)
{
    Q_UNUSED(callerNode)

    QFinalState *finalState = getCastState();

    if(!finalState)
    {
        qWarning() << "Can't change parent, the state is not a final state";
        return false;
    }

    QFinalState *newState = new QFinalState(parentState);

    setState(newState);

    modifiedTargetedState = newState;

    return true;
}
