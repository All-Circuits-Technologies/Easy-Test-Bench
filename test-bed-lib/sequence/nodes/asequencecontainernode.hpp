// SPDX-FileCopyrightText: 2020 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "sequence/nodes/atargetingnode.hpp"

#include "testbedcore/types/sequencestatetype.hpp"
#include "testbedcore/types/stopaskedtype.hpp"


/** @brief Abstract node which represents all the nodes which includes child NodesSequence
    @note This allow to reverberate errors or stop asked by user */
class ASequenceContainerNode : public ATargetingNode
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param nodeType The type of node
            @param sequenceParent The NodesSequence parent of this node
            @param parent The class parent */
        explicit ASequenceContainerNode(NodeType::Enum nodeType,
                                        const QSharedPointer<NodesSequence> &sequenceParent,
                                        QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~ASequenceContainerNode() override = default;

    public:
        /** @brief Set the behavior to apply when the child sequence fails
            @note If the sequence fails and if:
                    * the param is set to true, the parent sequence will be ended in error.
                    * the param is set to false, the parent sequence won't be ended in error and
                           (if the success output is given) the output success variable will be set
                           to false.
                  If the sequence is successful and the success output has been given, it will be
                  set to true */
        void setFailIfAnErrorOccured(bool failIfAnErrorOccured)
        { _failIfAnErrorOccured = failIfAnErrorOccured; }

        /** @brief Say if the sequence parent will be ended in error, if the child sequence has
                   failed
            @see ASequenceContainerNode::setFailIfAnErrorOccured */
        bool isFailingIfAnErrorOccured() const { return _failIfAnErrorOccured; }

    public:
        /** @brief Get the child nodes sequence linked to the container node
            @note The nodes sequence can be null if it hasn't already been loaded */
        virtual QSharedPointer<NodesSequence> getChildNodesSequence() const = 0;

    protected slots:
        /** @brief Called when the child nodes sequence has finished
            @param success True if the child nodes sequence has ended successfuly */
        void onChildNodesSequenceFinished(bool success);

    protected slots:
        /** @see ASequenceNode::nodeActivity */
        virtual void nodeActivity() override;

        /** @see ANode::forgetAllAboutIt */
        virtual void forgetAllAboutIt(const QSharedPointer<ANode> &nodeToDetachWith) override;

    protected:
        /** @brief Manage the connection and disconnection between the child nodes sequence signals
                   and this object
            @param connectNode True to connect to the child node sequence signals,
                               False to disconnect
            @param childNode The child node sequence to attach with */
        void manageConnectionToSequence(bool connectNode,
                                        const QSharedPointer<NodesSequence> &childNode);

        /** @brief Deal with the special output writing (some external outputs aren't linked with
                   internal references). This method writes them.
            @see JsonParsersConstants::Sequence::sequenceSuccessOutput
            @param childNode The nodes sequence child linked to the container
            @param childSequenceSuccess Say if the child sequence has ended in success
            @param error The error detail if an error occurred
            @return True if no error occurs */
        bool dealWithOutputWriting(const QSharedPointer<NodesSequence> &childNode,
                                   bool childSequenceSuccess,
                                   QString &error);

    protected:
        /** @brief Called in the @ref ASequenceContainerNode::nodeActivity method to prepare the
                   sequence before starting it
            @param error The error detail if an error occurred
            @return True if no error occurs */
        virtual bool preparingSequenceBeforeStart(QString &error);

        /** @brief Same as forgetAllAboutIt but manages the case where the node to detach is this
                   instance
            @see ANode::forgetAllAboutIt */
        virtual void forgetAllAboutMe() = 0;

    private slots:
        /** @brief Called when the sequence state change
            @param newState The current state
            @param previousState The previous state before the current one*/
        void onNewState(SequenceStateType::Enum newState, SequenceStateType::Enum previousState);

        /** @brief Called when a stop has been asked for the sequence
            @param stopAskedType Give a contact to the stop asking */
        void onStopAsked(StopAskedType::Enum stopAskedType);

    private:
        bool _failIfAnErrorOccured{true};
};
