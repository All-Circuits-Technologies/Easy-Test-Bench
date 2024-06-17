// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "sequence/nodes/asequencenode.hpp"

class QFinalState;


/** @brief The final node represents the final state of the state machine */
class FinalNode : public ASequenceNode
{
    Q_OBJECT

    public:
        /** @brief Class constuctor
            @param sequenceParent The NodesSequence parent of this node
            @param parent The class parent */
        explicit FinalNode(const QSharedPointer<NodesSequence> &sequenceParent,
                           QObject *parent = nullptr);

    protected:
        /** @see ASequenceNode::canTargetNode */
        virtual TargetType::Enum canTargetNode() override { return TargetType::None; }

    protected slots:
        /** @see ASequenceNode::nodeActivity */
        virtual void nodeActivity() override;

    private:
        /** @brief Cast the abstract state of the ASequenceNode to a QState
            @note Because the state is created in the class constructor with a QState,
                  this might return a non NULL pointer.
            @return The state linked to this node or NULL if there is no state or if the state is
                    not a QState */
        QFinalState *getCastState() const;

    protected:
        /** @see ASequenceNode::changeParentStateWithWaterfallEffect */
        virtual bool changeParentStateWithWaterfallEffect(
                                        QState *parentState,
                                        ASequenceNode &callerNode,
                                        QPointer<QAbstractState> &modifiedTargetedState) override;
};
