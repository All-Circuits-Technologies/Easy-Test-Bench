// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QHash>
#include <QJsonArray>
#include <QVector>

#include "jsonutility/jsontypedefs.hpp"
#include "sequence/corestatemachine/nodetype.hpp"
#include "sequence/nodes/asequencenode.hpp"
#include "testbedcore/types/typemanaged.hpp"

class ANode;
class ASequenceContainerNode;
class ASequenceNode;
class NodesSequence;
class NodesSequenceManager;
class SequenceFile;
class SequenceLoadingErrorHandler;


/** @brief Class to load the jsonArray from a SequenceFile */
class SequenceJsonArrayLoader : public QObject
{
    Q_OBJECT

    private:
        /** @brief Class constructor
            @param nodesSequenceManager The nodesSequence manager to get informations from
            @param parent The class parent */
        explicit SequenceJsonArrayLoader(NodesSequenceManager &nodesSequenceManager,
                                         QObject *parent = nullptr);

    public:
        /** @brief Load and parse sequence file
            @param nodesSequenceManager NodesSequenceManage to load the nodesSequence in
            @param jsonArray            The jsonArray of the sequence that need to be loaded
            @param errorHandler An optional sequence loading error handler pointer
            @return Return false if an error occurred, else true */
        static bool loadAndParseSequenceFile(NodesSequenceManager &nodesSequenceManager,
                                             QJsonArray &jsonArray,
                                             SequenceLoadingErrorHandler *errorHandler = nullptr);

    private:
        /** @brief Load the sequence from the sequence file rootArray
            @param rootArray The root array of the sequence file
            @param errorHandler An optional sequence loading error handler pointer
            @return True if no problem occurs */
        bool loadSequence(const QJsonArray &rootArray,
                          SequenceLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Load NodesSequence references and default attributes
            @param nodesSequenceJsonObject The JSON object to parse
            @param nodesSequence The nodes sequence to fill with the informations
            @param parentNodesSequence The nodesSequence parent of the previous nodeSequence given
                                       (useful for Inclusion and Group node for instance)
            @param errorHandler An optional sequence loading error handler pointer
            @return True if no problem occurs */
        bool loadNodesSequence(const QJsonObject &nodesSequenceJsonObject,
                               QSharedPointer<NodesSequence> &nodesSequence,
                               const QSharedPointer<NodesSequence> &parentNodesSequence =
                                                                QSharedPointer<NodesSequence>(),
                               SequenceLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Load all the children of the NodesSequence given
            @param nodesSequence The nodesSequence to complete with children nodes
            @param errorHandler An optional sequence loading error handler pointer
            @return True If no problem occurs */
        bool loadAllChildren(QSharedPointer<NodesSequence> &nodesSequence,
                             SequenceLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Load all the reference nodes linked to the NodesSequence given
            @param nodesSequence The nodesSequence to complete with the reference nodes
            @param errorHandler An optional sequence loading error handler pointer
            @return True if no problem occurs */
        bool loadAllReferenceNodes(const QSharedPointer<NodesSequence> &nodesSequence,
                                   SequenceLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Find a node of the type given and return the first one found
            @param nodeType The type of the node to find
            @param parentInstanceName The parent instance name of the node to find
            @param found Set to true in the method, if the node has been found
            @param foundNodeIndex If node has been found, return the node index in the root nodes
            @param errorHandler An optional sequence loading error handler pointer
            @return True if no problem occurs */
        bool findFirstNode(NodeType::Enum nodeType,
                           const QString &parentInstanceName,
                           bool &found,
                           QJsonArray::iterator &foundNodeIndex,
                           SequenceLoadingErrorHandler *errorHandler);

        /** @brief Try to find a node in the NodesSequence children list already loaded,
                   if it's not loaded, try to find and load it from the root nodes
            @param fromNode The method will try to get the name of the node to load, in getting the
                            attribute value from this JSON object
            @param attributeName The method will try to get the name of the node to load, in getting
                                 the value of the attribute given by its name
            @param nodesSequence The node loaded will be added in the nodesSequence
            @param nodeFoundAndLoaded The node found and loaded
            @param errorHandler An optional sequence loading error handler pointer
            @return True if no problem occurs */
        bool findAndLoadSequenceNode(const QJsonObject &fromNode,
                                     const QString &attributeName,
                                     QSharedPointer<NodesSequence> &nodesSequence,
                                     QSharedPointer<ASequenceNode> &nodeFoundAndLoaded,
                                     SequenceLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Try to find a node in the NodesSequence children list already loaded,
                   if it's not loaded, try to find and load it from the root nodes
            @param nodeName The name of the node to find and load
            @param nodesSequence The node loaded will be added in the nodesSequence
            @param nodeFoundAndLoaded The node found and loaded
            @param errorHandler An optional sequence loading error handler pointer
            @return True if no problem occurs */
        bool findAndLoadSequenceNode(const QString &nodeName,
                                     QSharedPointer<NodesSequence> &nodesSequence,
                                     QSharedPointer<ASequenceNode> &nodeFoundAndLoaded,
                                     SequenceLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Load a sequence node information
            @note Even if the method returns true, the nodeToLoad can be NULL, for instance: if the
                  node has been found but its parent doen't match with the NodesSequence given
            @param iter The index in root array of the node to load
            @param nodesSequence The nodesSequence to load the node from
            @param nodeToLoad The node loaded
            @param errorHandler An optional sequence loading error handler pointer
            @return True if no problem occurs */
        bool loadSequenceNode(const QJsonArray::Iterator &iter,
                              QSharedPointer<NodesSequence> &nodesSequence,
                              QSharedPointer<ASequenceNode> &nodeToLoad,
                              SequenceLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Load the shared information of the abstract sequence node
            @param defaultObject The JSON object to parse and load
            @param nodeCreated A node already created to fill with the information got
            @param nodeToLoad A pointer to the nodeCreated, useful in order to simplify the calling
                              code
            @param errorHandler An optional sequence loading error handler pointer
            @return True if no problem occurs */
        bool loadDefaultNode(const QJsonObject &defaultObject,
                             const QSharedPointer<ASequenceNode> &nodeCreated,
                             QSharedPointer<ASequenceNode> &nodeToLoad,
                             SequenceLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Load a sequence module node with all the necessary data
            @param seqModuleObject The JSON object to parse and load
            @param nodesSequence The nodesSequence parent of the node to load
            @param nodeToLoad The method fills this pointer with the node loaded
            @param errorHandler An optional sequence loading error handler pointer
            @return True if no problem occurs */
        bool loadSequenceModuleNode(const QJsonObject &seqModuleObject,
                                    const QSharedPointer<NodesSequence> &nodesSequence,
                                    QSharedPointer<ASequenceNode> &nodeToLoad,
                                    SequenceLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Load a fork node with all the necessary data
            @param forkObject The JSON object to parse and load
            @param nodesSequence The nodesSequence parent of the node to load
            @param nodeToLoad The method fills this pointer with the node loaded
            @param errorHandler An optional sequence loading error handler pointer
            @return True if no problem occurs */
        bool loadForkNode(const QJsonObject &forkObject,
                          QSharedPointer<NodesSequence> &nodesSequence,
                          QSharedPointer<ASequenceNode> &nodeToLoad,
                          SequenceLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Load a Join node with all the necessary data
            @param joinObject The JSON object to parse and load
            @param nodesSequence The nodesSequence parent of the node to load
            @param nodeToLoad The method fills this pointer with the node loaded
            @param errorHandler An optional sequence loading error handler pointer
            @return True if no problem occurs */
        bool loadJoinNode(const QJsonObject &joinObject,
                          QSharedPointer<NodesSequence> &nodesSequence,
                          QSharedPointer<ASequenceNode> &nodeToLoad,
                          SequenceLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Load a decisional node with all the necessary data
            @param decisionalObject The JSON object to parse and load
            @param nodesSequence The nodesSequence parent of the node to load
            @param nodeToLoad The method fills this pointer with the node loaded
            @param errorHandler An optional sequence loading error handler pointer
            @return True if no problem occurs */
        bool loadDecisionalNode(const QJsonObject &decisionalObject,
                                QSharedPointer<NodesSequence> &nodesSequence,
                                QSharedPointer<ASequenceNode> &nodeToLoad,
                                SequenceLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Load a decisional transition to a specific node
            @note The JsonObject returned has the following formats:
                    For the logical operators:
                    {
                        "operator": DecisionnalOperator::Enum,
                        "elements": [
                            [JsonObject of a comparing operator]
                        ]
                    }
                    For the comparing operators:
                    {
                        "operator": DecisionnalOperator::Enum,
                        "value": [string|number|bool]
                    }
            @param toNode One JSON object of the "ToNodes" JSON array
            @param deciName The name of the decisional node linked to the transition to load
            @param refTypeManaged The type managed by the reference linked to the decisional node
            @param transition A JsonObject representation of the transitions
            @param errorHandler An optional sequence loading error handler pointer
            @return True if no problem occurs */
        bool loadDecisionalTransition(const QJsonObject &toNode,
                                      const QString &deciName,
                                      TypeManaged::Enum refTypeManaged,
                                      JsonObject &transition,
                                      SequenceLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Load an abstract container object
            @param containerObject The JSON object to parse and load
            @param childNodesSequence The child nodes sequence to get success output variable from
            @param nodeCreated The SequenceContainerNode already created and to fill with the
                               the specific informations of the node
            @param errorHandler An optional sequence loading error handler pointer
            @return True if no problem occurs */
        bool loadASequenceContainerNode(const QJsonObject &containerObject,
                                        const QSharedPointer<NodesSequence> &childNodesSequence,
                                        const QSharedPointer<ASequenceContainerNode> &nodeCreated,
                                        SequenceLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Load a group node with all the necessary data
            @param groupObject The JSON object to parse and load
            @param nodesSequence The nodesSequence parent of the node to load
            @param nodeToLoad The method fills this pointer with the node loaded
            @param errorHandler An optional sequence loading error handler pointer
            @return True if no problem occurs */
        bool loadGroupNode(const QJsonObject &groupObject,
                           QSharedPointer<NodesSequence> &nodesSequence,
                           QSharedPointer<ASequenceNode> &nodeToLoad,
                           SequenceLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Load an inclusion node with all the necessary data
            @param inclusionObject The JSON object to parse and load
            @param nodesSequence The nodesSequence parent of the node to load
            @param nodeToLoad The method fills this pointer with the node loaded
            @param errorHandler An optional sequence loading error handler pointer
            @return True if no problem occurs */
        bool loadInclusionNode(const QJsonObject &inclusionObject,
                               QSharedPointer<NodesSequence> &nodesSequence,
                               QSharedPointer<ASequenceNode> &nodeToLoad,
                               SequenceLoadingErrorHandler *errorHandler = nullptr);
    private:
        NodesSequenceManager &_nodesSequenceManager;
        QJsonArray _rootNodes;
        QVector<QJsonArray::iterator> _alreadyCreated;
        QHash<QString, QSharedPointer<ASequenceNode>> _loadingNodes;
};
