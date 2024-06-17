// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "sequence/corestatemachine/anodeport.hpp"

#include <QSharedPointer>

class VariableNode;


/** @brief The output node is used for represents an output of a node, it can only be linked to
           variable node
    @note The OutNodePort can have an OutputNode and an Internal OutputNode, thoses allow to have a
          link between a variable contains in a parent NodesSequence and an aonther one in a child
          NodesSequence */
class OutNodePort : public ANodePort
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The parent class */
        explicit OutNodePort(QObject *parent = nullptr);

        /** @brief Copy constructor
            @param outPort The object to copy */
        OutNodePort(const OutNodePort &outPort);

        /** @brief Assignement operator, set the outPort value to this node
            @param outPort The port to get its infos
            @return This element modified */
        OutNodePort &operator=(const OutNodePort &outPort);

        /** @brief Set output node linked to this node port
            @param variableNode The variable node to link with this port
            @return True If no problem occurred and if the node are compatible */
        bool setOutputNode(const QSharedPointer<VariableNode> &variableNode);

        /** @brief Get output node linked to this node port */
        const QSharedPointer<VariableNode> &getOutputNode() const { return _outputNode; }

        /** @brief Set the internal output node linked to this node port */
        bool setInternalOutputNode(const QSharedPointer<VariableNode> &internalOutputNode);

        /** @brief Get the internal output node linked to this node port */
        const QSharedPointer<VariableNode> &getInternalOutputNode() const
        { return _internalOutputNode; }

        /** @brief Equality operator, test if the other OutNodePort is equal to this one
            @param nodePort The other OutNodePort to test with
            @return True if the two OutNodePort are equals */
        bool operator==(const OutNodePort &nodePort) const;

    private:
        QSharedPointer<VariableNode> _outputNode;
        QSharedPointer<VariableNode> _internalOutputNode;
};

/** @brief Return a qHash for OutNodePort
    @param number The class to get the qHash from
    @param seed The seed to add in the qHash
    @return The hash created */
inline uint qHash(const OutNodePort &inNodePort, uint seed)
{
    return qHash(inNodePort.getOutputNode(), seed) ^
           qHash(inNodePort.getInternalOutputNode(), seed);
}
