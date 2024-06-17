// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "sequence/nodes/atargetingnode.hpp"

class ForkNode;
class ForkNodeKey;
class QFinalState;


/** @brief This node represents a join node and it's closely linked with a fork node */
class JoinNode : public ATargetingNode
{
    Q_OBJECT

    public:
        /** @brief Class constuctor
            @param sequenceParent The NodesSequence parent of this node
            @param parent The class parent */
        explicit JoinNode(const QSharedPointer<NodesSequence> &sequenceParent,
                          QObject *parent = nullptr);

    public:
        /** @brief Set the fork node linked to this join node
            @param forkState The state of the fork
            @param forkNode The fork node linked to this
            @param forkNodeKey The key only allowes authorized class instance to call this method
            @return The transition created or modified, NULL if a problem occurred */
        QAbstractTransition *setForkNodeLinked(QState &forkState,
                                               const QSharedPointer<ForkNode> &forkNode,
                                               const ForkNodeKey &forkNodeKey);

        /** @brief Get Fork node linked with this node */
        const QSharedPointer<ForkNode> &getForkNodeLinked() const { return _forkNode; }

        /** @brief Remove the link with the fork node linked
            @warning This method has to be only called by the fork node linked, DO NOT USE IT
            @return True If no problem occurs */
        bool removeForkNodeLinked();

    public:
        /** @see ASequenceNode::addTransitionToThisNode
            @note The method will call the fork node special function in order to add the last
                  node as the final node of the sub state machine */
        virtual QAbstractTransition *addTransitionToThisNode(
                                    QState &state,
                                    ASequenceNode &callerNode,
                                    const TargetingSequenceNodesKey &targetingKey,
                                    QAbstractTransition *transition = Q_NULLPTR) override;

    protected:
        /** @see ASequenceNode::canItBeTargeted */
        virtual TargetType::Enum canItBeTargeted() override { return TargetType::Multiple; }

        /** @see ASequenceNode::removeTransitionWithThisNodeProtect */
        virtual bool removeTransitionWithThisNodeProtect(ASequenceNode &callerNode) override;

    protected slots:
        /** @see ASequenceNode::nodeActivity */
        virtual void nodeActivity() override;

        /** @see ANode::forgetAllAboutIt */
        virtual void forgetAllAboutIt(const QSharedPointer<ANode> &nodeToDetachWith) override;

    protected:
        /** @see ASequenceNode::changeParentStateIfDifferentWithWaterfallEffectProtect */
        virtual bool changeParentStateIfDifferentWithWaterfallEffectProtect(
                                        QState *parentStateToCompareWith,
                                        ASequenceNode &callerNode,
                                        QPointer<QAbstractState> &modifiedTargetedState) override;

        /** @see ASequenceNode::changeParentStateWithWaterfallEffect */
        virtual bool changeParentStateWithWaterfallEffect(
                                        QState *parentState,
                                        ASequenceNode &callerNode,
                                        QPointer<QAbstractState> &modifiedTargetedState) override;

    private:
        /** @brief Set the fork node linked
            @param forkState The fork state
            @param forkNode The fork node linked
            @return The transition between the fork node and the join node, or NULL if a problem
                    occurs */
        QAbstractTransition *setForkNodeLinked(QState &forkState,
                                               const QSharedPointer<ForkNode> &forkNode);

    private:
        QSharedPointer<ForkNode> _forkNode;
        QHash<ASequenceNode*, QFinalState *> _finalNodes;
};
