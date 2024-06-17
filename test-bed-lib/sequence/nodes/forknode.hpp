// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "sequence/nodes/asequencenode.hpp"

#include <QHash>

#include "testbedcore/types/stopaskedtype.hpp"

class QAbstractTransition;
class JoinNode;
class JoinNodeKey;
class QStateMachine;
class QState;


/** @brief The fork node is useful to represent a parallelization of serveral paths
    @note The node contains a state machine for the different paths
    @note The Fork node is linked to one and only one join node, all the node between the two are
          contained in state machines managed by this node */
class ForkNode : public ASequenceNode
{
    Q_OBJECT

    public:
        /** @brief Class constuctor
            @param sequenceParent The NodesSequence parent of this node
            @param parent The class parent */
        explicit ForkNode(const QSharedPointer<NodesSequence> &sequenceParent,
                          QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~ForkNode() override;

        /** @brief Add a transition to the node targeted
            @param targetedNode The node to target
            @return True If no problem occurs */
        bool addTransitionToNode(const QSharedPointer<ASequenceNode> &targetedNode);

        /** @brief Remove one transition to a node previously targeted
            @param targetedNode The targeted to no more target
            @return True If a problem occurs */
        bool removeTransitionToNode(const QSharedPointer<ASequenceNode> &targetedNode);

        /** @brief Get all the nodes linked with this node */
        QVector<QSharedPointer<ASequenceNode>> getLinkedNodes() const;

        /** @brief Set the join node linked to this fork node
            @warning The link between the two is important because it allowes to manage the nodes
                     between
            @note This method will call the @ref JoinNode::setForkNodeLinked method
            @param joinNode The join node linked to this fork node
            @param forkNode This fork node, we need the QSharedPointer to give to the JoinNode
            @return True if a problem occurred */
        bool setJoinNodeLinked(const QSharedPointer<JoinNode> &joinNode,
                               const QSharedPointer<ForkNode> &thisForkNode);

        /** @brief Get join node linked */
        const QSharedPointer<JoinNode> &getJoinNodeLinked() const { return _joinNodeLinked; }

        /** @see ASequenceNode::canTargetNode */
        virtual TargetType::Enum canTargetNode() override { return TargetType::Multiple; }

    protected slots:
        /** @see ASequenceNode::nodeActivity */
        virtual void nodeActivity() override;

        /** @see ANode::forgetAllAboutIt */
        virtual void forgetAllAboutIt(const QSharedPointer<ANode> &nodeToDetachWith) override;

        /** @brief Same as forgetAllAboutIt but manages the case where the node to detach is this
                   instance
            @see ANode::forgetAllAboutIt */
        void forgetAllAboutMe();

    private slots:
        /** @brief Called when all the parallel paths are ended */
        void onEndOfStateMachine();

        /** @brief Called when a stop has been asked for the sequence
            @param stopAskedType Give a contact to the stop asking */
        void onStopAskedInSequence(StopAskedType::Enum stopAskedType);

    protected:
        /** @see ASequenceNode::changeParentStateWithWaterfallEffect */
        virtual bool changeParentStateWithWaterfallEffect(
                                        QState *parentState,
                                        ASequenceNode &callerNode,
                                        QPointer<QAbstractState> &modifiedTargetedState) override;

        /** @see ASequenceNode::setState */
        virtual void setState(QAbstractState *state) override;

    private:
        /** @brief Remove the transition to the JoinNode linked
            @return True if no problem occurs */
        bool removeTransitionToJoinNode();

    private:
        /** @brief Represents a parallel path */
        struct ParallelChildState
        {
            QAbstractTransition *transition;
            QStateMachine *child;
            QState *initialChildOfChild;
        };

    private:
        /** @brief Cast the abstract state of the ASequenceNode to a QState
            @note Because the state is created in the class constructor with a QState,
                  this might return a non NULL pointer.
            @return The state linked to this node or NULL if there is no state or if the state is
                    not a QState */
        QState *getCastState() const;

        /** @brief Init a parallel path and create all the states needed
            @param parallel The parallel path to fill */
        void fillParallelChildBranch(ParallelChildState &parallel);

        /** @brief Remove a parallel path
            @param childState The parallel path to remove */
        void removeChildState(ParallelChildState &childState);

    private:
        QHash<QSharedPointer<ASequenceNode>, ParallelChildState> _transitions;

        QSharedPointer<JoinNode> _joinNodeLinked;
        QAbstractTransition *_joinNodetransition{nullptr};
};
