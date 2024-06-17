// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "decisionalnode.hpp"

#include <QCoreApplication>
#include <QDebug>
#include <QState>
#include <QStateMachine>

#include "collectionutility/qhashhelper.hpp"
#include "errors/helpers/fileloadingerrorcausehelper.hpp"
#include "sequence/accesskeys/asequencenodekey.hpp"
#include "sequence/accesskeys/targetingsequencenodeskey.hpp"
#include "sequence/decisionaltransition/adecisionaltransition.hpp"
#include "sequence/decisionaltransition/anddecisionaltransition.hpp"
#include "sequence/decisionaltransition/decisionaleventvalue.hpp"
#include "sequence/decisionaltransition/equalitydecisionaltransition.hpp"
#include "sequence/decisionaltransition/greaterthandecisionaltransition.hpp"
#include "sequence/decisionaltransition/lesserthandecisionaltransition.hpp"
#include "sequence/decisionaltransition/ordecisionaltransition.hpp"
#include "sequence/nodessequence.hpp"
#include "sequence/referencenodes/areferencenode.hpp"
#include "sequence/sequencenodeshelper.hpp"
#include "testbedcore/parsers/jsonparsersconstants.hpp"
#include "testbedcore/testbedglobal.hpp"


DecisionalNode::DecisionalNode(const QSharedPointer<NodesSequence> &sequenceParent,
                               QObject *parent) :
    ASequenceNode(NodeType::DecisionalNode, sequenceParent, parent)
{
    setState(new QState(sequenceParent->accessStateMachine({}).data()));

    connect(&sequenceParent->getSequenceState(), &SequenceStateHelper::stopAsked,
            this,                                &DecisionalNode::onStopAskedInSequence);
}

void DecisionalNode::setReferenceNodeToTest(const QSharedPointer<AReferenceNode> &refNodeToTest)
{
    _refNodeToTest = refNodeToTest;
}

bool DecisionalNode::setDefaultNode(const QSharedPointer<ASequenceNode> &defaultNode)
{
    if(!defaultNode.isNull() &&
       !SequenceNodesHelper::verifyIfItsValidToTargetThisNode(defaultNode))
    {
        qWarning() << "The node: " << defaultNode->getInstanceName() << " can't be targeted";
        return false;
    }

    QState *state = getCastState();

    if(!state)
    {
        qWarning() << "Can only add a transition to a QState";
        return false;
    }

    if(_defaultTransition && _defaultNode)
    {
        RETURN_IF_FALSE(_defaultNode->removeTransitionWithThisNode(*this, {}));

        if(_defaultTransition->targetState() != nullptr)
        {
            state->removeTransition(_defaultTransition);
        }

        delete _defaultTransition;
        _defaultTransition = nullptr;

        _defaultNode.clear();
    }

    if(defaultNode)
    {
        _defaultTransition = defaultNode->addTransitionToThisNode(*state, *this, {});

        if(_defaultTransition == nullptr)
        {
            return false;
        }
    }

    _defaultNode = defaultNode;

    return true;
}

bool DecisionalNode::addTransitionToNode(const QSharedPointer<ASequenceNode> &targetedNode,
                                         const JsonObject &transitionElem,
                                         SequenceLoadingErrorHandler *errorHandler)
{
    if(targetedNode.isNull())
    {
        FileLoadingErrorCauseHelper::addNodeCannotBeTargetedError(errorHandler);
        qWarning() << "Can't add transition to a NULL node";
        return false;
    }

    if(!targetedNode.isNull() &&
       !SequenceNodesHelper::verifyIfItsValidToTargetThisNode(targetedNode))
    {
        FileLoadingErrorCauseHelper::addUnavailableTargetedNodeError(
                                                                    targetedNode->getInstanceName(),
                                                                    errorHandler);
        qWarning() << "The node: " << targetedNode->getInstanceName() << " can't be targeted";
        return false;
    }

    QState *state = getCastState();

    if(!state)
    {
        FileLoadingErrorCauseHelper::addInvalidTransitionError(errorHandler);
        qWarning() << "Can only add a transition to a QState";
        return false;
    }

    if(_transitions.contains(targetedNode) && !removeTransitionToNode(targetedNode))
    {
        FileLoadingErrorCauseHelper::addInvalidTransitionError(errorHandler);
        return false;
    }

    ADecisionalTransition *decisionalTransition = createTransition(transitionElem, state);

    if(decisionalTransition == nullptr)
    {
        FileLoadingErrorCauseHelper::addInvalidTransitionError(errorHandler);
        return false;
    }

    TransitionInfos transitionInfos = { TestResult::NotPerformed, decisionalTransition };

    if(!targetedNode->addTransitionToThisNode(*state, *this, {}, decisionalTransition))
    {
        FileLoadingErrorCauseHelper::addInvalidTransitionError(errorHandler);
        delete decisionalTransition;
        return false;
    }

    connect(decisionalTransition, &ADecisionalTransition::testResult,
            this,                 &DecisionalNode::onTestResult);

    _transitions.insert(targetedNode, transitionInfos);

    return true;
}

bool DecisionalNode::getTransitions(JsonArray &transitionElems) const
{
    QHash<QSharedPointer<ASequenceNode>, TransitionInfos>::const_iterator iter =
                                                                            _transitions.cbegin();

    for(;iter != _transitions.cend(); ++iter)
    {
        const ADecisionalTransition *transition = iter.value().transition;

        JsonObject jsonObject;
        RETURN_IF_FALSE(getJsonObject(transition, jsonObject));

        jsonObject[JsonParsersConstants::Sequence::ToNodeNameAttribute] =
                                                                    iter.key()->getInstanceName();

        transitionElems.append(QVariant::fromValue(jsonObject));
    }

    return true;
}

bool DecisionalNode::getJsonObject(const ADecisionalTransition *transition, JsonObject &jsonObject)
{
    if(!transition)
    {
        qWarning() << "The transition can't be empty, if you want to get the jsonObject";
        return false;
    }

    jsonObject[JsonParsersConstants::Sequence::OperatorAttribute] =
                                            DecisionalOperator::toString(transition->getOperator());

    if(transition->isLogicalOperator())
    {
        const ALogicalOperatorDecisionalTransition *logical =
                            qobject_cast<const ALogicalOperatorDecisionalTransition *>(transition);

        if(!logical)
        {
            qWarning() << "The element: " << transition->getOperator() << ", is not a logical "
                       << "operator";
            return false;
        }

        JsonArray elements;

        for(const ADecisionalTransition *deciTrans : logical->getTransitions())
        {
            JsonObject element;
            RETURN_IF_FALSE(getJsonObject(deciTrans, element));
            elements.append(QVariant::fromValue(element));
        }

        jsonObject[JsonParsersConstants::Sequence::ElementsNameAttribute] =
                                                                    QVariant::fromValue(elements);
    }
    else
    {
        jsonObject[JsonParsersConstants::Sequence::ValueNameAttribute] =
                                           QVariant::fromValue(transition->getValueToCompareWith());
    }

    return true;
}

QVector<QSharedPointer<ASequenceNode>> DecisionalNode::getLinkedNodes() const
{
    return QHashHelper::getKeys(_transitions);
}

bool DecisionalNode::removeTransitionToNode(const QSharedPointer<ASequenceNode> &targetedNode)
{
    if(!_transitions.contains(targetedNode))
    {
        // No transition to remove
        return true;
    }

    QState *state = getCastState();

    if(!state)
    {
        qWarning() << "Can only manage transitions with QState";
        return false;
    }

    const TransitionInfos infos = _transitions.value(targetedNode);

    RETURN_IF_FALSE(targetedNode->removeTransitionWithThisNode(*this, {}));

    if(infos.transition->targetState() != nullptr)
    {
        state->removeTransition(infos.transition);
    }

    delete infos.transition;

    _transitions.remove(targetedNode);

    return true;
}

void DecisionalNode::nodeActivity()
{
    ASequenceNode::nodeActivity();

    qDebug() << "Enter decisional node activity: " << getInstanceName()
             << ", value: " << _refNodeToTest->getValue();

    if(!testStoppingAndPausingAtRunTime())
    {
        return;
    }

    resetTransitions();

    QAbstractState *state = getState();

    if(!state)
    {
        qWarning() << "A state has to be linked with the ndoe";
        errorProcess(*this, "The node has no state attached to it.");
        return;
    }

    DecisionalEventValue *eventValue = new DecisionalEventValue(_refNodeToTest->getValue());

    // Send the event to the transition in order to test the value
    state->machine()->postEvent(eventValue);
}

void DecisionalNode::forgetAllAboutIt(const QSharedPointer<ANode> &nodeToDetachWith)
{
    ASequenceNode::forgetAllAboutIt(nodeToDetachWith);

    QSharedPointer<ASequenceNode> sequenceNode = nodeToDetachWith.objectCast<ASequenceNode>();

    if(!sequenceNode.isNull())
    {
        if(sequenceNode == _defaultNode)
        {
            setDefaultNode(QSharedPointer<ASequenceNode>());
        }

        if(_transitions.contains(sequenceNode))
        {
            removeTransitionToNode(sequenceNode);
        }
    }
    else if(_refNodeToTest == nodeToDetachWith)
    {
        _refNodeToTest.clear();
    }
}

void DecisionalNode::onTestResult(bool testResult)
{
    if(!testStoppingAndPausingAtRunTime())
    {
        errorProcess(*this, "Stop has been asked");
        return;
    }

    ADecisionalTransition *transition = qobject_cast<ADecisionalTransition*>(sender());

    if(!transition)
    {
        // The wrong element called the function
        return;
    }

    TestResult mostImportantResult = Failed;

    QHash<QSharedPointer<ASequenceNode>, TransitionInfos>::iterator iter = _transitions.begin();

    for(; iter != _transitions.end(); ++iter)
    {
        TransitionInfos &infos = iter.value();
        if(infos.transition == transition)
        {
            if(testResult)
            {
                // The element is successful, it's useless to test anything else, because the state
                // will end
                infos.testResult = Success;
                return;
            }

            infos.testResult = Failed;
        }

        if(mostImportantResult == Failed && infos.testResult != Failed)
        {
            mostImportantResult = infos.testResult;
        }
    }

    if(mostImportantResult == Failed)
    {
        // Every tests has failed, so fired the default condition
        if(!_defaultTransition)
        {
            qWarning() << "No default transition has been set";
            errorProcess(*this, "No default transition has been set for the node");
            return;
        }

        exitProcess();
    }
}

void DecisionalNode::onStopAskedInSequence(StopAskedType::Enum /*stopAskedType*/)
{
    errorProcess(*this, "Stop has been asked");
}

bool DecisionalNode::changeParentStateWithWaterfallEffect(
                                                    QState *parentState,
                                                    ASequenceNode &callerNode,
                                                    QPointer<QAbstractState> &modifiedTargetedState)
{
    Q_UNUSED(callerNode)

    QState *state = getCastState();

    if(!state)
    {
        qWarning() << "Can only manage transitions with QState";
        return false;
    }

    QState *newState = new QState(parentState);

    if(_defaultNode && _defaultTransition->targetState() != nullptr)
    {
        state->removeTransition(_defaultTransition);
    }

    QHash<QSharedPointer<ASequenceNode>, TransitionInfos>::const_iterator citer =
                                                                            _transitions.cbegin();

    for(; citer != _transitions.cend(); ++citer)
    {
        if(citer.value().transition->targetState() != nullptr)
        {
            state->removeTransition(citer.value().transition);
        }
    }

    setState(newState);

    modifiedTargetedState = newState;

    if(_defaultNode)
    {
        QPointer<QAbstractState> targetState;
        RETURN_IF_FALSE(_defaultNode->changeParentStateIfDifferentWithWaterfallEffect(parentState,
                                                                                      *this,
                                                                                      targetState,
                                                                                      {}));

        _defaultTransition->setTargetState(targetState);
        newState->addTransition(_defaultTransition);
    }

    QHash<QSharedPointer<ASequenceNode>, TransitionInfos>::iterator iter = _transitions.begin();

    for(;iter != _transitions.end(); ++iter)
    {
        QPointer<QAbstractState> targetState;
        RETURN_IF_FALSE(iter.key()->changeParentStateIfDifferentWithWaterfallEffect(parentState,
                                                                                    *this,
                                                                                    targetState,
                                                                                    {}));

        iter.value().transition->setTargetState(targetState);
        newState->addTransition(iter.value().transition);
    }

    return true;
}

QState *DecisionalNode::getCastState() const
{
    return qobject_cast<QState*>(getState());
}

void DecisionalNode::resetTransitions()
{
    QHash<QSharedPointer<ASequenceNode>, TransitionInfos>::iterator iter = _transitions.begin();

    for(;iter != _transitions.end(); ++iter)
    {
        iter.value().testResult = NotPerformed;
    }
}

ADecisionalTransition *DecisionalNode::createTransition(const JsonObject &transition,
                                                        QState *parentState)
{
    bool ok = false;
    int deciOpeRaw = transition.value(JsonParsersConstants::Sequence::OperatorAttribute).toInt(&ok);

    if(!ok)
    {
        qWarning() << "A problem occurred when trying to convert: " << deciOpeRaw << ", to int";
        return nullptr;
    }

    DecisionalOperator::Enum deciOpe = DecisionalOperator::parseFromInt(deciOpeRaw);

    if(deciOpe == DecisionalOperator::Unknown)
    {
        qWarning() << "Can't parse the operator given to decisional operator type: " << deciOpeRaw;
        return nullptr;
    }

    if(deciOpe == DecisionalOperator::Or || deciOpe == DecisionalOperator::And)
    {
        ALogicalOperatorDecisionalTransition *logical = createLogicTransition(deciOpe, parentState);

        if(!logical)
        {
            qWarning() << "A problem occurred when trying to parse the operator between";
            return nullptr;
        }

        JsonArray elements = transition.value(
                          JsonParsersConstants::Sequence::ElementsNameAttribute).value<JsonArray>();

        QVector<ADecisionalTransition *> deciTransitions;

        for(const QVariant &varChildrenElement : elements)
        {
            JsonObject childElem = varChildrenElement.value<JsonObject>();

            ADecisionalTransition *childTrans = createTransition(childElem, parentState);

            if(!childTrans)
            {
                qWarning() << "The child element can't be created";
                delete logical;
                qDeleteAll(deciTransitions);
                return nullptr;
            }

            deciTransitions.append(childTrans);
        }

        if(!logical->setTransitions(deciTransitions))
        {
            delete logical;
            qDeleteAll(deciTransitions);
            return nullptr;
        }

        return logical;
    }

    ADecisionalTransition *decisionalTransition = createTransition(
                            deciOpe,
                            transition.value(JsonParsersConstants::Sequence::ValueNameAttribute),
                            parentState);

    if(decisionalTransition == nullptr)
    {
        qWarning() << "The operator can't be managed";
        return nullptr;
    }

    return decisionalTransition;
}

ADecisionalTransition *DecisionalNode::createTransition(DecisionalOperator::Enum deciOperator,
                                                        const QVariant &value,
                                                        QState *parentState)
{
    ADecisionalTransition *decisionalTransition = nullptr;

    switch(deciOperator)
    {
        case DecisionalOperator::EqualTo:
        case DecisionalOperator::NotEqualTo:
            decisionalTransition = new EqualityDecisionalTransition(
                                                    (deciOperator == DecisionalOperator::EqualTo),
                                                    parentState);
            break;

        case DecisionalOperator::GreaterThan:
        case DecisionalOperator::GreaterOrEqualTo:
            decisionalTransition = new GreaterThanDecisionalTransition(
                                            (deciOperator == DecisionalOperator::GreaterOrEqualTo),
                                            parentState);
            break;

        case DecisionalOperator::LesserThan:
        case DecisionalOperator::LesserOrEqualTo:
            decisionalTransition = new LesserThanDecisionalTransition(
                                             (deciOperator == DecisionalOperator::LesserOrEqualTo),
                                             parentState);
            break;

        case DecisionalOperator::Or:
        case DecisionalOperator::And:
        case DecisionalOperator::Unknown:
            return nullptr;
    }

    if(!decisionalTransition->setValueToCompareWith(value))
    {
        delete decisionalTransition;
        return nullptr;
    }

    return decisionalTransition;
}

ALogicalOperatorDecisionalTransition *DecisionalNode::createLogicTransition(
                                                            DecisionalOperator::Enum deciOperator,
                                                            QState *parentState)
{
    ALogicalOperatorDecisionalTransition *decisionalTransition = nullptr;

    switch(deciOperator)
    {
        case DecisionalOperator::Or:
            decisionalTransition = new OrDecisionalTransition(parentState);
            break;

        case DecisionalOperator::And:
            decisionalTransition = new AndDecisionalTransition(parentState);
            break;

        case DecisionalOperator::LesserThan:
        case DecisionalOperator::LesserOrEqualTo:
        case DecisionalOperator::GreaterThan:
        case DecisionalOperator::GreaterOrEqualTo:
        case DecisionalOperator::EqualTo:
        case DecisionalOperator::NotEqualTo:
        case DecisionalOperator::Unknown:
            return nullptr;
    }

    return decisionalTransition;
}
