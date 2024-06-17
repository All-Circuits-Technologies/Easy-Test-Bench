// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "jsonutility/jsontypedefs.hpp"

class ANode;
class ANodePort;
class AReferenceNode;
class ATargetingNode;
class ConstantNode;
class DecisionalNode;
class FinalNode;
class ForkNode;
class GroupNode;
class InclusionNode;
class InitialNode;
class InNodePort;
class JoinNode;
class MergeNode;
class NodesSequence;
class OutNodePort;
class ReferencesHolder;
class SequenceFile;
class SequenceModuleNode;
class VariableNode;


/** @brief Useful methods to transform a ANode object to a JsonObject or JsonArray */
class NodesToJsonParser
{
    private:
        explicit NodesToJsonParser() { }

    public:
        /** @brief Transform the nodesSequences of a sequence file and all its children to a
                   JsonArray
            @param sequenceFile The sequenceFile to parse and get NodesSequence from in order to
                                change nodes to JsonArray
            @param jsonArray The elements parsed will be added to this JsonArray
            @return True if no problem occurs */
        static bool sequenceFileToJsonArray(const SequenceFile &sequenceFile,
                                            JsonArray &jsonArray);

        /** @brief Transform a NodesSequence to a JsonObject
            @param nodesSequence The node to transform
            @param jsonObject The JsonObject parsed
            @return True if no problem occurs */
        static bool nodesSequenceToJsonObject(const NodesSequence &nodesSequence,
                                              JsonObject &jsonObject);

        /** @brief Transform an InitialNode to a JsonObject
            @param initialNode The node to transform
            @param jsonObject The JsonObject parsed
            @return True if no problem occurs */
        static bool initialNodeToJsonObject(const InitialNode &initialNode,
                                            JsonObject &jsonObject);

        /** @brief Transform a JoinNode to a JsonObject
            @param joinNode The node to transform
            @param jsonObject The JsonObject parsed
            @return True if no problem occurs */
        static bool joinNodeToJsonObject(const JoinNode &joinNode,
                                         JsonObject &jsonObject);

        /** @brief Transform a MergeNode to a JsonObject
            @param mergeNode The node to transform
            @param jsonObject The JsonObject parsed
            @return True if no problem occurs */
        static bool mergeNodeToJsonObject(const MergeNode &mergeNode,
                                          JsonObject &jsonObject);

        /** @brief Transform a FinalNode to a JsonObject
            @param finalNode The node to transform
            @param jsonObject The JsonObject parsed
            @return True if no problem occurs */
        static bool finalNodeToJsonObject(const FinalNode &finalNode,
                                          JsonObject &jsonObject);

        /** @brief Transform a SequenceModuleNode to a JsonObject
            @param sequenceModuleNode The node to transform
            @param jsonObject The JsonObject parsed
            @return True if no problem occurs */
        static bool sequenceModuleNodeToJsonObject(const SequenceModuleNode &sequenceModuleNode,
                                                   JsonObject &jsonObject);

        /** @brief Transform the inputs of a SequenceModuleNode to a JsonArray
            @param sequenceModuleNode The node to get inputs from
            @param jsonArray The JsonArray parsed
            @return True if no problem occurs */
        static bool sequenceModuleInputValuesToJsonArray(
                                                    const SequenceModuleNode &sequenceModuleNode,
                                                    JsonArray &jsonArray);

        /** @brief Transform a VariableNode to a JsonObject
            @param variableNode The node to transform
            @param jsonObject The JsonObject parsed
            @return True if no problem occurs */
        static bool variableNodeToJsonObject(const VariableNode &variableNode,
                                             JsonObject &jsonObject);

        /** @brief Transform a ConstantNode to a JsonObject
            @param constantNode The node to transform
            @param jsonObject The JsonObject parsed
            @return True if no problem occurs */
        static bool constantNodeToJsonObject(const ConstantNode &constantNode,
                                             JsonObject &jsonObject);

        /** @brief Transform a ForkNode to a JsonObject
            @param forkNode The node to transform
            @param jsonObject The JsonObject parsed
            @return True if no problem occurs */
        static bool forkNodeToJsonObject(const ForkNode &forkNode,
                                         JsonObject &jsonObject);

        /** @brief Transform a DecisionalNode to a JsonObject
            @param decisionalNode The node to transform
            @param jsonObject The JsonObject parsed
            @return True if no problem occurs */
        static bool decisionalNodeToJsonObject(const DecisionalNode &decisionalNode,
                                               JsonObject &jsonObject);

        /** @brief Transform a GroupNode to a JsonObject
            @param groupNode The node to transform
            @param jsonObject The JsonObject parsed
            @return True if no problem occurs */
        static bool groupNodeToJsonObject(const GroupNode &groupNode,
                                          JsonObject &jsonObject);

        /** @brief Transform a InclusionNode to a JsonObject
            @param inclusionNode The node to transform
            @param jsonObject The JsonObject parsed
            @return True if no problem occurs */
        static bool inclusionNodeToJsonObject(const InclusionNode &inclusionNode,
                                              JsonObject &jsonObject);

    private:
        /** @brief Transform the abstract informations of a ANode to a JsonObject
            @param nodeObject The ANode to parse
            @param jsonObject The JsonObject to fill with information got from the ANode
            @return True if no problem occurs */
        static bool aNodeToJsonObject(const ANode &nodeObject,
                                      JsonObject &jsonObject);

        /** @brief Transform the informations from a ReferencesHolder to a JsonObject
            @param referencesHolder The ReferencesHolder to parse
            @param jsonObject The JsonObject to fill with information got from the
                   ReferencesHolder
            @return True if no problem occurs */
        static bool referencesHolderToJsonObject(const ReferencesHolder &referencesHolder,
                                                 JsonObject &jsonObject);

        /** @brief Transform the informations from a InNodePort to a JsonObject
            @param inNodePort The InNodePort to parse
            @param jsonObject The JsonObject to fill with information got from the InNodePort
            @return True if no problem occurs */
        static bool inputNodePortToJsonObject(const InNodePort &inNodePort,
                                              JsonObject &jsonObject);

        /** @brief Transform the informations from a OutNodePort to a JsonObject
            @param outNodePort The OutNodePort to parse
            @param jsonObject The JsonObject to fill with information got from the OutNodePort
            @return True if no problem occurs */
        static bool outputNodePortToJsonObject(const OutNodePort &outNodePort,
                                               JsonObject &jsonObject);

        /** @brief Transform the abstracts informations from a ANodePort to a JsonObject
            @param aNodePort The ANodePort to parse
            @param jsonObject The JsonObject to fill with information got from the ANodePort
            @return True if no problem occurs */
        static bool aNodePortToJsonObject(const ANodePort &aNodePort,
                                          JsonObject &jsonObject);

        /** @brief Transform the abstract informations of a ATargetingNode to a JsonObject
            @param targetingNode The ATargetingNode to parse
            @param jsonObject The JsonObject to fill with information got from the ATargetingNode
            @return True if no problem occurs */
        static bool targetingNodeToJsonObject(const ATargetingNode &targetingNode,
                                              JsonObject &jsonObject);

        /** @brief Transform the abstract informations of a AReferenceNode to a JsonObject
            @param refNode The AReferenceNode to parse
            @param jsonObject The JsonObject to fill with information got from the AReferenceNode
            @return True if no problem occurs */
        static bool aReferenceNodeToJsonObject(const AReferenceNode &refNode,
                                               JsonObject &jsonObject);

        /** @brief Transform a NodesSequence and all its children to JsonObjects and append them to
                   the JsonArray given
            @param nodesSequence The nodesSequence to transform
            @param jsonArray The JsonArray to fill with the informations
            @return True if no problem occurs */
        static bool loadNodesSequenceAndChildrenToJsonObject(const NodesSequence &nodesSequence,
                                                             JsonArray &jsonArray);

        /** @brief Transform all the children of a NodesSequence to JsonObjects and append them to
                   the JsonArray given
            @param nodesSequence The nodesSequence to get the children from and transform them to
                                 JsonObjects
            @param jsonArray The JsonArray to fill with the informations
            @return True if no problem occurs */
        static bool loadChildrenToJsonObject(const NodesSequence &nodesSequence,
                                             JsonArray &jsonArray);
};
