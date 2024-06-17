// SPDX-FileCopyrightText: 2019 - 2020 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "sequence/nodes/asequencecontainernode.hpp"


/** @brief The group node represents a sub diagram with an init and a final state */
class GroupNode : public ASequenceContainerNode
{
    Q_OBJECT

    public:
        /** @brief Class constuctor
            @param sequenceParent The NodesSequence parent of this node
            @param parent The class parent */
        explicit GroupNode(const QSharedPointer<NodesSequence> &sequenceParent,
                           QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~GroupNode() override;

        /** @brief The Nodes Sequence child of the group node and it will contain all the children
                   nodes
            @param nodesSequence The NodesSequence to link with the group */
        void setNodesSequence(const QSharedPointer<NodesSequence> &nodesSequence);

    public:
        /** @see ASequenceContainerNode::getChildNodesSequence */
        virtual QSharedPointer<NodesSequence> getChildNodesSequence() const override;

    protected:
        /** @see ASequenceContainerNode::forgetAllAboutMe */
        virtual void forgetAllAboutMe() override;

    private:
        QSharedPointer<NodesSequence> _nodesSequence;
};
