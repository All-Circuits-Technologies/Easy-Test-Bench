// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "nodestojsonparser.hpp"

#include <QDebug>
#include <QFile>
#include <QFileInfo>

#include "bankcoreshared/usebylib/ibankinterface.hpp"
#include "bankcoreshared/usebylib/isequenceinstancemodule.hpp"
#include "sequence/nodes/decisionalnode.hpp"
#include "sequence/nodes/finalnode.hpp"
#include "sequence/nodes/forknode.hpp"
#include "sequence/nodes/groupnode.hpp"
#include "sequence/nodes/inclusionnode.hpp"
#include "sequence/nodes/initialnode.hpp"
#include "sequence/nodes/joinnode.hpp"
#include "sequence/nodes/mergenode.hpp"
#include "sequence/nodes/sequencemodulenode.hpp"
#include "sequence/nodessequence.hpp"
#include "sequence/referencenodes/constantnode.hpp"
#include "sequence/referencenodes/variablenode.hpp"
#include "sequence/sequencefile.hpp"
#include "testbedcore/parsers/commonjsonparser.hpp"
#include "testbedcore/parsers/jsonparsersconstants.hpp"
#include "testbedcore/testbedglobal.hpp"
#include "testbedcore/types/unit.hpp"


bool NodesToJsonParser::sequenceFileToJsonArray(const SequenceFile &sequenceFile,
                                                JsonArray &jsonArray)
{
    if(!sequenceFile.isSequenceDataLoaded())
    {
        qWarning() << "The sequence file data aren't loaded";
        return false;
    }

    QSharedPointer<const NodesSequence> nodesSequence = sequenceFile.findFirstLoadedNodesSequence();

    if(!nodesSequence)
    {
        qWarning() << "There is no NodesSequence loaded can't proceed";
        return false;
    }

    return loadNodesSequenceAndChildrenToJsonObject(*nodesSequence, jsonArray);
}

bool NodesToJsonParser::nodesSequenceToJsonObject(const NodesSequence &nodesSequence,
                                                  JsonObject &jsonObject)
{
    RETURN_IF_FALSE(aNodeToJsonObject(nodesSequence, jsonObject));

    RETURN_IF_FALSE(referencesHolderToJsonObject(nodesSequence.getReferencesHolder(), jsonObject));

    return true;
}

bool NodesToJsonParser::initialNodeToJsonObject(const InitialNode &initialNode,
                                                JsonObject &jsonObject)
{
    RETURN_IF_FALSE(targetingNodeToJsonObject(initialNode, jsonObject));

    return true;
}

bool NodesToJsonParser::joinNodeToJsonObject(const JoinNode &joinNode, JsonObject &jsonObject)
{
    RETURN_IF_FALSE(targetingNodeToJsonObject(joinNode, jsonObject));

    const QSharedPointer<ForkNode> &forkNode = joinNode.getForkNodeLinked();

    if(!forkNode)
    {
        qWarning() << "The join node: " << joinNode.getInstanceName() << ", has no fork node "
                   << "linked";
        return false;
    }
    
    jsonObject[JsonParsersConstants::Sequence::ForkNodeInstanceNameAttribute] =
                                                                        forkNode->getInstanceName();

    return true;
}

bool NodesToJsonParser::mergeNodeToJsonObject(const MergeNode &mergeNode, JsonObject &jsonObject)
{
    RETURN_IF_FALSE(targetingNodeToJsonObject(mergeNode, jsonObject));

    return true;
}

bool NodesToJsonParser::finalNodeToJsonObject(const FinalNode &finalNode, JsonObject &jsonObject)
{
    RETURN_IF_FALSE(aNodeToJsonObject(finalNode, jsonObject));

    return true;
}

bool NodesToJsonParser::sequenceModuleNodeToJsonObject(const SequenceModuleNode &sequenceModuleNode,
                                                       JsonObject &jsonObject)
{
    RETURN_IF_FALSE(targetingNodeToJsonObject(sequenceModuleNode, jsonObject));

    RETURN_IF_FALSE(referencesHolderToJsonObject(sequenceModuleNode.getReferencesHolder(),
                                                 jsonObject));

    jsonObject[JsonParsersConstants::Generic::DisableAttributeName] =
                                                                    !sequenceModuleNode.isEnable();

    const ISequenceInstanceModule *seqItf = sequenceModuleNode.getSequenceBankItf();

    if(!seqItf)
    {
        qWarning() << "The sequence bank interface of the node: "
                   << sequenceModuleNode.getInstanceName() << ", can't be null";
        return false;
    }

    if(!seqItf->getBankInterface())
    {
        qWarning() << "The sequence interface of the node: " << sequenceModuleNode.getInstanceName()
                   << ", must be linked with a bank instance";
        return false;
    }

    QString bankName;

    if(!seqItf->getBankInterface()->getUniqueName(bankName))
    {
        qWarning() << "Can't get the name of the bank linked to the sequence: "
                   << sequenceModuleNode.getInstanceName();
        return false;
    }

    jsonObject[JsonParsersConstants::Sequence::BankNameAttribute] = bankName;

    QString moduleName;

    if(!seqItf->getModuleName(moduleName))
    {
        qWarning() << "Can't get the name of the module linked to the sequence node: "
                   << sequenceModuleNode.getInstanceName();
        return false;
    }

    jsonObject[JsonParsersConstants::Sequence::ModuleNameAttribute] = moduleName;

    JsonArray parameters;

    if(!seqItf->getModuleParameters(parameters))
    {
        qWarning() << "Can't get the parameters of the module linked to the sequence node: "
                   << sequenceModuleNode.getInstanceName();
        return false;
    }

    jsonObject[JsonParsersConstants::Sequence::ModuleParametersNameAttribute] =
                                                                    QVariant::fromValue(parameters);

    return true;
}

bool NodesToJsonParser::sequenceModuleInputValuesToJsonArray(
                                                    const SequenceModuleNode &sequenceModuleNode,
                                                    JsonArray &jsonArray)
{
    for(const InNodePort &inNodePort : sequenceModuleNode.getReferencesHolder().getInputs())
    {
        const QSharedPointer<AReferenceNode> &refNode = inNodePort.getInputNode();

        if(refNode.isNull())
        {
            if(inNodePort.isReferenceCanBeUndefined())
            {
                // Can be undefined and it is, so we continue
                continue;
            }

            qWarning() << "A reference node has to be given to the node port: "
                       << inNodePort.getName() << ", of the module: "
                       << sequenceModuleNode.getInstanceName();
            return false;
        }

        JsonObject inputObj;
        
        inputObj[JsonParsersConstants::Generic::NameAttribute] = inNodePort.getName();

        QString valueKey = CommonJsonParser::getValueAttributeName(refNode->getTypeManaged());

        inputObj[valueKey] = refNode->getValue();

        jsonArray.append(QVariant::fromValue(inputObj));
    }

    return true;
}

bool NodesToJsonParser::variableNodeToJsonObject(const VariableNode &variableNode,
                                                 JsonObject &jsonObject)
{
    RETURN_IF_FALSE(aReferenceNodeToJsonObject(variableNode, jsonObject));

    return true;
}

bool NodesToJsonParser::constantNodeToJsonObject(const ConstantNode &constantNode,
                                                 JsonObject &jsonObject)
{
    RETURN_IF_FALSE(aReferenceNodeToJsonObject(constantNode, jsonObject));

    TypeManaged::Enum typeManaged = constantNode.getTypeManaged();

    QString valueKey = CommonJsonParser::getValueAttributeName(typeManaged);

    jsonObject[valueKey] = constantNode.getValue();

    return true;
}

bool NodesToJsonParser::forkNodeToJsonObject(const ForkNode &forkNode, JsonObject &jsonObject)
{
    RETURN_IF_FALSE(aNodeToJsonObject(forkNode, jsonObject));

    QString joinNode;

    if(forkNode.getJoinNodeLinked())
    {
        joinNode = forkNode.getJoinNodeLinked()->getInstanceName();
    }
    
    jsonObject[JsonParsersConstants::Sequence::JoinNodeInstanceNameAttribute] = joinNode;

    JsonArray toNodes;

    QVector<QSharedPointer<ASequenceNode>> linkedNodes = forkNode.getLinkedNodes();
    for(auto citer = linkedNodes.cbegin(); citer != linkedNodes.cend(); ++citer)
    {
        if(*citer)
        {
            toNodes.append((*citer)->getInstanceName());
        }
    }

    jsonObject[JsonParsersConstants::Sequence::ToNodesNameAttribute] = QVariant::fromValue(toNodes);

    return true;
}

bool NodesToJsonParser::decisionalNodeToJsonObject(const DecisionalNode &decisionalNode,
                                                   JsonObject &jsonObject)
{
    RETURN_IF_FALSE(aNodeToJsonObject(decisionalNode, jsonObject));

    QString refNodeTest;

    if(decisionalNode.getReferenceNodeToTest())
    {
        refNodeTest = decisionalNode.getReferenceNodeToTest()->getInstanceName();
    }

    jsonObject[JsonParsersConstants::Sequence::ReferenceNodeNameToTestAttribute] = refNodeTest;

    QString defaultNodeName;

    if(decisionalNode.getDefaultNode())
    {
        defaultNodeName = decisionalNode.getDefaultNode()->getInstanceName();
    }

    jsonObject[JsonParsersConstants::Sequence::DefaultNodeNameAttribute] = defaultNodeName;

    JsonArray toNodes;
    RETURN_IF_FALSE(decisionalNode.getTransitions(toNodes));

    jsonObject[JsonParsersConstants::Sequence::ToNodesNameAttribute] = QVariant::fromValue(toNodes);

    return true;
}

bool NodesToJsonParser::groupNodeToJsonObject(const GroupNode &groupNode, JsonObject &jsonObject)
{
    RETURN_IF_FALSE(targetingNodeToJsonObject(groupNode, jsonObject));

    QSharedPointer<NodesSequence> nodesSequence = groupNode.getChildNodesSequence();

    if(nodesSequence.isNull())
    {
        qWarning() << "The group node: " << groupNode.getInstanceName()
                   << " has no nodes sequences linked to it";
        return false;
    }

    RETURN_IF_FALSE(referencesHolderToJsonObject(nodesSequence->getReferencesHolder(), jsonObject));

    return true;
}

bool NodesToJsonParser::inclusionNodeToJsonObject(const InclusionNode &inclusionNode,
                                                  JsonObject &jsonObject)
{
    RETURN_IF_FALSE(targetingNodeToJsonObject(inclusionNode, jsonObject));

    QSharedPointer<const NodesSequence> nodesSeq = inclusionNode.getChildNodesSequence();

    QString sequenceFileName;

    if(!nodesSeq.isNull())
    {
        RETURN_IF_FALSE(referencesHolderToJsonObject(nodesSeq->getReferencesHolder(), jsonObject));

        sequenceFileName = inclusionNode.getSequenceFileName();
    }
    else
    {
        RETURN_IF_FALSE(referencesHolderToJsonObject(
                                        ReferencesHolder(ReferencesHolderType::CanHaveInternalRefs),
                                        jsonObject));
    }

    jsonObject[JsonParsersConstants::Sequence::OtherFileReferencePathAttribute] = sequenceFileName;

    return true;
}

bool NodesToJsonParser::aNodeToJsonObject(const ANode &nodeObject, JsonObject &jsonObject)
{
    jsonObject[JsonParsersConstants::Generic::UniqueNameAttribute] = nodeObject.getInstanceName();

    if(!nodeObject.getDescription().isEmpty())
    {
        jsonObject[JsonParsersConstants::Generic::DescriptionName] = nodeObject.getDescription();
    }

    if(nodeObject.getNodeType() == NodeType::UnknownNode)
    {
        qWarning() << "Can't transform an unknown node: " << nodeObject.getInstanceName();
        return false;
    }

    jsonObject[JsonParsersConstants::Generic::TypeName] =
                                                    NodeType::toString(nodeObject.getNodeType());

    QString parentName = "";

    if(nodeObject.getInstanceParent())
    {
        parentName = nodeObject.getInstanceParent()->getInstanceName();
    }

    jsonObject[JsonParsersConstants::Sequence::ParentInstanceNameAttribute] = parentName;

    return true;
}

bool NodesToJsonParser::referencesHolderToJsonObject(const ReferencesHolder &referencesHolder,
                                                     JsonObject &jsonObject)
{
    JsonArray inputs;
    JsonArray outputs;

    for(const InNodePort &inputNode : referencesHolder.getInputs())
    {
        JsonObject inNodeObject;
        RETURN_IF_FALSE(inputNodePortToJsonObject(inputNode, inNodeObject));
        inputs.append(inNodeObject);
    }

    jsonObject[JsonParsersConstants::Sequence::InputsNameAttribute] = QVariant::fromValue(inputs);

    for(const OutNodePort &outputNode : referencesHolder.getOutputs())
    {
        JsonObject outNodeObject;
        RETURN_IF_FALSE(outputNodePortToJsonObject(outputNode, outNodeObject));
        outputs.append(outNodeObject);
    }

    jsonObject[JsonParsersConstants::Sequence::OutputsNameAttribute] = QVariant::fromValue(outputs);

    return true;
}

bool NodesToJsonParser::inputNodePortToJsonObject(const InNodePort &inNodePort,
                                                  JsonObject &jsonObject)
{
    RETURN_IF_FALSE(aNodePortToJsonObject(inNodePort, jsonObject));

    if(inNodePort.getInputNode())
    {
        jsonObject[JsonParsersConstants::Sequence::ReferenceNodeNameAttribute] =
                                                    inNodePort.getInputNode()->getInstanceName();
    }

    if(inNodePort.getInternalInputNode())
    {
        jsonObject[JsonParsersConstants::Sequence::InternalBindingRefNodeNameAttribute] =
                                            inNodePort.getInternalInputNode()->getInstanceName();
    }

    if(inNodePort.isReferenceCanBeUndefined())
    {
        jsonObject[JsonParsersConstants::Sequence::CanBeUndefinedAttribute] = true;
    }

    return true;
}

bool NodesToJsonParser::outputNodePortToJsonObject(const OutNodePort &outNodePort,
                                                   JsonObject &jsonObject)
{
    RETURN_IF_FALSE(aNodePortToJsonObject(outNodePort, jsonObject));

    if(outNodePort.getOutputNode())
    {
        jsonObject[JsonParsersConstants::Sequence::VariableNodeNameAttribute] =
                                                    outNodePort.getOutputNode()->getInstanceName();
    }

    if(outNodePort.getInternalOutputNode())
    {
        jsonObject[JsonParsersConstants::Sequence::InternalBindingVarNodeNameAttribute] =
                                            outNodePort.getInternalOutputNode()->getInstanceName();
    }

    return true;
}

bool NodesToJsonParser::aNodePortToJsonObject(const ANodePort &aNodePort, JsonObject &jsonObject)
{
    jsonObject[JsonParsersConstants::Generic::NameAttribute] = aNodePort.getName();

    if(aNodePort.getTypeManaged() == TypeManaged::Unknown)
    {
        qWarning() << "The type of the node port is unknown: " << aNodePort.getName();
        return false;
    }

    jsonObject[JsonParsersConstants::Sequence::TypeManagedNameAttribute] =
                                                TypeManaged::toString(aNodePort.getTypeManaged());

    const Unit &unit = aNodePort.getUnit();

    if(unit.getUnitType() != UnitType::None)
    {
        jsonObject[JsonParsersConstants::Sequence::UnitNameAttribute] =
                                                            UnitType::getSymbol(unit.getUnitType());
        
        jsonObject[JsonParsersConstants::Sequence::UnitPowerOf10NameAttribute] =
                                                            QString::number(unit.getPowerOfTen());
    }

    return true;
}

bool NodesToJsonParser::targetingNodeToJsonObject(const ATargetingNode &targetingNode,
                                                  JsonObject &jsonObject)
{
    RETURN_IF_FALSE(aNodeToJsonObject(targetingNode, jsonObject));

    QString toNode = "";

    if(targetingNode.getNodeTargeted())
    {
        toNode = targetingNode.getNodeTargeted()->getInstanceName();
    }

    jsonObject[JsonParsersConstants::Sequence::ToNodeNameAttribute] = toNode;

    return true;
}

bool NodesToJsonParser::aReferenceNodeToJsonObject(const AReferenceNode &refNode,
                                                   JsonObject &jsonObject)
{
    RETURN_IF_FALSE(aNodeToJsonObject(refNode, jsonObject));

    if(refNode.getTypeManaged() == TypeManaged::Unknown)
    {
        qWarning() << "The type of the node port is unknown: " << refNode.getInstanceName();
        return false;
    }

    jsonObject[JsonParsersConstants::Sequence::TypeManagedNameAttribute] =
                                                TypeManaged::toString(refNode.getTypeManaged());

    const Unit &unit = refNode.getUnit();

    if(unit.getUnitType() != UnitType::None)
    {
        jsonObject[JsonParsersConstants::Sequence::UnitNameAttribute] = unit.toString();
        
        jsonObject[JsonParsersConstants::Sequence::UnitPowerOf10NameAttribute] =
                                                            QString::number(unit.getPowerOfTen());
    }

    return true;
}

bool NodesToJsonParser::loadNodesSequenceAndChildrenToJsonObject(const NodesSequence &nodesSequence,
                                                                 JsonArray &jsonArray)
{
    JsonObject nodesSequenceObj;

    RETURN_IF_FALSE(nodesSequenceToJsonObject(nodesSequence, nodesSequenceObj));

    jsonArray.append(nodesSequenceObj);

    return loadChildrenToJsonObject(nodesSequence, jsonArray);
}

bool NodesToJsonParser::loadChildrenToJsonObject(const NodesSequence &nodesSequence,
                                                 JsonArray &jsonArray)
{
    QVector<QSharedPointer<AReferenceNode> > refsNodes = nodesSequence.getReferenceNodes();
    for(auto citer = refsNodes.cbegin(); citer != refsNodes.cend(); ++citer)
    {
        JsonObject jsonObject;

        switch((*citer)->getNodeType())
        {
            case NodeType::ConstantNode:
                RETURN_IF_FALSE(constantNodeToJsonObject(*((*citer).objectCast<ConstantNode>()),
                                                         jsonObject));
                break;

            case NodeType::VariableNode:
                RETURN_IF_FALSE(variableNodeToJsonObject(*((*citer).objectCast<VariableNode>()),
                                                         jsonObject));
                break;

            case NodeType::ForkNode:
            case NodeType::JoinNode:
            case NodeType::FinalNode:
            case NodeType::MergeNode:
            case NodeType::InitialNode:
            case NodeType::SequenceModuleNode:
            case NodeType::DecisionalNode:
            case NodeType::InclusionNode:
            case NodeType::NodesSequence:
            case NodeType::GroupNode:
            case NodeType::UnknownNode:
                qWarning() << "The node: " << (*citer)->getInstanceName() << " isn't a reference "
                           << "node, so it's stored at the wrong place";
                return false;
        }

        jsonArray.append(jsonObject);
    }

    QVector<QSharedPointer<ASequenceNode>> seqNodes = nodesSequence.getSequenceNodes();
    for(auto citer = seqNodes.cbegin(); citer != seqNodes.cend(); ++citer)
    {
        JsonObject jsonObject;

        switch((*citer)->getNodeType())
        {
            case NodeType::ConstantNode:
            case NodeType::VariableNode:
            case NodeType::UnknownNode:
                qWarning() << "The node: " << (*citer)->getInstanceName()
                           << " isn't a sequence node, so it's stored at the wrong place";
                return false;

            case NodeType::ForkNode:
                RETURN_IF_FALSE(forkNodeToJsonObject(*((*citer).objectCast<ForkNode>()),
                                                     jsonObject));
                break;

            case NodeType::JoinNode:
                RETURN_IF_FALSE(joinNodeToJsonObject(*((*citer).objectCast<JoinNode>()),
                                                     jsonObject));
                break;

            case NodeType::FinalNode:
                RETURN_IF_FALSE(finalNodeToJsonObject(*((*citer).objectCast<FinalNode>()),
                                                      jsonObject));
                break;

            case NodeType::MergeNode:
                RETURN_IF_FALSE(mergeNodeToJsonObject(*((*citer).objectCast<MergeNode>()),
                                                      jsonObject));
                break;

            case NodeType::InitialNode:
                RETURN_IF_FALSE(initialNodeToJsonObject(*((*citer).objectCast<InitialNode>()),
                                                        jsonObject));
                break;

            case NodeType::SequenceModuleNode:
                RETURN_IF_FALSE(sequenceModuleNodeToJsonObject(
                                                    *((*citer).objectCast<SequenceModuleNode>()),
                                                    jsonObject));
                break;

            case NodeType::DecisionalNode:
                RETURN_IF_FALSE(decisionalNodeToJsonObject(*((*citer).objectCast<DecisionalNode>()),
                                                           jsonObject));
                break;

            case NodeType::InclusionNode:
                RETURN_IF_FALSE(inclusionNodeToJsonObject(*((*citer).objectCast<InclusionNode>()),
                                                          jsonObject));
                break;

            case NodeType::NodesSequence:
                RETURN_IF_FALSE(loadNodesSequenceAndChildrenToJsonObject(
                                                            *((*citer).objectCast<NodesSequence>()),
                                                            jsonArray));
                break;

            case NodeType::GroupNode:
                QSharedPointer<GroupNode> groupNode = citer->objectCast<GroupNode>();

                RETURN_IF_FALSE(groupNodeToJsonObject(*groupNode, jsonObject));

                RETURN_IF_FALSE(loadChildrenToJsonObject(*groupNode->getChildNodesSequence(),
                                                         jsonArray));
                break;
        }

        jsonArray.append(jsonObject);
    }

    return true;
}
