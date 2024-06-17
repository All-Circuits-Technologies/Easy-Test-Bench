// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "sequence/nodes/asequencenode.hpp"

#include <QHash>
#include <QVariant>

#include "jsonutility/jsontypedefs.hpp"
#include "sequence/decisionaltransition/decisionaloperator.hpp"
#include "testbedcore/types/stopaskedtype.hpp"

class ADecisionalTransition;
class ALogicalOperatorDecisionalTransition;
class AReferenceNode;
class SequenceLoadingErrorHandler;


/** @brief The decisional node is useful to represent a choice and its junctions
    @note The node test the value of a reference node with the value of transitions */
class DecisionalNode : public ASequenceNode
{
    Q_OBJECT

    public:
        /** @brief Class constuctor
            @param sequenceParent The NodesSequence parent of this node
            @param parent The class parent */
        explicit DecisionalNode(const QSharedPointer<NodesSequence> &sequenceParent,
                                QObject *parent = nullptr);

    public:
        /** @brief Set the reference node to test by the decisional node
            @param refNodeToTest The reference node to test */
        void setReferenceNodeToTest(const QSharedPointer<AReferenceNode> &refNodeToTest);

        /** @brief Get the reference node tested by the decisional node */
        const QSharedPointer<AReferenceNode> &getReferenceNodeToTest() const
        { return _refNodeToTest; }

        /** @brief Set the default node
            @note While testing, if no transitions are triggered, it will redirect to  the default
                  node.
            @note A node already targeted by a transition can be the default node
            @param defaultNode The node to target if no others are chosen
            @return True if no problem occurs */
        bool setDefaultNode(const QSharedPointer<ASequenceNode> &defaultNode);

        /** @brief Get the default node */
        const QSharedPointer<ASequenceNode> &getDefaultNode() const { return _defaultNode; }

        /** @brief Add transition to the node
            @param targetedNode The node to target
            @param transitionElem A JsonObject representation of the transition to add, see:
                                  @ref SequenceFileJsonParser::loadDecisionalTransition to have the
                                  details of what the object contains
            @param errorHandler An optional sequence loading error handler pointer
            @return True if no problem occurs */
        bool addTransitionToNode(const QSharedPointer<ASequenceNode> &targetedNode,
                                 const JsonObject &transitionElem,
                                 SequenceLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Get all the transitions of the node but formatted to JsonArray (which allow to
                   have something more abstract and easy to use)
            @note See @ref SequenceFileJsonParser::loadDecisionalTransition to have the details of
                  what the object contains
            @param transitionElems The transitions elements of the node
            @return True if no problem occurs */
        bool getTransitions(JsonArray &transitionElems) const;

        /** @brief Get all the nodes linked with this node via the transitions (except the default
                   one)
            @note This method doesn't the default node */
        QVector<QSharedPointer<ASequenceNode>> getLinkedNodes() const;

        /** @brief Remove one transition to a node previously targeted
            @param targetedNode The targeted to no more target
            @return True If a problem occurs */
        bool removeTransitionToNode(const QSharedPointer<ASequenceNode> &targetedNode);

    protected slots:
        /** @see ASequenceNode::nodeActivity */
        virtual void nodeActivity() override;

        /** @see ANode::changeParentStateWithWaterfallEffect */
        virtual void forgetAllAboutIt(const QSharedPointer<ANode> &nodeToDetachWith) override;

    private slots:
        /** @brief Called when one test has been done.
            @note If all tests have been done and none succeed, it triggers the transition to the
                  defaultNode
            @warning The signal emitted has to be coming from ADecisionalTransition object
            @param testResult The result of the test */
        void onTestResult(bool testResult);

        /** @brief Called when a stop has been asked for the sequence
            @param stopAskedType Give a contact to the stop asking */
        void onStopAskedInSequence(StopAskedType::Enum stopAskedType);

    protected:
        /** @see ASequenceNode::canTargetNode */
        virtual TargetType::Enum canTargetNode() override { return TargetType::Multiple; }

        /** @see ASequenceNode::changeParentStateWithWaterfallEffect */
        virtual bool changeParentStateWithWaterfallEffect(
                                        QState *parentState,
                                        ASequenceNode &callerNode,
                                        QPointer<QAbstractState> &modifiedTargetedState) override;

    private:
        /** @brief Cast the abstract state of the ASequenceNode to a QState
            @note Because the state is created in the class constructor with a QState,
                  this might return a non NULL pointer.
            @return The state linked to this node or NULL if there is no state or if the state is
                    not a QState */
        QState *getCastState() const;

        /** @brief Reset the result of all the transitions tests
            @attention Has to be called before triggering any transitions */
        void resetTransitions();

    private:
        /** @brief Help to create a transition
            @param transition A JsonObject representation of the transition to add, see:
                              @ref SequenceFileJsonParser::loadDecisionalTransition to have the
                              details of what the object contains
            @param parentState The state source of the transition to create
            @return The decisional transition created or NULL if a problem occurred */
        static ADecisionalTransition *createTransition(const JsonObject &transition,
                                                       QState *parentState = nullptr);

        /** @brief Help to create a transition, which is not an AND or an OR operator
            @param deciOperator The transition operator
            @param value The value to compare with
            @param parentState The state source of the transition to create
            @return The decisional transition created or NULL if a problem occurred */
        static ADecisionalTransition *createTransition(DecisionalOperator::Enum deciOperator,
                                                       const QVariant &value,
                                                       QState *parentState = nullptr);

        /** @brief Help to create a logical transition, which is an AND or an OR operator
            @param deciOperator The logical transition operator
            @param parentState The state source of the transition to create
            @return The logical decisional transition created or NULL if a problem occurred */
        static ALogicalOperatorDecisionalTransition *createLogicTransition(
                                                            DecisionalOperator::Enum deciOperator,
                                                            QState *parentState = nullptr);

        /** @brief Get a JsonObject representation of the transition
            @param transition The transition to parse to JsonObject
            @param jsonObject The jsonObject parsed, see:
                              @ref SequenceFileJsonParser::loadDecisionalTransition to have the
                              details of what the object contains
            @return True if no problem occurred */
        static bool getJsonObject(const ADecisionalTransition *transition, JsonObject &jsonObject);

    private:
        /** @brief Decisional transition test result */
        enum TestResult
        {
            NotPerformed,
            Success,
            Failed
        };

        /** @brief Contains a decisional transition and its current result */
        struct TransitionInfos
        {
            TestResult testResult;
            ADecisionalTransition *transition;
        };

    private:
        QSharedPointer<AReferenceNode> _refNodeToTest;

        QSharedPointer<ASequenceNode> _defaultNode;
        QAbstractTransition *_defaultTransition{nullptr};

        QHash<QSharedPointer<ASequenceNode>, TransitionInfos> _transitions;
};
