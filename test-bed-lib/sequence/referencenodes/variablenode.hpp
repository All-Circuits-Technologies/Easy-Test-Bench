// SPDX-FileCopyrightText: 2019 - 2020 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "sequence/referencenodes/areferencenode.hpp"

class ASequenceContainerNodeKey;
class SequenceModuleNodeKey;
class NodesSequenceKey;


/** @brief Represents a variable node, the value of a variable node is set at runtime */
class VariableNode : public AReferenceNode
{
    Q_OBJECT

    public:
        /** @brief Class constuctor
            @param sequenceParent The NodesSequence parent of this node
            @param parent The class parent */
        explicit VariableNode(const QSharedPointer<NodesSequence> &sequenceParent,
                              QObject *parent = nullptr);

        /** @brief Set the value of variable
            @note Call only be called by a SequenceModule instance
            @param key The key only allowes authorized class instance to call this method
            @param value The value to set
            @return True if no problem occurs */
        bool setVariableValue(const SequenceModuleNodeKey &key, const QVariant &value);

        /** @brief Set the value of variable
            @note Only be called by a NodesSequence instance
            @param key The key only allowes authorized class instance to call this method
            @param value The value to set
            @return True if no problem occurs */
        bool setVariableValue(const NodesSequenceKey &key, const QVariant &value);

        /** @brief Set the value of a variable
            @note Only be called by a sequence container node instance
            @param key The key only allowes authorized class instance to call this method
            @param value The value to set
            @return True if no problem occurs */
        bool setVariableValue(const ASequenceContainerNodeKey &key, const QVariant &value);
};
