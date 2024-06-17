// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "sequencejsonarrayloader.hpp"

#include <QDebug>
#include <QIODevice>
#include <QJsonArray>
#include <QJsonObject>

#include "errors/helpers/fileloadingerrorcausehelper.hpp"
#include "errors/helpers/sequenceloadingerrorhelper.hpp"
#include "errors/sequenceloadingerrorhandler.hpp"
#include "jsonparsers/jsontonodesparser.hpp"
#include "jsonutility/jsonhelper.hpp"
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
#include "sequence/nodessequencemanager.hpp"
#include "sequence/referencenodes/constantnode.hpp"
#include "sequence/referencenodes/variablenode.hpp"
#include "testbedcore/parsers/commonjsonparser.hpp"
#include "testbedcore/parsers/jsonparsersconstants.hpp"


SequenceJsonArrayLoader::SequenceJsonArrayLoader(NodesSequenceManager &nodesSequenceManager,
                                                 QObject *parent) :
    QObject(parent),
    _nodesSequenceManager(nodesSequenceManager)
{
}

bool SequenceJsonArrayLoader::loadAndParseSequenceFile(NodesSequenceManager &nodesSequenceManager,
                                                       QJsonArray &jsonArray,
                                                       SequenceLoadingErrorHandler *errorHandler)
{
    SequenceJsonArrayLoader seqParser(nodesSequenceManager);
    RETURN_IF_FALSE(seqParser.loadSequence(jsonArray, errorHandler));

    return true;
}

bool SequenceJsonArrayLoader::loadSequence(const QJsonArray &rootArray,
                                           SequenceLoadingErrorHandler *errorHandler)
{
    if(rootArray.isEmpty())
    {
        qInfo() << "Empty project";
        return true;
    }

    _rootNodes = rootArray;

    bool found = false;
    QJsonArray::iterator nodesSequenceIndex;

    RETURN_IF_FALSE(findFirstNode(NodeType::NodesSequence,
                                  "",
                                  found,
                                  nodesSequenceIndex,
                                  errorHandler));

    if(!found)
    {
        FileLoadingErrorCauseHelper::addFirstNodeWrongTypeError(
            NodeType::toString(NodeType::NodesSequence),
            errorHandler);
        qWarning() << "The sequence file doesn't contain the compulsory node: "
                   << NodeType::toString(NodeType::NodesSequence);

        return false;
    }

    QJsonObject jsonObject;

    if(!JsonHelper::getObject(*nodesSequenceIndex, jsonObject))
    {
        FileLoadingErrorCauseHelper::addFirstNodeWrongTypeError(
            NodeType::toString(NodeType::NodesSequence),
            errorHandler);
        qWarning() << "The node: "<< NodeType::toString(NodeType::NodesSequence) << ", hasn't been "
                   << "found";

        return false;
    }

    // Remove extracted node
    _alreadyCreated.append(nodesSequenceIndex);

    QSharedPointer<NodesSequence> nodesSequence;

    TEST_AND_RETURN_IF_FALSE(loadNodesSequence(jsonObject,
                                               nodesSequence,
                                               QSharedPointer<NodesSequence>(),
                                               errorHandler),
                             SequenceLoadingErrorHelper::addFailedNodesSequenceError(
                                 errorHandler,
                                 nodesSequence->getInstanceName()););

    _nodesSequenceManager.setNodesSeq(nodesSequence);

    // Load all the children
    TEST_AND_RETURN_IF_FALSE(loadAllChildren(nodesSequence, errorHandler),
                             SequenceLoadingErrorHelper::addFailedNodesSequenceError(
                                 errorHandler,
                                 nodesSequence->getInstanceName()););

        return true;
}

bool SequenceJsonArrayLoader::loadNodesSequence(
    const QJsonObject &nodesSequenceJsonObject,
    QSharedPointer<NodesSequence> &nodesSequence,
    const QSharedPointer<NodesSequence> &parentNodesSequence,
    SequenceLoadingErrorHandler *errorHandler)
{
    nodesSequence = NodesSequence::createNodesSequence();

    RETURN_IF_FALSE(JsonToNodesParser::loadAbstractNode(nodesSequenceJsonObject,
                                                        *nodesSequence,
                                                        errorHandler));

    // Before all, load all the variables and constants
    RETURN_IF_FALSE(loadAllReferenceNodes(nodesSequence, errorHandler));

    RETURN_IF_FALSE(JsonToNodesParser::loadNodesSequenceReferences(nodesSequenceJsonObject,
                                                                   *nodesSequence.data(),
                                                                   parentNodesSequence.data(),
                                                                   errorHandler));

    return true;
}

bool SequenceJsonArrayLoader::loadAllChildren(QSharedPointer<NodesSequence> &nodesSequence,
                                              SequenceLoadingErrorHandler *errorHandler)
{
    QJsonArray::Iterator iter = _rootNodes.begin();

    for(;iter != _rootNodes.end(); ++iter)
    {
        if(_alreadyCreated.contains(iter))
        {
            continue;
        }

        QSharedPointer<ASequenceNode> node;
        RETURN_IF_FALSE(loadSequenceNode(iter, nodesSequence, node, errorHandler));

        if(node)
        {
            nodesSequence->addSequenceNode(node);
        }
    }

    return true;
}

bool SequenceJsonArrayLoader::loadAllReferenceNodes(
    const QSharedPointer<NodesSequence> &nodesSequence,
    SequenceLoadingErrorHandler *errorHandler)
{
    QJsonArray::Iterator iter = _rootNodes.begin();

    for(;iter != _rootNodes.end(); ++iter)
    {
        if(_alreadyCreated.contains(iter))
        {
            continue;
        }

        if(!iter->isObject())
        {
            FileLoadingErrorCauseHelper::addElementsNotObjectsError(errorHandler);
            qWarning() << "Some node elements in the file are not objects";
            return false;
        }

        QJsonObject nodeObject = iter->toObject();

        QString nodeParentName;

        TEST_AND_RETURN_IF_FALSE(JsonToNodesParser::getParentInstanceName(nodeObject,
                                                                          nodeParentName),
                                 FileLoadingErrorCauseHelper::addUnavailableParentInstanceNameError(
                                     errorHandler););

        if(nodeParentName != nodesSequence->getInstanceName())
        {
            // In this method, we want to get a node child of the NodesSequence given, so if the
            // parent name got is not egual to the NodesSequence name, we don't load the node. But
            // it's not an error.
            continue;
        }

        QString type;

        TEST_AND_RETURN_IF_FALSE(JsonHelper::getString(
                                     nodeObject,
                                     JsonParsersConstants::Generic::TypeName,
                                     type),
                                 FileLoadingErrorCauseHelper::addUnavailableNodeTypeError(
                                     errorHandler););

        NodeType::Enum nodeType = NodeType::parseFromString(type);

        QSharedPointer<AReferenceNode> refNode;

        if(nodeType == NodeType::ConstantNode)
        {
            ConstantNode *constantNode = new ConstantNode(nodesSequence);

            if(!JsonToNodesParser::loadConstantNode(nodeObject, *constantNode, errorHandler))
            {
                SequenceLoadingErrorHelper::addFailedChildNodeError(constantNode->getInstanceName(),
                                                                    NodeType::ConstantNode,
                                                                    errorHandler);
                delete constantNode;
                return false;
            }

            refNode = QSharedPointer<ConstantNode>(constantNode);
        }
        else if(nodeType == NodeType::VariableNode)
        {
            VariableNode *variableNode = new VariableNode(nodesSequence);

            if(!JsonToNodesParser::loadVariableNode(nodeObject, *variableNode, errorHandler))
            {
                SequenceLoadingErrorHelper::addFailedChildNodeError(variableNode->getInstanceName(),
                                                                    NodeType::VariableNode,
                                                                    errorHandler);
                delete variableNode;
                return false;
            }

            refNode = QSharedPointer<VariableNode>(variableNode);
        }

        if(!refNode.isNull())
        {
            nodesSequence->addReferenceNode(refNode);
            _alreadyCreated.append(iter);
        }
    }

    return true;
}

bool SequenceJsonArrayLoader::findFirstNode(NodeType::Enum nodeType,
                                            const QString &parentInstanceName,
                                            bool &found,
                                            QJsonArray::iterator &foundNodeIndex,
                                            SequenceLoadingErrorHandler *errorHandler)
{
    QJsonArray::iterator iter = _rootNodes.begin();
    found = false;

    for(;iter != _rootNodes.end(); ++iter)
    {
        if(_alreadyCreated.contains(iter))
        {
            continue;
        }

        QJsonObject jsonObject;

        if(!JsonHelper::getObject(*iter, jsonObject))
        {
            FileLoadingErrorCauseHelper::addUnavailableFirstNodeError(errorHandler);
            qWarning() << "One element of the sequence file isn't well formed (it's not an object)";
            return false;
        }

        NodeType::Enum currNodeType;

        RETURN_IF_FALSE(JsonToNodesParser::getNodeType(jsonObject, currNodeType));

        if(currNodeType == NodeType::UnknownNode)
        {
            FileLoadingErrorCauseHelper::addUnknownFirstNodeTypeError(errorHandler);
            qWarning() << "The element has an unknown type";
            return false;
        }

        if(nodeType != currNodeType)
        {
            // Continue to iterate
            continue;
        }

        if(nodeType == NodeType::NodesSequence)
        {
            found = true;
            foundNodeIndex = iter;
            break;
        }

        QString currParentInstanceName;

        if(!JsonHelper::getString(jsonObject,
                                  JsonParsersConstants::Sequence::ParentInstanceNameAttribute,
                                  currParentInstanceName))
        {
            FileLoadingErrorCauseHelper::addFirstNodeMissingAttributeError(
                JsonParsersConstants::Sequence::ParentInstanceNameAttribute,
                errorHandler);
            qWarning() << "The element has no the compulsory attribute: "
                       << JsonParsersConstants::Sequence::ParentInstanceNameAttribute;
            return false;
        }

        if(currParentInstanceName == parentInstanceName)
        {
            found = true;
            foundNodeIndex = iter;
            break;
        }
    }

    return true;
}

bool SequenceJsonArrayLoader::findAndLoadSequenceNode(
    const QJsonObject &fromNode,
    const QString &attributeName,
    QSharedPointer<NodesSequence> &nodesSequence,
    QSharedPointer<ASequenceNode> &nodeFoundAndLoaded,
    SequenceLoadingErrorHandler *errorHandler)
{
    QString nodeName;

    TEST_AND_RETURN_IF_FALSE(JsonHelper::getString(fromNode, attributeName, nodeName, true),
                             FileLoadingErrorCauseHelper::addUnavailableAttributeError(
                                 attributeName,
                                 errorHandler););

    if(!nodeName.isEmpty())
    {
        QString uniqueName;

        TEST_AND_RETURN_IF_FALSE(JsonHelper::getString(
                                     fromNode,
                                     JsonParsersConstants::Generic::UniqueNameAttribute,
                                     uniqueName),
                                 FileLoadingErrorCauseHelper::addUnavailableAttributeError(
                                     JsonParsersConstants::Generic::UniqueNameAttribute,
                                     errorHandler););

        if(uniqueName == nodeName)
        {
            FileLoadingErrorCauseHelper::addNodeCannotTargetItselfError(errorHandler);
            qWarning() << "The node name can't target itself";
            return false;
        }

        RETURN_IF_FALSE(findAndLoadSequenceNode(nodeName,
                                                nodesSequence,
                                                nodeFoundAndLoaded,
                                                errorHandler));

        if(nodeFoundAndLoaded.isNull())
        {
            FileLoadingErrorCauseHelper::addTargetedNodeNotFound(nodeName, errorHandler);
            qWarning() << "The node: " << nodeName << ", doesn't exist";
            return false;
        }
    }

    return true;
}

bool SequenceJsonArrayLoader::findAndLoadSequenceNode(
    const QString &nodeName,
    QSharedPointer<NodesSequence> &nodesSequence,
    QSharedPointer<ASequenceNode> &nodeFoundAndLoaded,
    SequenceLoadingErrorHandler *errorHandler)
{
    QSharedPointer<ANode> tmpNode = nodesSequence->getSequenceNode(nodeName);

    nodeFoundAndLoaded = tmpNode.objectCast<ASequenceNode>();

    if(!nodeFoundAndLoaded.isNull())
    {
        return true;
    }

    if(!tmpNode.isNull())
    {
        FileLoadingErrorCauseHelper::addNodeFoundIsNotNodeSequenceError(tmpNode->getInstanceName(),
                                                                        errorHandler);
        qWarning() << "A problem occurred, the node found is not a sequence node: "
                   << tmpNode->getInstanceName();
        return false;
    }

    if(_loadingNodes.contains(nodeName))
    {
        // This is used to avoid circular loop, the node is currently loading, therefore no need to
        // load it again.
        nodeFoundAndLoaded = _loadingNodes.value(nodeName);
        return true;
    }

    QJsonArray::Iterator iter = _rootNodes.begin();

    for(;iter != _rootNodes.end(); ++iter)
    {
        if(_alreadyCreated.contains(iter))
        {
            continue;
        }

        if(!iter->isObject())
        {
            FileLoadingErrorCauseHelper::addElementsNotObjectsError(errorHandler);
            qWarning() << "Some node elements in the file are not objects";
            return false;
        }

        QJsonObject nodeObject = iter->toObject();

        QString nodeUniqueName;

        TEST_AND_RETURN_IF_FALSE(JsonHelper::getString(
                                     nodeObject,
                                     JsonParsersConstants::Generic::UniqueNameAttribute,
                                     nodeUniqueName),
                                 FileLoadingErrorCauseHelper::addUnavailableAttributeError(
                                     JsonParsersConstants::Generic::UniqueNameAttribute,
                                     errorHandler););

        if(nodeUniqueName != nodeName)
        {
            continue;
        }

        RETURN_IF_FALSE(loadSequenceNode(iter, nodesSequence, nodeFoundAndLoaded, errorHandler));

        if(!nodeFoundAndLoaded.isNull())
        {
            nodesSequence->addSequenceNode(nodeFoundAndLoaded);
            _loadingNodes.remove(nodeName);
            qDebug() << "Node completely loaded: " << nodeName;
            _alreadyCreated.append(iter);
            break;
        }
    }

    return true;
}

bool SequenceJsonArrayLoader::loadSequenceNode(const QJsonArray::Iterator &iter,
                                               QSharedPointer<NodesSequence> &nodesSequence,
                                               QSharedPointer<ASequenceNode> &nodeToLoad,
                                               SequenceLoadingErrorHandler *errorHandler)
{
    if(!iter->isObject())
    {
        FileLoadingErrorCauseHelper::addElementsNotObjectsError(errorHandler);
        qWarning() << "Some node elements in the file are not objects";
        return false;
    }

    QJsonObject nodeObject = iter->toObject();

    QString nodeParentName;

    TEST_AND_RETURN_IF_FALSE(JsonToNodesParser::getParentInstanceName(nodeObject, nodeParentName),
                             FileLoadingErrorCauseHelper::addUnavailableParentInstanceNameError(
                                 errorHandler););

    if(nodeParentName != nodesSequence->getInstanceName())
    {
        // In this method, we want to get a node child of the NodesSequence given, so if the parent
        // name got is not egual to the NodesSequence name, we don't load the node. But it's not
        // an error.
        return true;
    }

    NodeType::Enum nodeType = NodeType::UnknownNode;

    TEST_AND_RETURN_IF_FALSE(JsonToNodesParser::getNodeType(nodeObject, nodeType, errorHandler),
                             FileLoadingErrorCauseHelper::addUnavailableNodeTypeError(
                                 errorHandler););

    switch(nodeType)
    {
        case NodeType::NodesSequence:
        case NodeType::VariableNode:
        case NodeType::ConstantNode:
            // The nodes aren't managed by this function (there aren't nodessequence),
            // we ignore them
            return true;

        case NodeType::UnknownNode:
            FileLoadingErrorCauseHelper::addUnknownNodeTypeError(errorHandler);
            qWarning() << "The element at index has an unknown type";
            return false;

        case NodeType::FinalNode:
            TEST_AND_RETURN_IF_FALSE(loadDefaultNode(
                                         nodeObject,
                                         QSharedPointer<FinalNode>(new FinalNode(nodesSequence)),
                                         nodeToLoad,
                                         errorHandler),
                                     SequenceLoadingErrorHelper::addFailedChildNodeError(
                                         nodeToLoad->getInstanceName(),
                                         NodeType::FinalNode,
                                         errorHandler););
            break;

        case NodeType::ForkNode:
            TEST_AND_RETURN_IF_FALSE(loadForkNode(
                                         nodeObject,
                                         nodesSequence,
                                         nodeToLoad,
                                         errorHandler),
                                     SequenceLoadingErrorHelper::addFailedChildNodeError(
                                         nodeToLoad->getInstanceName(),
                                         NodeType::ForkNode,
                                         errorHandler););
            break;

        case NodeType::JoinNode:
            TEST_AND_RETURN_IF_FALSE(loadJoinNode(
                                         nodeObject,
                                         nodesSequence,
                                         nodeToLoad,
                                         errorHandler),
                                     SequenceLoadingErrorHelper::addFailedChildNodeError(
                                         nodeToLoad->getInstanceName(),
                                         NodeType::JoinNode,
                                         errorHandler););
            break;

        case NodeType::InitialNode:
            TEST_AND_RETURN_IF_FALSE(loadDefaultNode(
                                         nodeObject,
                                         QSharedPointer<InitialNode>(
                                             new InitialNode(nodesSequence)),
                                         nodeToLoad,
                                         errorHandler),
                                     SequenceLoadingErrorHelper::addFailedChildNodeError(
                                         nodeToLoad->getInstanceName(),
                                         NodeType::InitialNode,
                                         errorHandler););
            break;

        case NodeType::MergeNode:
            TEST_AND_RETURN_IF_FALSE(loadDefaultNode(
                                         nodeObject,
                                         QSharedPointer<MergeNode>(new MergeNode(nodesSequence)),
                                         nodeToLoad,
                                         errorHandler),
                                     SequenceLoadingErrorHelper::addFailedChildNodeError(
                                         nodeToLoad->getInstanceName(),
                                         NodeType::MergeNode,
                                         errorHandler););
            break;

        case NodeType::SequenceModuleNode:
            TEST_AND_RETURN_IF_FALSE(loadSequenceModuleNode(
                                         nodeObject,
                                         nodesSequence,
                                         nodeToLoad,
                                         errorHandler),
                                     SequenceLoadingErrorHelper::addFailedChildNodeError(
                                         nodeToLoad->getInstanceName(),
                                         NodeType::SequenceModuleNode,
                                         errorHandler););
            break;

        case NodeType::DecisionalNode:
            TEST_AND_RETURN_IF_FALSE(loadDecisionalNode(
                                         nodeObject,
                                         nodesSequence,
                                         nodeToLoad,
                                         errorHandler),
                                     SequenceLoadingErrorHelper::addFailedChildNodeError(
                                         nodeToLoad->getInstanceName(),
                                         NodeType::DecisionalNode,
                                         errorHandler););
            break;

        case NodeType::GroupNode:
            TEST_AND_RETURN_IF_FALSE(loadGroupNode(
                                         nodeObject,
                                         nodesSequence,
                                         nodeToLoad,
                                         errorHandler),
                                     SequenceLoadingErrorHelper::addFailedChildNodeError(
                                         nodeToLoad->getInstanceName(),
                                         NodeType::GroupNode,
                                         errorHandler););
            break;

        case NodeType::InclusionNode:
            TEST_AND_RETURN_IF_FALSE(loadInclusionNode(
                                         nodeObject,
                                         nodesSequence,
                                         nodeToLoad,
                                         errorHandler),
                                     SequenceLoadingErrorHelper::addFailedChildNodeError(
                                         nodeToLoad->getInstanceName(),
                                         NodeType::InclusionNode,
                                         errorHandler););
            break;
    }

    qDebug() << "Node loaded: " << nodeToLoad->getInstanceName();

    QSharedPointer<ATargetingNode> targetingNode = nodeToLoad.objectCast<ATargetingNode>();

    if(targetingNode)
    {
        QSharedPointer<ASequenceNode> targetedNode;

        RETURN_IF_FALSE(findAndLoadSequenceNode(nodeObject,
                                                JsonParsersConstants::Sequence::ToNodeNameAttribute,
                                                nodesSequence,
                                                targetedNode,
                                                errorHandler));

        if(targetedNode)
        {
            // The targeted node has been given
            targetingNode->addTransitionToNode(targetedNode);
        }
    }

    return true;
}

bool SequenceJsonArrayLoader::loadDefaultNode(const QJsonObject &defaultObject,
                                              const QSharedPointer<ASequenceNode> &nodeCreated,
                                              QSharedPointer<ASequenceNode> &nodeToLoad,
                                              SequenceLoadingErrorHandler *errorHandler)
{
    RETURN_IF_FALSE(JsonToNodesParser::loadAbstractNode(defaultObject, *nodeCreated, errorHandler));

    qDebug() << "Loading nodes: " << nodeCreated->getInstanceName();
    _loadingNodes.insert(nodeCreated->getInstanceName(), nodeCreated);

    nodeToLoad = nodeCreated;

    return true;
}

bool SequenceJsonArrayLoader::loadSequenceModuleNode(
    const QJsonObject &seqModuleObject,
    const QSharedPointer<NodesSequence> &nodesSequence,
    QSharedPointer<ASequenceNode> &nodeToLoad,
    SequenceLoadingErrorHandler *errorHandler)
{
    QSharedPointer<SequenceModuleNode> sequenceModuleNode =
                      QSharedPointer<SequenceModuleNode>(new SequenceModuleNode(nodesSequence));

    RETURN_IF_FALSE(loadDefaultNode(seqModuleObject, sequenceModuleNode, nodeToLoad, errorHandler));

    RETURN_IF_FALSE(JsonToNodesParser::loadSequenceModuleNode(seqModuleObject,
                                                              *nodesSequence,
                                                              *sequenceModuleNode,
                                                              errorHandler));

    nodeToLoad = sequenceModuleNode;

    return true;
}

bool SequenceJsonArrayLoader::loadForkNode(const QJsonObject &forkObject,
                                           QSharedPointer<NodesSequence> &nodesSequence,
                                           QSharedPointer<ASequenceNode> &nodeToLoad,
                                           SequenceLoadingErrorHandler *errorHandler)
{
    QSharedPointer<ForkNode> forkNode = QSharedPointer<ForkNode>(new ForkNode(nodesSequence));

    RETURN_IF_FALSE(loadDefaultNode(forkObject, forkNode, nodeToLoad, errorHandler));

    QSharedPointer<ASequenceNode> joinNode;

    RETURN_IF_FALSE(findAndLoadSequenceNode(
        forkObject,
        JsonParsersConstants::Sequence::JoinNodeInstanceNameAttribute,
        nodesSequence,
        joinNode,
        errorHandler));

    QSharedPointer<JoinNode> joinNodeCasted = joinNode.objectCast<JoinNode>();

    if(joinNodeCasted.isNull())
    {
        QString joinNodeName;
        TEST_AND_RETURN_IF_FALSE(JsonHelper::getString(
                                     forkObject,
                                     JsonParsersConstants::Sequence::JoinNodeInstanceNameAttribute,
                                     joinNodeName),
                                 FileLoadingErrorCauseHelper::addUnavailableAttributeError(
                                     joinNodeName,
                                     errorHandler););

        FileLoadingErrorCauseHelper::addJoinNodeNotFoundError(joinNodeName, errorHandler);

        qWarning() << "The join node: " << joinNodeName
                   << ", doesn't exist or it's not a join node";
        return false;
    }

    if(!joinNodeCasted->getForkNodeLinked().isNull())
    {
        FileLoadingErrorCauseHelper::addJoinNodeAlreadyLinkedToForkNodeError(
            joinNodeCasted->getInstanceName(),
            joinNodeCasted->getForkNodeLinked()->getInstanceName(),
            errorHandler);

        qWarning() << "The join node: " << joinNodeCasted->getInstanceName() << ", is already "
                   << "linked to the fork node: "
                   << joinNodeCasted->getForkNodeLinked()->getInstanceName();
        return false;
    }

    RETURN_IF_FALSE(forkNode->setJoinNodeLinked(joinNodeCasted, forkNode));

    QStringList toNodesName;
    RETURN_IF_FALSE(JsonHelper::getStringList(forkObject,
                                              JsonParsersConstants::Sequence::ToNodesNameAttribute,
                                              toNodesName));

    if(toNodesName.removeDuplicates() != 0)
    {
        qWarning() << "The forkNode named: " << forkNode->getInstanceName() << ", cannot targets "
                   << "the same node several times";
        return false;
    }

    for(const QString &toNodeName : toNodesName)
    {
        QSharedPointer<ASequenceNode> targetedNode;

        RETURN_IF_FALSE(findAndLoadSequenceNode(toNodeName,
                                                nodesSequence,
                                                targetedNode,
                                                errorHandler));

        RETURN_IF_FALSE(forkNode->addTransitionToNode(targetedNode));
    }

    nodeToLoad = forkNode;

    return true;
}

bool SequenceJsonArrayLoader::loadJoinNode(const QJsonObject &joinObject,
                                           QSharedPointer<NodesSequence> &nodesSequence,
                                           QSharedPointer<ASequenceNode> &nodeToLoad,
                                           SequenceLoadingErrorHandler *errorHandler)
{
    QSharedPointer<JoinNode> joinNode = QSharedPointer<JoinNode>(new JoinNode(nodesSequence));

    RETURN_IF_FALSE(loadDefaultNode(joinObject, joinNode, nodeToLoad));

    QSharedPointer<ASequenceNode> forkNode;

    RETURN_IF_FALSE(findAndLoadSequenceNode(
        joinObject,
        JsonParsersConstants::Sequence::ForkNodeInstanceNameAttribute,
        nodesSequence,
        forkNode,
        errorHandler));

    QSharedPointer<ForkNode> forkNodeCasted = forkNode.objectCast<ForkNode>();

    if(forkNodeCasted.isNull())
    {
        QString forkNodeName;
        TEST_AND_RETURN_IF_FALSE(JsonHelper::getString(
                                     joinObject,
                                     JsonParsersConstants::Sequence::ForkNodeInstanceNameAttribute,
                                     forkNodeName),
                                 FileLoadingErrorCauseHelper::addUnavailableAttributeError(
                                     JsonParsersConstants::Sequence::ForkNodeInstanceNameAttribute,
                                     errorHandler););

        FileLoadingErrorCauseHelper::addForkNodeNotFoundError(forkNodeName, errorHandler);
        qWarning() << "The fork node: " << forkNodeName
                   << ", doesn't exist or it's not a join node";
        return false;
    }

    const QSharedPointer<JoinNode> &joinNodeLinkedToForkNode = forkNodeCasted->getJoinNodeLinked();

    if(!joinNodeLinkedToForkNode.isNull() &&
       joinNodeLinkedToForkNode != joinNode)
    {
        // When calling this method, we can be in two different scenarios:
        //  - The method has been called by the ForkNode Loading (means that the ForkNode has been
        //    firstly encountered in the sequence file. And because it has a link to this object, it
        //    has asked to get this JoinNode).
        //    In this scenario, the ForkNode is loading when it's asking to get this JoinNode, and
        //    it's asking it in order to set a link between the fork and the join node.
        //    Therefore, in this scenario and at this step, the ForkNode has no set the link with
        //    this JoinNode (the attribute is null).That's why we don't test if the current joinNode
        //    linked to the forkNode is this one.
        //  - The method has been called by the JoinNode Loading (means that this JoinNode has been
        //    firstly encoutered in the sequence file.)
        //    In this scenario, asking for the ForkNode will lead to completely load it (if it
        //    hasn't been load it yet). Therefore, in this case, we can test if the current JoinNode
        //    linked to the ForkNode is this one.
        //
        //    To resume: if the JoinNode link contained in the ForkNode is not null, it has to be
        //               this one

        FileLoadingErrorCauseHelper::addForkNodeAlreadyLinkedToJoinNodeError(
            joinNode->getInstanceName(),
            joinNodeLinkedToForkNode->getInstanceName(),
            forkNodeCasted->getInstanceName(),
            errorHandler);
        qWarning() << "This JoinNode: " << joinNode->getInstanceName() << ", is linked to the "
                   << "ForkNode: " << forkNodeCasted->getInstanceName() << ", but the ForkNode has "
                   << "already been connected to the JoinNode: " <<
                      joinNodeLinkedToForkNode->getInstanceName();
        return false;
    }

    return true;
}

bool SequenceJsonArrayLoader::loadDecisionalNode(const QJsonObject &decisionalObject,
                                                 QSharedPointer<NodesSequence> &nodesSequence,
                                                 QSharedPointer<ASequenceNode> &nodeToLoad,
                                                 SequenceLoadingErrorHandler *errorHandler)
{
    QSharedPointer<DecisionalNode> decisionalNode =
                                  QSharedPointer<DecisionalNode>(new DecisionalNode(nodesSequence));

    RETURN_IF_FALSE(loadDefaultNode(decisionalObject, decisionalNode, nodeToLoad, errorHandler));

    QString referenceNodeName;
    TEST_AND_RETURN_IF_FALSE(JsonHelper::getString(
                                 decisionalObject,
                                 JsonParsersConstants::Sequence::ReferenceNodeNameToTestAttribute,
                                 referenceNodeName),
                             FileLoadingErrorCauseHelper::addUnavailableAttributeError(
                                 JsonParsersConstants::Sequence::ReferenceNodeNameToTestAttribute,
                                 errorHandler););

    QSharedPointer<AReferenceNode> refNode = nodesSequence->getReferenceNode(referenceNodeName);

    if(refNode.isNull())
    {
        FileLoadingErrorCauseHelper::addTargetedNodeNotFound(referenceNodeName, errorHandler);
        qWarning() << "The node targeted: " << referenceNodeName << ", doesn't exist";
        return false;
    }

    decisionalNode->setReferenceNodeToTest(refNode);

    QString defaultNodeName;
    QSharedPointer<ASequenceNode> defaultNode;
    TEST_AND_RETURN_IF_FALSE(JsonHelper::getString(
                                 decisionalObject,
                                 JsonParsersConstants::Sequence::DefaultNodeNameAttribute,
                                 defaultNodeName),
                             FileLoadingErrorCauseHelper::addUnavailableAttributeError(
                                 JsonParsersConstants::Sequence::DefaultNodeNameAttribute,
                                 errorHandler););

    QJsonArray nodeArray;
    TEST_AND_RETURN_IF_FALSE(JsonHelper::getArray(
                                 decisionalObject,
                                 JsonParsersConstants::Sequence::ToNodesNameAttribute,
                                 nodeArray),
                             FileLoadingErrorCauseHelper::addUnavailableAttributeError(
                                 JsonParsersConstants::Sequence::ToNodesNameAttribute,
                                 errorHandler););

    for(int idx = 0; idx < nodeArray.count(); idx++)
    {
        QJsonObject transition;
        TEST_AND_RETURN_IF_FALSE(JsonHelper::getObject(nodeArray, idx, transition),
                                 FileLoadingErrorCauseHelper::addUnavailableTransitionError(
                                     errorHandler););

        QString toNodeName;
        TEST_AND_RETURN_IF_FALSE(JsonHelper::getString(
                                     transition,
                                     JsonParsersConstants::Sequence::ToNodeNameAttribute,
                                     toNodeName),
                                 FileLoadingErrorCauseHelper::addUnavailableAttributeError(
                                     JsonParsersConstants::Sequence::ToNodesNameAttribute,
                                     errorHandler););

        QSharedPointer<ASequenceNode> targetedNode;
        RETURN_IF_FALSE(findAndLoadSequenceNode(toNodeName,
                                                nodesSequence,
                                                targetedNode,
                                                errorHandler));

        if(toNodeName == defaultNodeName)
        {
            defaultNode = targetedNode;
        }

        // Parse operators in waterfalls effect
        JsonObject element;
        RETURN_IF_FALSE(loadDecisionalTransition(transition,
                                                 decisionalNode->getInstanceName(),
                                                 refNode->getTypeManaged(),
                                                 element,
                                                 errorHandler));

        // Add transitions
        RETURN_IF_FALSE(decisionalNode->addTransitionToNode(targetedNode, element, errorHandler));
    }

    if(!defaultNode)
    {
        // The default node is not a case of the decisional node
        RETURN_IF_FALSE(findAndLoadSequenceNode(defaultNodeName,
                                                nodesSequence,
                                                defaultNode,
                                                errorHandler));
    }

    decisionalNode->setDefaultNode(defaultNode);

    nodeToLoad = decisionalNode;

    return true;
}

bool SequenceJsonArrayLoader::loadDecisionalTransition(const QJsonObject &toNode,
                                                       const QString &deciName,
                                                       TypeManaged::Enum refTypeManaged,
                                                       JsonObject &transition,
                                                       SequenceLoadingErrorHandler *errorHandler)
{
    QString decisionalOperator;
    TEST_AND_RETURN_IF_FALSE(JsonHelper::getString(
                                 toNode,
                                 JsonParsersConstants::Sequence::OperatorAttribute,
                                 decisionalOperator),
                             FileLoadingErrorCauseHelper::addUnavailableAttributeError(
                                 JsonParsersConstants::Sequence::OperatorAttribute,
                                 errorHandler););

    DecisionalOperator::Enum deciOpe = DecisionalOperator::parseFromString(decisionalOperator);

    if(deciOpe == DecisionalOperator::Unknown)
    {
        FileLoadingErrorCauseHelper::addUnknownDecisionalOperatorError(deciName,
                                                                       decisionalOperator,
                                                                       errorHandler);
        qWarning() << "In the node: " << deciName << ". The decisional operator: "
                   << decisionalOperator << " is not known";
        return false;
    }

    transition[JsonParsersConstants::Sequence::OperatorAttribute] = deciOpe;

    if(deciOpe == DecisionalOperator::Or || deciOpe == DecisionalOperator::And)
    {
        if(!toNode.contains(JsonParsersConstants::Sequence::ElementsNameAttribute))
        {
            FileLoadingErrorCauseHelper::addUnavailableAttributeError(
                JsonParsersConstants::Sequence::ElementsNameAttribute,
                errorHandler);

            qWarning() << "Cannot find the attribute: "
                       << JsonParsersConstants::Sequence::ElementsNameAttribute
                       << ", in the decisional node: " << deciName;
            return false;
        }

        QJsonArray jsonElements;
        TEST_AND_RETURN_IF_FALSE(JsonHelper::getArray(
                                     toNode,
                                     JsonParsersConstants::Sequence::ElementsNameAttribute,
                                     jsonElements),
                                 FileLoadingErrorCauseHelper::addUnavailableAttributeError(
                                     JsonParsersConstants::Sequence::ElementsNameAttribute,
                                     errorHandler););

        JsonArray elements;
        for(int idx = 0; idx < jsonElements.count(); idx++)
        {
            QJsonObject subJsonObject;
            TEST_AND_RETURN_IF_FALSE(JsonHelper::getObject(
                                         jsonElements,
                                         idx,
                                         subJsonObject),
                                     FileLoadingErrorCauseHelper::addUnavailableElementsError(
                                         errorHandler););

            JsonObject subObject;
            RETURN_IF_FALSE(loadDecisionalTransition(subJsonObject,
                                                     deciName,
                                                     refTypeManaged,
                                                     subObject,
                                                     errorHandler));

            elements.append(QVariant::fromValue(subObject));
        }

        transition[JsonParsersConstants::Sequence::ElementsNameAttribute] =
                                                                    QVariant::fromValue(elements);
    }
    else
    {
        if(!toNode.contains(JsonParsersConstants::Sequence::ValueNameAttribute))
        {
            FileLoadingErrorCauseHelper::addUnavailableAttributeError(
                JsonParsersConstants::Sequence::ElementsNameAttribute,
                errorHandler);

            qWarning() << "Cannot find the attribute: "
                       << JsonParsersConstants::Sequence::ValueNameAttribute
                       << ", in the decisional node: " << deciName;
            return false;
        }

        QVariant value;
        RETURN_IF_FALSE(CommonJsonParser::getValue(
            toNode,
            JsonParsersConstants::Sequence::ValueNameAttribute,
            refTypeManaged,
            value,
            false,
            errorHandler));

        transition[JsonParsersConstants::Sequence::ValueNameAttribute] = value;
    }

    return true;
}

bool SequenceJsonArrayLoader::loadASequenceContainerNode(
    const QJsonObject &containerObject,
    const QSharedPointer<NodesSequence> &childNodesSequence,
    const QSharedPointer<ASequenceContainerNode> &nodeCreated,
    SequenceLoadingErrorHandler *errorHandler)
{
    bool emitErrorIfSequenceFailed = true;
    TEST_AND_RETURN_IF_FALSE(JsonHelper::getBoolean(
                                 containerObject,
                                 JsonParsersConstants::Sequence::EmitErrorIfSequenceFailedAttribute,
                                 emitErrorIfSequenceFailed,
                                 true),
                             FileLoadingErrorCauseHelper::addUnavailableAttributeError(
                                 JsonParsersConstants::Sequence::EmitErrorIfSequenceFailedAttribute,
                                 errorHandler););

    nodeCreated->setFailIfAnErrorOccured(emitErrorIfSequenceFailed);

    if(!emitErrorIfSequenceFailed && childNodesSequence)
    {
        bool errorOutputFound = false;

        for(const OutNodePort &out : childNodesSequence->getReferencesHolder().getOutputs())
        {
            if(out.getName() == JsonParsersConstants::Sequence::SequenceSuccessOutput)
            {
                if(out.getTypeManaged() != TypeManaged::Boolean)
                {
                    FileLoadingErrorCauseHelper::addGroupNodeOutputNotBoolError(
                        JsonParsersConstants::Sequence::SequenceSuccessOutput,
                        nodeCreated->getInstanceName(),
                        errorHandler);
                    qWarning() << "Error with the output: "
                               << JsonParsersConstants::Sequence::SequenceSuccessOutput
                               << ", of the group node: " << nodeCreated->getInstanceName()
                               << ", it's not a boolean";
                    return false;
                }

                errorOutputFound = true;
            }
        }

        if(!errorOutputFound)
        {
            FileLoadingErrorCauseHelper::addCompulsaryOutputGroupNodeError(
                JsonParsersConstants::Sequence::SequenceSuccessOutput,
                nodeCreated->getInstanceName(),
                JsonParsersConstants::Sequence::EmitErrorIfSequenceFailedAttribute,
                errorHandler);
            qWarning() << "The output: "
                       << JsonParsersConstants::Sequence::SequenceSuccessOutput
                       << ", is compulsory in the group node: " << nodeCreated->getInstanceName()
                       << ", because the attribute: "
                       << JsonParsersConstants::Sequence::EmitErrorIfSequenceFailedAttribute
                       << ", is equal to false";
            return false;
        }
    }

    return true;
}

bool SequenceJsonArrayLoader::loadGroupNode(const QJsonObject &groupObject,
                                            QSharedPointer<NodesSequence> &nodesSequence,
                                            QSharedPointer<ASequenceNode> &nodeToLoad,
                                            SequenceLoadingErrorHandler *errorHandler)
{
    QSharedPointer<GroupNode> groupNode = QSharedPointer<GroupNode>(new GroupNode(nodesSequence));

    RETURN_IF_FALSE(loadDefaultNode(groupObject, groupNode, nodeToLoad, errorHandler));

    QSharedPointer<NodesSequence> childNodesSequence;

    RETURN_IF_FALSE(loadNodesSequence(groupObject,
                                      childNodesSequence,
                                      nodesSequence,
                                      errorHandler));

    RETURN_IF_FALSE(loadASequenceContainerNode(groupObject,
                                               childNodesSequence,
                                               groupNode,
                                               errorHandler));

    groupNode->setNodesSequence(childNodesSequence);

    // Load all the children
    RETURN_IF_FALSE(loadAllChildren(childNodesSequence, errorHandler));

    return true;
}

bool SequenceJsonArrayLoader::loadInclusionNode(const QJsonObject &inclusionObject,
                                               QSharedPointer<NodesSequence> &nodesSequence,
                                               QSharedPointer<ASequenceNode> &nodeToLoad,
                                               SequenceLoadingErrorHandler *errorHandler)
{
    QSharedPointer<InclusionNode> inclusionNode = QSharedPointer<InclusionNode>(
                                                                new InclusionNode(nodesSequence));

    RETURN_IF_FALSE(loadDefaultNode(inclusionObject, inclusionNode, nodeToLoad, errorHandler));

    QString sequenceFilePath;
    TEST_AND_RETURN_IF_FALSE(JsonHelper::getString(
                                 inclusionObject,
                                 JsonParsersConstants::Sequence::OtherFileReferencePathAttribute,
                                 sequenceFilePath),
                             FileLoadingErrorCauseHelper::addUnavailableAttributeError(
                                 JsonParsersConstants::Sequence::OtherFileReferencePathAttribute,
                                 errorHandler););

    QSharedPointer<NodesSequence> inclusionNodesSequence;

    if(!sequenceFilePath.isEmpty())
    {
        RETURN_IF_FALSE(inclusionNode->setAndLoadSequenceFile(sequenceFilePath, errorHandler));

        inclusionNodesSequence = inclusionNode->getChildNodesSequence();

        if(inclusionNodesSequence.isNull())
        {
            SequenceLoadingErrorHelper::addFailedNodesSequenceError(
                errorHandler,
                inclusionNode->getInstanceName());
            qWarning() << "The nodes sequence linked to the the inclusion node: "
                       << inclusionNode->getInstanceName() << ", is null";
            return false;
        }

        RETURN_IF_FALSE(JsonToNodesParser::loadNodesSequenceReferences(inclusionObject,
                                                                       *inclusionNodesSequence,
                                                                       nodesSequence.data(),
                                                                       errorHandler));
    }

    RETURN_IF_FALSE(loadASequenceContainerNode(inclusionObject,
                                               inclusionNodesSequence,
                                               inclusionNode,
                                               errorHandler));

    return true;
}
