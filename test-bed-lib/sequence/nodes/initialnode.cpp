// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "initialnode.hpp"

#include <QStateMachine>

#include "sequence/accesskeys/asequencenodekey.hpp"
#include "sequence/nodessequence.hpp"
#include "testbedcore/testbedglobal.hpp"


InitialNode::InitialNode(const QSharedPointer<NodesSequence> &sequenceParent, QObject *parent) :
    ATargetingNode(NodeType::InitialNode, sequenceParent, parent)
{
    sequenceParent->accessStateMachine({})->setInitialState(getState());
}

void InitialNode::setState(QAbstractState *state)
{
    ATargetingNode::setState(state);

    getInstanceParent()->accessStateMachine({})->setInitialState(state);
}

void InitialNode::nodeActivity()
{
    ATargetingNode::nodeActivity();

    qDebug() << "Enter Initial node activity: " << getInstanceName();

    if(!testStoppingAndPausingAtRunTime())
    {
        return;
    }

    // Nothing to do more
    exitProcess();
}
