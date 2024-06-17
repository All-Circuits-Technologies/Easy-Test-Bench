// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "checksequencevalidity.hpp"

#include <QDebug>

#include "corestatemachine/innodeport.hpp"
#include "corestatemachine/outnodeport.hpp"
#include "sequence/nodessequence.hpp"
#include "sequence/nodes/atargetingnode.hpp"
#include "sequence/nodes/decisionalnode.hpp"
#include "sequence/nodes/forknode.hpp"
#include "sequence/nodes/groupnode.hpp"
#include "sequence/nodes/inclusionnode.hpp"
#include "sequence/nodes/joinnode.hpp"
#include "sequence/nodes/sequencemodulenode.hpp"
#include "sequence/referencenodes/areferencenode.hpp"
#include "sequence/referencenodes/variablenode.hpp"
#include "sequence/sequencenodeshelper.hpp"
#include "testbedcore/testbedglobal.hpp"
#include "testbedcore/parsers/commonjsonparser.hpp"


bool CheckSequenceValidity::isValid(const QSharedPointer<NodesSequence> &nodeSequence,
                                    QString &error,
                                    QSharedPointer<ANode> &nodeWithError)
{
    // If the following tests failed, we consider that's the fault of the node sequence given
    nodeWithError = nodeSequence;

    if(nodeSequence.isNull())
    {
        MANAGE_LOG_ERR(error, "The node sequence given is null");
        return false;
    }

    if(nodeSequence->getStateMachine() == nullptr)
    {
        MANAGE_LOG_ERR(error,
                       "The state machine of the node sequence: " +
                       nodeSequence->getInstanceName()  + ", is null");
        return false;
    }

    RETURN_IF_FALSE(isValid(nodeSequence->accessReferencesHolder(), nodeSequence, error));

    QSharedPointer<ASequenceNode> initNodeFound;
    QSharedPointer<ASequenceNode> finalNodeFound;

    QVector<QString> nodesName = nodeSequence->getSequenceNodesName();
    for(auto citer = nodesName.cbegin(); citer != nodesName.cend(); ++citer)
    {
        QSharedPointer<ASequenceNode> node = nodeSequence->getSequenceNode(*citer);

        if(node.isNull())
        {
            MANAGE_LOG_ERR(error, "The sequence node given: " + *citer + ", is null");
            return false;
        }

        nodeWithError = node;

        RETURN_IF_FALSE(isValid(node, error));

        if(node->getNodeType() == NodeType::InitialNode)
        {
            if(initNodeFound)
            {
                MANAGE_LOG_ERR(error,
                               "The sequence can't have two initial nodes: " +
                               initNodeFound->getInstanceName() + " and " +
                               node->getInstanceName());
                return false;
            }

            initNodeFound = node;
        }
        else if(node->getNodeType() == NodeType::FinalNode)
        {
            if(finalNodeFound)
            {
                MANAGE_LOG_ERR(error,
                               "The sequence can't have two final nodes: " +
                               finalNodeFound->getInstanceName() + " and " +
                               node->getInstanceName());
                return false;
            }

            finalNodeFound = node;
        }
    }

    if(initNodeFound.isNull())
    {
        MANAGE_LOG_ERR(error,
                       "The sequence needs to have an initial node");
        return false;
    }

    if(finalNodeFound.isNull())
    {
        MANAGE_LOG_ERR(error,
                       "The sequence needs to have a final node");
        return false;
    }

    nodeWithError.clear();
    return true;
}

bool CheckSequenceValidity::isValid(const QSharedPointer<ASequenceNode> &node,
                                    QString &error)
{
    RETURN_IF_FALSE(testIfLinkedWithOtherNodes(node, error));

    bool ok = true;

    // Test, if necessary, the particularity of a node
    switch(node->getNodeType())
    {
        case NodeType::InitialNode:
        case NodeType::FinalNode:
        case NodeType::MergeNode:
        case NodeType::ConstantNode:
        case NodeType::VariableNode:
        case NodeType::NodesSequence:
            // Nothing else to test here
            break;

        case NodeType::DecisionalNode:
            ok = isDecisionnalNodeValid(node.objectCast<DecisionalNode>(), error);
            break;

        case NodeType::ForkNode:
            ok = isForkNodeValid(node.objectCast<ForkNode>(), error);
            break;

        case NodeType::JoinNode:
            ok = isJoinNodeValid(node.objectCast<JoinNode>(), error);
            break;

        case NodeType::GroupNode:
            ok = isGroupNodeValid(node.objectCast<GroupNode>(), error);
            break;

        case NodeType::InclusionNode:
            ok = isInclusionNodeValid(node.objectCast<InclusionNode>(), error);
            break;

        case NodeType::SequenceModuleNode:
            ok = isSequenceModuleNodeValid(node.objectCast<SequenceModuleNode>(), error);
            break;

        case NodeType::UnknownNode:
            MANAGE_LOG_ERR(error,
                           "The node is an unknown node: " + node->getInstanceName());
            return false;
    }

    return ok;
}

bool CheckSequenceValidity::isValid(const ReferencesHolder &refHolder,
                                    const QSharedPointer<ANode> &linkedNode,
                                    QString &error)
{
    for(const InNodePort &input: refHolder.getInputs())
    {
        if(input.getInputNode().isNull() && !input.isReferenceCanBeUndefined())
        {
            MANAGE_LOG_ERR(error,
                           "The input node: " + input.getName() + " of the node: " +
                           linkedNode->getInstanceName() + ", has to be connected " +
                           "with a reference node.");
            return false;
        }

        if(!input.validateReference(input.getInputNode()))
        {
            MANAGE_LOG_ERR(error,
                           "The node port: " + input.getName() + ", of the node: " +
                           linkedNode->getInstanceName() + ", and the reference node: " +
                           input.getInputNode()->getInstanceName() + ", are incompatible");
            return false;
        }

        if(refHolder.getHolderType() == ReferencesHolderType::CanHaveInternalRefs)
        {
            if(input.getInternalInputNode().isNull() && !input.isReferenceCanBeUndefined())
            {
                MANAGE_LOG_ERR(error,
                               "The internal input node: " + input.getName() + " of the node: " +
                               linkedNode->getInstanceName() + ", has to be connected " +
                               "with a reference node.");
                return false;
            }

            if(!input.getInternalInputNode().isNull() && input.getInputNode().isNull())
            {
                MANAGE_LOG_ERR(error,
                               "If the internal input node is set, currently to: " +
                               input.getName() + " in the node: " +
                               linkedNode->getInstanceName() + ", the external ref has to be not " +
                               "null.");
                return false;
            }

            if(!input.validateReference(input.getInternalInputNode()))
            {
                MANAGE_LOG_ERR(error,
                               "The node port: " + input.getName() + ", of the node: " +
                               linkedNode->getInstanceName() + ", and the internal reference " +
                               "node: " + input.getInternalInputNode()->getInstanceName() + ", are " +
                               "incompatible");
                return false;
            }
        }
    }

    for(const OutNodePort &output: refHolder.getOutputs())
    {
        if(!output.validateReference(output.getOutputNode()))
        {
            MANAGE_LOG_ERR(error,
                           "The node port: " + output.getName() + ", of the node: " +
                           linkedNode->getInstanceName() + ", and the reference node: " +
                           output.getOutputNode()->getInstanceName() + ", are incompatible");
            return false;
        }

        if(refHolder.getHolderType() == ReferencesHolderType::CanHaveInternalRefs)
        {
            if(!output.getOutputNode().isNull()                 &&
               !isOutputNodeCanHaveNoInternalBinding(output)    &&
               output.getInternalOutputNode().isNull())
            {
                MANAGE_LOG_ERR(error,
                               "The internal node port: " + output.getName() + ", of the node: " +
                               linkedNode->getInstanceName() + ", is null");
                return false;
            }

            if(!output.validateReference(output.getInternalOutputNode()))
            {
                MANAGE_LOG_ERR(error,
                               "The node port: " + output.getName() + ", of the node: " +
                               linkedNode->getInstanceName() + ", and the reference node: " +
                               output.getInternalOutputNode()->getInstanceName() + ", are " +
                               "incompatible");
                return false;
            }
        }
    }

    return true;
}

bool CheckSequenceValidity::isOutputNodeCanHaveNoInternalBinding(const OutNodePort &output)
{
    return JsonParsersConstants::Sequence::AllowedOutputWithoutInternalBindings.contains(
                                                                                output.getName());
}

bool CheckSequenceValidity::testIfLinkedWithOtherNodes(const QSharedPointer<ASequenceNode> &node,
                                                       QString &error)
{
    if(node->getNodeType() == NodeType::ForkNode)
    {
        QSharedPointer<ForkNode> forkNode = node.objectCast<ForkNode>();

        if(forkNode->getLinkedNodes().isEmpty())
        {
            MANAGE_LOG_ERR(error,
                           "The fork node: " + node->getInstanceName() + ", doesn't have " +
                           "any parallel children path");
            return false;
        }
    }
    else if(node->getNodeType() == NodeType::DecisionalNode)
    {
        QSharedPointer<DecisionalNode> decisionalNode = node.objectCast<DecisionalNode>();

        if(decisionalNode->getLinkedNodes().isEmpty() && decisionalNode->getDefaultNode().isNull())
        {
            MANAGE_LOG_ERR(error,
                           "The fork node: " + node->getInstanceName() + ", doesn't have " +
                           "any parallel children path");
            return false;
        }
    }
    else
    {
        QSharedPointer<ATargetingNode> targetingNode = node.objectCast<ATargetingNode>();

        if(targetingNode && targetingNode->getNodeTargeted().isNull())
        {
            MANAGE_LOG_ERR(error,
                           "The node doesn't target any node: " + node->getInstanceName());
            return false;
        }
    }

    const QSharedPointer<NodesSequence> &parent = node->getInstanceParent();

    QSharedPointer<ASequenceNode> targetingNodeFound;

    // Look for all the other nodes, if there are some which are targeting this one
    QVector<QString> nodesName = parent->getSequenceNodesName();
    for(auto citer = nodesName.cbegin(); citer != nodesName.cend(); ++citer)
    {
        QSharedPointer<ASequenceNode> otherNode = parent->getSequenceNode(*citer);

        if(otherNode == node)
        {
            // Don't test itself, continue
            continue;
        }

        if(SequenceNodesHelper::isTargetingNode(node, otherNode))
        {
            // The other node is targeting this one

            if(node->canItBeTargeted() == TargetType::None)
            {
                MANAGE_LOG_ERR(error,
                               "The node: " + node->getInstanceName() + ", is targeted by " +
                               "the node: "+ otherNode->getInstanceName() + ", but it can't " +
                               "be targeted.");
                return false;
            }

            if(node->canItBeTargeted() == TargetType::OnlyOne)
            {
                if(targetingNodeFound)
                {
                    MANAGE_LOG_ERR(error,
                                   "The node: " + node->getInstanceName() + ", is targeted " +
                                   "by two nodes: " + otherNode->getInstanceName() + ", and " +
                                   targetingNodeFound->getInstanceName() + ", but it can't " +
                                   "be twice.");
                    return false;
                }

                targetingNodeFound = otherNode;
            }
            else if(node->canItBeTargeted() == TargetType::Multiple)
            {
                targetingNodeFound = otherNode;
                break;
            }
        }
    }

    if(!targetingNodeFound && node->canItBeTargeted() != TargetType::None)
    {
        // Test if there is at least one node which is targeting this node
        MANAGE_LOG_ERR(error,
                           "The node: " + node->getInstanceName() + ", isn't targeted by any " +
                           "node.");
        return false;
    }

    return true;
}

bool CheckSequenceValidity::isJoinNodeValid(const QSharedPointer<JoinNode> &node, QString &error)
{
    if(node->getForkNodeLinked().isNull())
    {
        MANAGE_LOG_ERR(error,
                       "The join node: " + node->getInstanceName() + ", has no fork node");
        return false;
    }

    return true;
}

bool CheckSequenceValidity::isForkNodeValid(const QSharedPointer<ForkNode> &node, QString &error)
{
    if(node->getJoinNodeLinked().isNull())
    {
        MANAGE_LOG_ERR(error,
                       "The fork node: " + node->getInstanceName() + ", has no join node");
        return false;
    }

    // Don't test the last element, because if we arrive at the last element without an error, it
    // means that it's unique
    for(int idx = 0; idx < (node->getLinkedNodes().length() - 1); idx++)
    {
        const QSharedPointer<ASequenceNode> &childNode = node->getLinkedNodes().at(idx);

        if(node->getLinkedNodes().indexOf(childNode, idx + 1) != -1)
        {
            MANAGE_LOG_ERR(error,
                           "The fork node: " + node->getInstanceName() + ", can't target several "
                           "times the same node: " + childNode->getInstanceName());
            return false;
        }
    }

    return true;
}

bool CheckSequenceValidity::isASequenceContainerNodeValid(
                                                const QSharedPointer<ASequenceContainerNode> &node,
                                                const QSharedPointer<NodesSequence> &childNodesSeq,
                                                QString &error)
{
    bool emitErrorIfSequenceFailed = node->isFailingIfAnErrorOccured();
    if(!emitErrorIfSequenceFailed)
    {
        bool errorOutputFound = false;

        for(const OutNodePort &out : childNodesSeq->getReferencesHolder().getOutputs())
        {
            if(out.getName() == JsonParsersConstants::Sequence::SequenceSuccessOutput)
            {
                if(out.getTypeManaged() != TypeManaged::Boolean)
                {
                    MANAGE_LOG_ERR(error,
                                   "Error with the output: " +
                                   QString(JsonParsersConstants::Sequence::SequenceSuccessOutput) +
                                   ", of the group node: " + node->getInstanceName() +
                                   ", it's not a boolean");
                    return false;
                }

                errorOutputFound = true;
            }
        }

        if(!errorOutputFound)
        {
            MANAGE_LOG_ERR(error,
                           "The output: " +
                           QString(JsonParsersConstants::Sequence::SequenceSuccessOutput) +
                           ", is compulsory in the group node: " + node->getInstanceName() +
                           ", because the attribute: " +
                           JsonParsersConstants::Sequence::EmitErrorIfSequenceFailedAttribute +
                           ", is equal to false");
            return false;
        }
    }

    return true;
}

bool CheckSequenceValidity::isGroupNodeValid(const QSharedPointer<GroupNode> &node, QString &error)
{
    const QSharedPointer<NodesSequence> &nodesSequence = node->getChildNodesSequence();

    if(nodesSequence.isNull())
    {
        MANAGE_LOG_ERR(error,
                       "The group node: " + node->getInstanceName() + ", has no linked nodes " +
                       "sequence");
        return false;
    }

    QSharedPointer<ANode> anode;
    RETURN_IF_FALSE(isValid(nodesSequence, error, anode));

    RETURN_IF_FALSE(isASequenceContainerNodeValid(node, nodesSequence, error));

    return true;
}

bool CheckSequenceValidity::isInclusionNodeValid(const QSharedPointer<InclusionNode> &node,
                                                 QString &error)
{
    const QSharedPointer<NodesSequence> &nodesSequence = node->getChildNodesSequence();

    if(!nodesSequence.isNull())
    {
        QSharedPointer<ANode> anode;
        RETURN_IF_FALSE(isValid(nodesSequence, error, anode));

        RETURN_IF_FALSE(isASequenceContainerNodeValid(node, nodesSequence, error));
    }

    return true;
}

bool CheckSequenceValidity::isSequenceModuleNodeValid(
                                                    const QSharedPointer<SequenceModuleNode> &node,
                                                    QString &error)
{
    RETURN_IF_FALSE(isValid(node->getReferencesHolder(), node, error));

    if(!node->getSequenceBankItf())
    {
        MANAGE_LOG_ERR(error,
                       "The sequence module node: " + node->getInstanceName() + ", has no " +
                       "bank module linked");
        return false;
    }

    return true;
}

bool CheckSequenceValidity::isDecisionnalNodeValid(const QSharedPointer<DecisionalNode> &node,
                                                   QString &error)
{
    if(node->getDefaultNode().isNull())
    {
        MANAGE_LOG_ERR(error,
                       "The decisional node: " + node->getInstanceName() + ", has to have a " +
                       "default node");
        return false;
    }

    JsonArray transitionElems;
    if(!node->getTransitions(transitionElems))
    {
        MANAGE_LOG_ERR(error,
                       "Can't get the transitions of the decisional node: " +
                       node->getInstanceName());
        return false;
    }

    JsonArray::const_iterator iter = transitionElems.cbegin();

    for(; iter != transitionElems.cend(); ++iter)
    {
        JsonObject transitionObject = iter->value<JsonObject>();
        RETURN_IF_FALSE(isDecisionalTransitionValid(transitionObject,
                                                    node->getInstanceName(),
                                                    error));
    }

    return true;
}

bool CheckSequenceValidity::isDecisionalTransitionValid(const JsonObject &transition,
                                                        const QString &decisionalName,
                                                        QString &error)
{
    QVariant transitionOpe = transition[JsonParsersConstants::Sequence::OperatorAttribute];

    QString strOperator;
    if(!CommonJsonParser::getValue(transitionOpe, strOperator))
    {
        MANAGE_LOG_ERR(error,
                       "Can't get the string operator: " + transitionOpe.toString() + ", of the " +
                       "decisional node: " + decisionalName);
        return false;
    }

    DecisionalOperator::Enum deciOperator = DecisionalOperator::parseFromString(strOperator);

    if(deciOperator == DecisionalOperator::Unknown)
    {
        MANAGE_LOG_ERR(error,
                       "Can't cast the string: " + strOperator + ", to decisional operator of " +
                       "the decisional node: " + decisionalName);
        return false;
    }

    if(deciOperator != DecisionalOperator::Or && deciOperator != DecisionalOperator::And)
    {
        // Nothing to test here
        return true;
    }

    JsonArray elements = transition[JsonParsersConstants::Sequence::ElementsNameAttribute]
                                                                                .value<JsonArray>();

    if(elements.length() < 2)
    {
        MANAGE_LOG_ERR(error,
                       "The transitional operator: " + strOperator + ", of the decisional node: " +
                       decisionalName + ", has less than 2 elements under it. Therefore, the " +
                       "operator has no sens to be here.");
        return false;
    }

    JsonArray::const_iterator iter = elements.cbegin();

    for(; iter != elements.cend(); ++iter)
    {
        JsonObject transitionObject = iter->value<JsonObject>();
        RETURN_IF_FALSE(isDecisionalTransitionValid(transitionObject, decisionalName, error));
    }

    return true;
}
