// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "sequence/nodes/atargetingnode.hpp"


/** @brief Represents the initial node of the sequence */
class InitialNode : public ATargetingNode
{
    Q_OBJECT

    public:
        /** @brief Class constuctor
            @param sequenceParent The NodesSequence parent of this node
            @param parent The class parent */
        explicit InitialNode(const QSharedPointer<NodesSequence> &sequenceParent,
                             QObject *parent = nullptr);

    protected:
        /** @see ASequenceNode::canItBeTargeted */
        virtual TargetType::Enum canItBeTargeted() override { return TargetType::None; }

        /** @see ASequenceNode::setState */
        virtual void setState(QAbstractState *state) override;

    protected slots:
        /** @see ASequenceNode::nodeActivity */
        virtual void nodeActivity() override;
};
