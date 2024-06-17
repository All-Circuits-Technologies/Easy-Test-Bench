// SPDX-FileCopyrightText: 2019 - 2020 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once


/** @brief The Key can be used by any ASequenceNode and it allowes to create public methods only
           callable by those nodes */
class ASequenceNodeKey
{
    friend class ASequenceContainerNode;
    friend class ASequenceNode;
    friend class ATargetingNode;
    friend class FinalNode;
    friend class DecisionalNode;
    friend class ForkNode;
    friend class GroupNode;
    friend class InclusionNode;
    friend class InitialNode;
    friend class JoinNode;
    friend class MergeNode;
    friend class SequenceModuleNode;

    private:
        /** @brief Private class construtor */
        ASequenceNodeKey() {}
};
