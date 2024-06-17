// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "sequence/corestatemachine/anodeport.hpp"

#include <QSharedPointer>

class AReferenceNode;


/** @brief The input node is used for representing an input of a node, it can point all types of
           reference node
    @note The InNodePort can have an InputNode and an Internal InputNode, thoses allow to have a
          link between a reference contains in a parent NodesSequence and an aonther one in a child
          NodesSequence */
class InNodePort : public ANodePort
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The parent class */
        explicit InNodePort(QObject *parent = nullptr);

        /** @brief Copy constructor
            @param inPort The object to copy*/
        InNodePort(const InNodePort &inPort);

        /** @brief Assignement operator, set the inPort value to this node
            @param inPort The port to get its infos
            @return This element modified */
        InNodePort &operator=(const InNodePort &inPort);

    public:
        /** @brief Set input node linked to this node port
            @param referenceNode The reference node to link with this port
            @param errorHandler An optional sequence loading error handler pointer
            @return True If the node is compatible */
        bool setInputNode(const QSharedPointer<AReferenceNode> &referenceNode,
                          SequenceLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Get input mode linked to this node port */
        const QSharedPointer<AReferenceNode> &getInputNode() const { return _inputNode; }

        /** @brief Set the internal input node linked to this node port */
        bool setInternalInputNode(const QSharedPointer<AReferenceNode> &internalInputNode);

        /** @brief Get the internal input node linked to this node port */
        const QSharedPointer<AReferenceNode> &getInternalInputNode() const
        { return _internalInputNode; }

        /** @brief Set if the InNodePort references can be not given */
        void setCanReferenceBeUndefined(bool canBeUndefined) { _canBeUndefined = canBeUndefined; }

        /** @brief Say if the the references can be undefined */
        bool isReferenceCanBeUndefined() const { return _canBeUndefined; }

        /** @brief Equality operator, test if the other InNodePort is equal to this one
            @param nodePort The other InNodePort to test with
            @return True if the two InNodePort are equals */
        bool operator==(const InNodePort &nodePort) const;

    private:
        QSharedPointer<AReferenceNode> _inputNode;
        QSharedPointer<AReferenceNode> _internalInputNode;
        bool _canBeUndefined{false};
};

/** @brief Return a qHash for InNodePort
    @param number The class to get the qHash from
    @param seed The seed to add in the qHash
    @return The hash created */
inline uint qHash(const InNodePort &inNodePort, uint seed)
{
    return qHash(inNodePort.getInputNode(), seed) ^
           qHash(inNodePort.getInternalInputNode(), seed) ^
           qHash(inNodePort.isReferenceCanBeUndefined(), seed);
}
