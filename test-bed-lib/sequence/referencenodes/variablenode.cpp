// SPDX-FileCopyrightText: 2019 - 2020 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "variablenode.hpp"


VariableNode::VariableNode(const QSharedPointer<NodesSequence> &sequenceParent, QObject *parent) :
    AReferenceNode(NodeType::VariableNode, sequenceParent, parent)
{
}

bool VariableNode::setVariableValue(const SequenceModuleNodeKey &key, const QVariant &value)
{
    Q_UNUSED(key)

    return setValue(value);
}

bool VariableNode::setVariableValue(const NodesSequenceKey &key, const QVariant &value)
{
    Q_UNUSED(key)

    return setValue(value);
}

bool VariableNode::setVariableValue(const ASequenceContainerNodeKey &key, const QVariant &value)
{
    Q_UNUSED(key)

    return setValue(value);
}
