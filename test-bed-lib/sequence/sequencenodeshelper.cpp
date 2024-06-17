// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "sequencenodeshelper.hpp"

#include "sequence/nodes/atargetingnode.hpp"
#include "sequence/nodes/decisionalnode.hpp"
#include "sequence/nodes/forknode.hpp"
#include "sequence/nodessequence.hpp"
#include "testbedcore/testbedglobal.hpp"


bool SequenceNodesHelper::isTargetingNode(
                                    const QSharedPointer<ASequenceNode> &nodeTargeted,
                                    const QSharedPointer<ASequenceNode> &potentiallyTargetingNode)
{
    if(potentiallyTargetingNode->getNodeType() == NodeType::ForkNode)
    {
        QSharedPointer<ForkNode> forkNode = potentiallyTargetingNode.objectCast<ForkNode>();

        return forkNode->getLinkedNodes().contains(nodeTargeted);
    }

    if(potentiallyTargetingNode->getNodeType() == NodeType::DecisionalNode)
    {
        QSharedPointer<DecisionalNode> decisionalNode =
                                            potentiallyTargetingNode.objectCast<DecisionalNode>();

        return (decisionalNode->getDefaultNode() == nodeTargeted) ||
               (decisionalNode->getLinkedNodes().contains(nodeTargeted));
    }

    QSharedPointer<ATargetingNode> targetingOtherNode =
                                            potentiallyTargetingNode.objectCast<ATargetingNode>();

    return (targetingOtherNode && (targetingOtherNode->getNodeTargeted() == nodeTargeted));
}

bool SequenceNodesHelper::verifyIfItsValidToTargetThisNode(
                                                        const QSharedPointer<ASequenceNode> &node)
{
    return (node->canItBeTargeted() >= getCurrentNumberOfTargeters(node));
}

TargetType::Enum SequenceNodesHelper::getCurrentNumberOfTargeters(
                                                        const QSharedPointer<ASequenceNode> &node)
{
    const QSharedPointer<NodesSequence> &parent = node->getInstanceParent();

    TargetType::Enum targetType = TargetType::None;

    // Look for all the other nodes, if there are some which are targeting this one
    QVector<QString> nodeNames = parent->getSequenceNodesName();
    for(auto citer = nodeNames.cbegin(); citer != nodeNames.cend(); ++citer)
    {
        QSharedPointer<ASequenceNode> otherNode = parent->getSequenceNode(*citer);

        if(otherNode == node)
        {
            // Don't test itself, continue
            continue;
        }

        if(SequenceNodesHelper::isTargetingNode(node, otherNode))
        {
            if(targetType == TargetType::None)
            {
                targetType = TargetType::OnlyOne;
            }
            else if(targetType == TargetType::OnlyOne)
            {
                targetType = TargetType::Multiple;

                // There is no step further multiple ; therefore, it's not necessary to iterate on
                // all elements.
                break;
            }
        }
    }

    return targetType;
}
