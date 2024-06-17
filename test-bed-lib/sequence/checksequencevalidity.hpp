// SPDX-FileCopyrightText: 2019 - 2020 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QSharedPointer>

#include "jsonutility/jsontypedefs.hpp"

class ANode;
class ASequenceContainerNode;
class ASequenceNode;
class DecisionalNode;
class ForkNode;
class GroupNode;
class InclusionNode;
class JoinNode;
class NodesSequence;
class OutNodePort;
class ReferencesHolder;
class SequenceModuleNode;


/** @brief Check all sequence validity in order to know if the sequence can be launched */
class CheckSequenceValidity
{
    private:
        /** @brief Private constructor */
        explicit CheckSequenceValidity() {}

    public:
        /** @brief Test if the node sequence given and all its children are valids
            @param nodeSequence The node sequence to test
            @param error When an error is detected, this string is filled with the error detail.
                         The same error is also printed in the console
            @param nodeWithError When an error is detected , this reference is equals to the
                                 problematic node in this sequence.
            @return True If the sequence is valid and can be launched */
        static bool isValid(const QSharedPointer<NodesSequence> &nodeSequence,
                            QString &error,
                            QSharedPointer<ANode> &nodeWithError);

        /** @brief Test if the node given is valid
            @param node The node to test
            @param error When an error is detected, this string is filled with the error detail.
                         The same error is also printed in the console
            @return True If the node is valid */
        static bool isValid(const QSharedPointer<ASequenceNode> &node,
                            QString &error);

        /** @brief Test if the references holder given is valid
            @param refHolder The reference holder to test
            @param linkedNode The node linked to the reference holder
            @param error When an error is detected, this string is filled with the error detail.
                         The same error is also printed in the console
            @return True If the node is valid */
        static bool isValid(const ReferencesHolder &refHolder,
                            const QSharedPointer<ANode> &linkedNode,
                            QString &error);

        /** @brief Some specific NodesSequence outputs can be exempted of internal bindings
            @param output The output node port to test
            @return True if the output node port can be exempted to not have internal bindings */
        static bool isOutputNodeCanHaveNoInternalBinding(const OutNodePort &output);

    private:
        /** @brief This method test if the node is targeted (and the number respects the limits) and
                   if it targets elements
            @param node The node to test
            @param error When an error is detected, this string is filled with the error detail.
                         The same error is also printed in the console
            @return True If the node is valid */
        static bool testIfLinkedWithOtherNodes(const QSharedPointer<ASequenceNode> &node,
                                               QString &error);

        /** @brief Test if the JoinNode is valid
            @param node The node to test
            @param error When an error is detected, this string is filled with the error detail.
                         The same error is also printed in the console
            @return True If the node is valid */
        static bool isJoinNodeValid(const QSharedPointer<JoinNode> &node,
                                    QString &error);

        /** @brief Test if the ForkNode is valid
            @param node The node to test
            @param error When an error is detected, this string is filled with the error detail.
                         The same error is also printed in the console
            @return True If the node is valid */
        static bool isForkNodeValid(const QSharedPointer<ForkNode> &node,
                                    QString &error);

        /** @brief Test if the ASequenceContainerNode is valid
            @param node The node to test
            @param childNodesSeq The child node linked to the sequence container
            @param error When an error is detected, this string is filled with the error detail.
                         The same error is also printed in the console
            @return True If the node is valid */
        static bool isASequenceContainerNodeValid(
                                                const QSharedPointer<ASequenceContainerNode> &node,
                                                const QSharedPointer<NodesSequence> &childNodesSeq,
                                                QString &error);

        /** @brief Test if the GroupNode is valid
            @param node The node to test
            @param error When an error is detected, this string is filled with the error detail.
                         The same error is also printed in the console
            @return True If the node is valid */
        static bool isGroupNodeValid(const QSharedPointer<GroupNode> &node,
                                     QString &error);

        /** @brief Test if the InclusionNode is valid
            @param node The node to test
            @param error When an error is detected, this string is filled with the error detail.
                         The same error is also printed in the console
            @return True If the node is valid */
        static bool isInclusionNodeValid(const QSharedPointer<InclusionNode> &node,
                                         QString &error);

        /** @brief Test if the SequenceModuleNode is valid
            @param node The node to test
            @param error When an error is detected, this string is filled with the error detail.
                         The same error is also printed in the console
            @return True If the node is valid */
        static bool isSequenceModuleNodeValid(const QSharedPointer<SequenceModuleNode> &node,
                                              QString &error);

        /** @brief Test if the DecisionalNode is valid
            @param node The node to test
            @param error When an error is detected, this string is filled with the error detail.
                         The same error is also printed in the console
            @return True If the node is valid */
        static bool isDecisionnalNodeValid(const QSharedPointer<DecisionalNode> &node,
                                           QString &error);

        /** @brief Test if the DecisionalNode transition is valid
            @param transition The JsonObject representation of the transition, see:
                               @ref SequenceFileJsonParser::loadDecisionalTransition to have the
                               details of what the object contains
            @param decisionalName The name of the decisional node link to the transition
            @param error When an error is detected, this string is filled with the error detail.
                         The same error is also printed in the console
            @return True If the node is valid */
        static bool isDecisionalTransitionValid(const JsonObject &transition,
                                                const QString &decisionalName,
                                                QString &error);
};
