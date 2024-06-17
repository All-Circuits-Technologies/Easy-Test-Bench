// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "sequence/referencenodes/areferencenode.hpp"

class SequenceLoadingErrorHandler;


/** @brief Represents a constant node, the value of a constant node can't be set at runtime, only
           when designing the diagram */
class ConstantNode : public AReferenceNode
{
    Q_OBJECT

    public:
        /** @brief Class constuctor
            @param sequenceParent The NodesSequence parent of this node
            @param parent The class parent */
        explicit ConstantNode(const QSharedPointer<NodesSequence> &sequenceParent,
                              QObject *parent = nullptr);

        /** @brief Set the constant value, can't be called at runtime
            @param value The value to set
            @param errorHandler An optional sequence loading error handler pointer
            @return True if no problem occurs */
        bool setConstantValue(const QVariant &value,
                              SequenceLoadingErrorHandler *erroHandler = nullptr);
};
