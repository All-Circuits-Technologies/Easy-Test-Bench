// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "sequence/nodes/anode.hpp"

#include <QPointer>

#include "sequence/corestatemachine/targettype.hpp"

class QAbstractTransition;
class QState;
class TargetingSequenceNodesKey;


/** @brief Abstract for all the sequence nodes and contains shared elements */
class ASequenceNode : public ANode
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param nodeType The type of node
            @param sequenceParent The NodesSequence parent of this node
            @param parent The class parent */
        explicit ASequenceNode(NodeType::Enum nodeType,
                               const QSharedPointer<NodesSequence> &sequenceParent,
                               QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~ASequenceNode() override;

    public:
        /** @brief Say if the node can be targeted by an another node, none or multiple */
        virtual TargetType::Enum canItBeTargeted() { return TargetType::OnlyOne; }

        /** @brief Say if the node can target an another node, none or multiple  */
        virtual TargetType::Enum canTargetNode() { return TargetType::OnlyOne; }

        /** @brief Add a state transition to this state node
            @note If transition is NULL, the method will create a new one and add the transition to
                    the given state.
                  If transition is not NULL, it will set the target state to this state and add the
                    transition to the given state.
            @param state The state which targets
            @param callerNode The node which targets this node
            @param targetingKey The key only allowes authorized class instance to call this method
            @param transition The transition to this node state
            @return The transition created or modified, NULL if a problem occurred */
        virtual QAbstractTransition *addTransitionToThisNode(
                                                    QState &state,
                                                    ASequenceNode &callerNode,
                                                    const TargetingSequenceNodesKey &targetingKey,
                                                    QAbstractTransition *transition = Q_NULLPTR);

    public:
        /** @brief To call when the transition to this node is removed, this method will change
                   its parent if necessary
            @param callerNode The node which targets this node
            @param targetingKey The key only allowes authorized class instance to call this method
            @return True if no problem occurs */
        bool removeTransitionWithThisNode(ASequenceNode &callerNode,
                                          const TargetingSequenceNodesKey &targetingKey);

        /** @brief If the parent state given is different to the node parent state, call the
                   @see ASequenceNode::changeParentStateWithWaterfallEffect
            @param parentStateToCompareWith The parent state to compare with, in order to know if
                                            it's necessary to modify the node parent state
            @param callerNode The node which targets this node
            @param modifiedTargetedState Set to this pointer reference the new parent created in the
                                         method
            @param targetingKey The key only allowes authorized class instance to call this method
            @return True if no problem occurs */
        bool changeParentStateIfDifferentWithWaterfallEffect(
                                                QState *parentStateToCompareWith,
                                                ASequenceNode &callerNode,
                                                QPointer<QAbstractState> &modifiedTargetedState,
                                                const TargetingSequenceNodesKey &targetingKey);

        /** @brief Say if the node is currently active */
        bool isActive() const { return _isActive; }

    protected slots:
        /** @brief This method is called when the node is active
            @warning In order to go to the next node, it's necessary to emit the exit signal
            @note If a problem occurred the error signal can be emitted  instead of the exit
                  signal */
        virtual void nodeActivity();

    protected:
        /** @brief Get the state linked to the node */
        QAbstractState *getState() const { return _state; }

        /** @brief This method helps to test at runtime the sequence state, and if it's necessary
                   to stop or pause the sequence
            @return True if no error occurs and no stop has been asked
                    False if an error occurs or if a stop has been asked */
        bool testStoppingAndPausingAtRunTime();

        /** @brief Helpful method to emit the exit signal and manage the active status
            @note Do not use the @ref exit signal directly */
        void exitProcess();

        /** @brief Helpful method to emit the error signal and manage the active status
            @note Do not use the @ref error signal directly */
        void errorProcess(const ANode &guiltyNode, const QString &errorTxt);

    protected:
        /** @brief Change the parent of the current node state
            @note Because it's impossible to set the parent of a living state, it's necessary to
                  remove it and create a new one with the state given as parent. This modification
                  has to be applyied to all the nodes targeted by this one (if there are some)
            @param parentState The parent state
            @param callerNode The node which targets this node
            @param modifiedTargetedState Set to this pointer reference the new parent created in the
                                         method
            @return True if no problem occurs */
        virtual bool changeParentStateWithWaterfallEffect(
                                            QState *parentState,
                                            ASequenceNode &callerNode,
                                            QPointer<QAbstractState> &modifiedTargetedState) = 0;

    protected:
        /** @brief Set the state linked to the node
            @note Set the state parent to this node
            @note If the object has already a state, the method will delete it
            @param state The state to set */
        virtual void setState(QAbstractState *state);

        /** @see ASequenceNode::changeParentStateIfDifferentWithWaterfallEffect */
        virtual bool changeParentStateIfDifferentWithWaterfallEffectProtect(
                                                QState *parentStateToCompareWith,
                                                ASequenceNode &callerNode,
                                                QPointer<QAbstractState> &modifiedTargetedState);

        /** @see ASequenceNode::removeTransitionWithThisNode */
        virtual bool removeTransitionWithThisNodeProtect(ASequenceNode &callerNode);

    signals:
        /** @brief To emit in nodeActivity to trigger the node changement
            @attention Do not use this signal but the method @ref exitProcess */
        void exit();

        /** @brief To emit in nodeActivity to inform that an error occurred. This signal replace the
                   exit signal and it's the state machine which decide what it's necessary to do
            @attention Do not use this signal but the method @ref errorProcess
            @param guiltyNode The node which caused the error
            @param error The string description of the error */
        void error(const ANode &guiltyNode, const QString &error);

    private:
        QAbstractState *_state{nullptr};
        bool _isActive{false};
};
