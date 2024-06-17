// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "sequence/nodes/asequencenode.hpp"

#include <QSharedPointer>

class QAbstractTransition;
class QState;


/** @brief Abstract node which represents all the nodes which can target one another node
    @note Do not concern Decisional and Fork node */
class ATargetingNode : public ASequenceNode
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param nodeType The type of node
            @param sequenceParent The NodesSequence parent of this node
            @param parent The class parent */
        explicit ATargetingNode(NodeType::Enum nodeType,
                                const QSharedPointer<NodesSequence> &sequenceParent,
                                QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~ATargetingNode() override;

    public:
        /** @brief Add a transition to a node
            @param targetingNode The node to target
            @return True if no problem occurs */
        bool addTransitionToNode(const QSharedPointer<ASequenceNode> &targetingNode);

        /** @brief Remove the actual transition to the node previously set
            @return True if no problem occurs */
        bool removeTransitionToNode();

        /** @brief Get the node that the class targets */
        const QSharedPointer<ASequenceNode> &getNodeTargeted() const { return _targetedNode; }

    protected:
        /** @see ASequenceNode::changeParentStateWithWaterfallEffect */
        virtual bool changeParentStateWithWaterfallEffect(
                                        QState *parentState,
                                        ASequenceNode &callerNode,
                                        QPointer<QAbstractState> &modifiedTargetedState) override;
    protected slots:
        /** @see ANode::forgetAllAboutIt */
        virtual void forgetAllAboutIt(const QSharedPointer<ANode> &nodeToDetachWith) override;

    private:
        /** @brief Cast the abstract state of the ASequenceNode to a QState
            @note Because the state is created in the class constructor with a QState,
                  this might not return a NULL pointer.
            @return The state linked to this node or NULL if there is no state or if the state is
                    not a QState */
        QState *getCastState() const;

    private:
        QAbstractTransition *_transition{nullptr};
        QSharedPointer<ASequenceNode> _targetedNode;
};
