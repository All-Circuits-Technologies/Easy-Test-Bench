// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QSharedPointer>

#include "sequence/corestatemachine/targettype.hpp"

class ASequenceNode;


/** @brief Helps to manage the sequence nodes, to test them and analyse how they are built */
class SequenceNodesHelper
{
    public:
        /** @brief Deleted constructor */
        SequenceNodesHelper() = delete;

    public:
        /** @brief Test if a node targets an another one
            @param nodeTargeted The node targeted to test
            @param potentiallyTargetingNode The node which is potentially targeting the previous one
            @return True If the second node targets the first one */
        static bool isTargetingNode(const QSharedPointer<ASequenceNode> &nodeTargeted,
                                    const QSharedPointer<ASequenceNode> &potentiallyTargetingNode);

        /** @brief Verify if its valid to target this node
            @param node The node to test
            @return True if it's valid to target it */
        static bool verifyIfItsValidToTargetThisNode(const QSharedPointer<ASequenceNode> &node);

        /** @brief Get the way the node is currently targetted (by multiple, one or no node)
            @param node The node to test and get targetting type from
            @return The way the node is currently targetted */
        static TargetType::Enum getCurrentNumberOfTargeters(
                                                        const QSharedPointer<ASequenceNode> &node);
};
