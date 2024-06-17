// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "errors/helpers/fileloadingerrorhelper.hpp"

#include "test-bed-lib/sequence/corestatemachine/nodetype.hpp"

class FileLoadingErrorHandler;


/** @brief Class providing static methods to handle errors related to sequence nodes */
class SequenceLoadingErrorHelper : public FileLoadingErrorHelper
{
    public:
        /** @brief Used to add an error when a nodes sequence cannot be load
            @param errorHandler An ErrorHandler pointer
            @param nodeName The name of the nodes sequence */
        static void addFailedNodesSequenceError(FileLoadingErrorHandler *errorHandler,
                                                const QString &nodeName = QString(""));

        /** @brief Used to add an error when a node cannot be read
            @param nodeName The name of the node
            @param nodeType The node type
            @param errorHandler An ErrorHandler pointer */
        static void addFailedChildNodeError(const QString &nodeName,
                                            NodeType::Enum nodeType,
                                            FileLoadingErrorHandler *errorHandler);
};
