// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "sequence/nodes/atargetingnode.hpp"


/** @brief Reprents a merge node, which is linked with a decisional node and allow to merge
           choices */
class MergeNode : public ATargetingNode
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param sequenceParent The NodesSequence parent of this node
            @param parent The class parent */
        explicit MergeNode(const QSharedPointer<NodesSequence> &sequenceParent,
                           QObject *parent = nullptr);

    protected:
        /** @see ASequenceNode::canItBeTargeted */
        virtual TargetType::Enum canItBeTargeted() override { return TargetType::Multiple; }

    protected slots:
        /** @see ASequenceNode::nodeActivity */
        virtual void nodeActivity() override;
};
