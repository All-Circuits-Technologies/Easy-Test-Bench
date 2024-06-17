// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once


/** @brief The Key can be used by a JoinNode object and it allowes to create public methods only
           callable by this kind of node */
class JoinNodeKey
{
    friend class JoinNode;

    private:
        /** @brief Private class construtor */
        JoinNodeKey() {}
};
