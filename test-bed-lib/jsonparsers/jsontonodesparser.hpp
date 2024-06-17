// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QtGlobal>

#include "sequence/corestatemachine/nodetype.hpp"

class SequenceLoadingErrorHandler;
class ANode;
class ANodePort;
class AReferenceNode;
class ConstantNode;
class FinalNode;
class ForkNode;
class InclusionNode;
class InitialNode;
class InNodePort;
class ISequenceInstanceModule;
class JoinNode;
class MergeNode;
class NodesSequence;
class OutNodePort;
class QJsonObject;
class ReferencesHolder;
class SequenceModuleNode;
class VariableNode;


/** @brief Helpful methods to parse and load the simple sequence nodes parameters.
    @warning Don't parse all the attributes which point to other sequence nodes. */
class JsonToNodesParser
{
    public:
        /** @brief Help to load the NodesSequence object
            @note - Load the abstract informations (@see loadAbtractNode)
                  - Load and connect to input reference holder
                  - Load and connect to output reference holder
            @warning The references of the nodesSequence have to be loaded before to call this
                     function
            @param nodeObject The JSON object to parse
            @param nodesSequence The NodesSequence to fill with infos got
            @param parentNodesSequence The nodesSequence parent of the nodesSequence param: useful
                                       for inclusion and group node
            @param errorHandler An optional sequence loading error handler pointer
            @return True if no problem occurs */
        static bool loadNodesSequenceReferences(const QJsonObject &nodeObject,
                                                NodesSequence &nodesSequence,
                                                const NodesSequence *parentNodesSequence = nullptr,
                                                SequenceLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Get parent instance name from the JSON object given
            @param node The JSON object to parse
            @param parentName The parent name given
            @param errorHandler An optional sequence loading error handler pointer
            @return False if the parent instance name  attribute hasn't been found in the JSON
                          object*/
        static bool getParentInstanceName(const QJsonObject &node,
                                          QString &parentName,
                                          SequenceLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Get the type of the node
            @param node The JSON object to parse
            @param nodeType The node type got
            @param errorHandler An optional sequence loading error handler pointer
            @return False if the node type attribute hasn't been found */
        static bool getNodeType(const QJsonObject &node,
                                NodeType::Enum &nodeType,
                                SequenceLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Load the default information of a node
            @note - Get the unique instance name
                  - Get the description
            @param nodeObject The JSON object to parse
            @param node The node to fill
            @param errorHandler An optional sequence loading error handler pointer
            @return True if no problem occurs */
        static bool loadAbstractNode(const QJsonObject &nodeObject,
                                     ANode &node,
                                     SequenceLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Help to load a Sequence module node
            @note - Load the abstract informations (@see loadAbtractNode)
                  - Load and connect to input reference holder
                  - Load and connect to output reference holder
                  - Load all the informations link to the linked banj=k
                  - Load the node state (if it's enabled or not)
            @warning The references of the nodesSequence have to be loaded before to call this
                     function
            @param sequenceModuleNode The JSON Object to parse
            @param nodesSequence The NodesSequence to get the references from
            @param sequenceNode The sequence node to fill with infos got
            @param errorHandler An optional sequence loading error handler pointer
            @return True if no problem occurs */
        static bool loadSequenceModuleNode(const QJsonObject &sequenceModuleNode,
                                           const NodesSequence &nodesSequence,
                                           SequenceModuleNode &sequenceNode,
                                           SequenceLoadingErrorHandler *errorHandler =nullptr);

        /** @brief Load a constant node informations from JSON
            @param constantObject The JSON object to parse
            @param constantNode The constant node to fill with infos got
            @param errorHandler An optional sequence loading error handler pointer
            @return True if no problem occurs */
        static bool loadConstantNode(const QJsonObject &constantObject,
                                     ConstantNode &constantNode,
                                     SequenceLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Load a variable node informations from JSON
            @param variableObject The JSON object to parse
            @param variableNode The variable node to fill with infos got
            @param errorHandler An optional sequence loading error handler pointer
            @return True if no problem occurs */
        static bool loadVariableNode(const QJsonObject &variableObject,
                                     VariableNode &variableNode,
                                     SequenceLoadingErrorHandler *errorHandler = nullptr);

    private:
        /** @brief Help to load the shared attributes between a constant and variable reference
            @param referenceObject The JSON object to parse
            @param referenceNode The reference node to fill
            @param errorHandler An optional sequence loading error handler pointer
            @return True if no problem occurs */
        static bool loadReferenceNode(const QJsonObject &referenceObject,
                                      AReferenceNode &referenceNode,
                                      SequenceLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Parse and load all the attributes shared in a ANodePort object
            @param nodePortObject The JSON object to parse
            @param aNodePort The node port to fill
            @param errorHandler An optional sequence loading error handler pointer
            @return True if no problem occurs */
        static bool loadANodePort(const QJsonObject &nodePortObject,
                                  ANodePort &aNodePort,
                                  SequenceLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Load input references holder
            @param nodeObject The JSON object to parse
            @param referencesHolder The references holder to fill with input references
            @param internalNodesSequence The internal nodesSequence (of an inclusion or group node)
            @param parentNodesSequence The external nodesSequence (the one which is the parent of
                                       the current node)
            @param errorHandler An optional sequence loading error handler pointer
            @return True if no problem occurs */
        static bool loadInputReferencesHolder(const QJsonObject &nodeObject,
                                              ReferencesHolder &referencesHolder,
                                              const NodesSequence *internalNodesSequence = nullptr,
                                              const NodesSequence *parentNodesSequence = nullptr,
                                              SequenceLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Update the references holder with the InNodePort references given
            @note The method only updates the existing InNodePort with the internal and external
                  references, and only if in the current NodePort those elements are not set yet.
                  If other elements are differents or if the references are already set, the method
                  will return false
            @note If an InNodePort not already exists in the ReferencesHolder given, it will add it
            @param inPort The inPort to set in the referencesHolder
            @param referencesHolder The holder to fill
            @param errorHandler An optional sequence loading error handler pointer
            @return True if no problem occurs */
        static bool addOrUpdateInputReferences(const InNodePort &inPort,
                                               ReferencesHolder &referencesHolder,
                                               SequenceLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Load output references holder
            @param nodeObject The JSON object to parse
            @param referencesHolder The references holder to fill with output variables
            @param internalNodesSequence The internal nodesSequence (of an inclusion or group node)
            @param externalNodesSequence The external nodesSequence (the one which is the parent of
                                       the current node)
            @param errorHandler An optional sequence loading error handler pointer
            @return True if no problem occurs */
        static bool loadOutputReferencesHolder(
                                            const QJsonObject &nodeObject,
                                            ReferencesHolder &referencesHolder,
                                            const NodesSequence *internalNodesSequence = nullptr,
                                            const NodesSequence *externalNodesSequence = nullptr,
                                            SequenceLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Update the references holder with the OutNodePort references given
            @note The method only updates the existing OutNodePort with the internal and external
                  variables, and only if in the current NodePort those elements are not set yet.
                  If other elements are differents or if the variables are already set, the method
                  will return false
            @note If an OutNodePort not already exists in the ReferencesHolder given, it will add it
            @param outPort The outPort to set in the referencesHolder
            @param referencesHolder The holder to fill
            @param errorHandler An optional sequence loading error handler pointer
            @return True if no problem occurs */
        static bool addOrUpdateOutputVariables(const OutNodePort &outPort,
                                               ReferencesHolder &referencesHolder,
                                               SequenceLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Try to set the modules param given to the sequence module
            @param moduleParams The module parameters to set in the sequence module
            @param module The sequence module to fill with the parameters given
            @param errorHandler An optional sequence loading error handler pointer
            @return True if no problem occurs */
        static bool loadModuleParameters(const QJsonArray &moduleParams,
                                         ISequenceInstanceModule *&module,
                                         SequenceLoadingErrorHandler *errorHandler = nullptr);
};
