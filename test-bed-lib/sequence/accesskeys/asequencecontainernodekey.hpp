// SPDX-FileCopyrightText: 2020 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once


/** @brief The key can be used by an ASequenceContainerNode object and it allowes to create public
           methods only callable by this kind of nodes */
class ASequenceContainerNodeKey
{
    friend class ASequenceContainerNode;
    friend class GroupNode;
    friend class InclusionNode;

    private:
        /** @brief Private class construtor */
        ASequenceContainerNodeKey() {}
};
