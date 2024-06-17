// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


/** @brief The different sequence node type */
class NodeType : public QObject
{
    Q_OBJECT

    public:
        enum Enum
        {
            NodesSequence,
            InitialNode,
            JoinNode,
            MergeNode,
            FinalNode,
            SequenceModuleNode,
            VariableNode,
            ConstantNode,
            ForkNode,
            DecisionalNode,
            GroupNode,
            InclusionNode,
            UnknownNode
        };
        Q_ENUM(Enum)

    public:
        /** @brief Parse the enum from string
            @param strValue The string to parse
            @return The enum parsed */
        static Enum parseFromString(const QString &strValue);

        /** @brief Get the string representation of the enum given
            @param value The value to stringify
            @return The string representation of the enum value */
        static QString toString(Enum value);
};
