// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "anode.hpp"

#include <QDebug>

#include "sequence/nodessequence.hpp"


ANode::ANode(NodeType::Enum nodeType,
             const QSharedPointer<NodesSequence> &sequenceParent,
             QObject *parent) :
    QObject(parent),
    _nodeType(nodeType),
    _parent(sequenceParent)
{
    if(sequenceParent)
    {
        connect(sequenceParent.data(), &NodesSequence::hasToDie,
                this,                  &ANode::forgetAllAboutIt);
    }
}

ANode::~ANode()
{
    qDebug() << "Destroying object: " << getInstanceName();
}

void ANode::forgetAllAboutIt(const QSharedPointer<ANode> &nodeToDetachWith)
{
    if(nodeToDetachWith == _parent)
    {
        _parent.clear();
    }
}
