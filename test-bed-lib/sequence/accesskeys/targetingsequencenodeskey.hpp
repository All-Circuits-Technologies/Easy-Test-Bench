// SPDX-FileCopyrightText: 2019 - 2020 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once


/** @brief The Key can be used by any nodes which targets others and it allowes to create public
           methods only callable by those nodes
    @note Differs to the @ref ASequenceNodeKey because the FinalNode is not a targeting node */
class TargetingSequenceNodesKey
{
    friend class ASequenceContainerNode;
    friend class ATargetingNode;
    friend class DecisionalNode;
    friend class ForkNode;
    friend class GroupNode;
    friend class InitialNode;
    friend class InclusionNode;
    friend class JoinNode;
    friend class MergeNode;
    friend class SequenceModuleNode;

    private:
        /** @brief Private class construtor */
        TargetingSequenceNodesKey() {}
};
