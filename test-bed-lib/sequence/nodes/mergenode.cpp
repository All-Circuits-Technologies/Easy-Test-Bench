// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "mergenode.hpp"

#include <QDebug>

#include "testbedcore/testbedglobal.hpp"


MergeNode::MergeNode(const QSharedPointer<NodesSequence> &sequenceParent, QObject *parent) :
    ATargetingNode(NodeType::MergeNode, sequenceParent, parent)
{
}

void MergeNode::nodeActivity()
{
    ATargetingNode::nodeActivity();

    qDebug() << "Enter merge node activity: " << getInstanceName();

    if(!testStoppingAndPausingAtRunTime())
    {
        return;
    }

    exitProcess();
}
