// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QSharedPointer>

#include "sequence/corestatemachine/nodetype.hpp"

class NodesSequence;
class QAbstractState;
class SequenceLogData;


/** @brief The abstract base for all the nodes */
class ANode : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param nodeType The type of node
            @param sequenceParent The NodesSequence parent of this node
            @param parent The class parent */
        explicit ANode(NodeType::Enum nodeType,
                       const QSharedPointer<NodesSequence> &sequenceParent,
                       QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~ANode() override;

    public:
        /** @brief Get the node instance name */
        const QString &getInstanceName() const { return _instanceName; }

        /** @brief Set the node instance name */
        void setInstanceName(const QString &instanceName) { _instanceName = instanceName; }

        /** @brief Get the node description */
        const QString &getDescription() const { return _description; }

        /** @brief Set the node description */
        void setDescription(const QString &description) { _description = description; }

        /** @brief Get the node instance parent  */
        const QSharedPointer<NodesSequence> &getInstanceParent() const { return _parent; }

        /** @brief Get the node type */
        NodeType::Enum getNodeType() const { return _nodeType; }

    protected:
        /** @brief Access instance parent */
        QSharedPointer<NodesSequence> &accessInstanceParent() { return _parent; }

    protected slots:
        /** @brief This method is called when we want to delete a node. Because we are working with
                   QSharedPointer, we need to delete all the references to the node in order to
                   delete it.
                   Therefore, in this method, if we know the node (because we keep it somewhere)
                   we release all the references to it.
                   It also works for itself: if the node to detach with is this node, it has to
                   prepare itself to be deleted
            @param nodeToDetachWith The node to detache with */
        virtual void forgetAllAboutIt(const QSharedPointer<ANode> &nodeToDetachWith);

    signals:
        /** @brief Emit to save a new log for the sequence
            @param newLog The new log to store */
        void newLog(const SequenceLogData &newLog);

    private:
        QString _instanceName;
        QString _description{""};
        NodeType::Enum _nodeType{NodeType::UnknownNode};
        QSharedPointer<NodesSequence> _parent;
};
