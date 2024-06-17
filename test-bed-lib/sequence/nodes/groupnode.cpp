// SPDX-FileCopyrightText: 2019 - 2020 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "groupnode.hpp"

#include "sequence/nodessequence.hpp"
#include "testbedcore/testbedglobal.hpp"


GroupNode::GroupNode(const QSharedPointer<NodesSequence> &sequenceParent, QObject *parent) :
    ASequenceContainerNode(NodeType::GroupNode, sequenceParent, parent)
{
}

GroupNode::~GroupNode()
{
    forgetAllAboutMe();
}

void GroupNode::setNodesSequence(const QSharedPointer<NodesSequence> &nodesSequence)
{
    _nodesSequence = nodesSequence;
}

QSharedPointer<NodesSequence> GroupNode::getChildNodesSequence() const
{
    return _nodesSequence;
}

void GroupNode::forgetAllAboutMe()
{
    if(!_nodesSequence.isNull())
    {
        _nodesSequence->unloadAll();
        _nodesSequence.clear();
    }
}
