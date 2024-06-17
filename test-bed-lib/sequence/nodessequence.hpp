// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "sequence/nodes/anode.hpp"

#include <QSharedPointer>
#include <QHash>

#include "sequence/corestatemachine/referencesholder.hpp"
#include "sequence/logs/sequencelogmanager.hpp"
#include "testbedcore/utility/sequencestatehelper.hpp"

class ASequenceNodeKey;
class ASequenceNode;
class QStateMachine;


/** @brief The nodes sequence which contains all the nodes of a sequence */
class NodesSequence : public ANode
{
    Q_OBJECT

    private:
        /** @brief Class constructor
            @param parent The parent class */
        explicit NodesSequence(QObject *parent = nullptr);

    public:
        /** @brief Class destructor */
        virtual ~NodesSequence() override;

        /** @brief This static method allowes to create a NodesSequence
            @return A new instance of the NodesSequence */
        static QSharedPointer<NodesSequence> createNodesSequence();

    //****************************************************************************************//
    //*                                   Manage sequence life                               *//
    //****************************************************************************************//
    public:
        /** @brief Start the sequence
            @note Before to launch the sequence, the sequence is checked and verified
            @return True If no problem occurs */
        bool startSequence();

        /** @brief Stop the sequence
            @note The method will try to quickly stop the sequence
            @note When the sequence is stopped, because we don't finish the sequence until its end,
                  the sequence returns in error.
            @param stopAskedType Give a contact to the stop asking
            @return True If no problem occurs */
        bool stopSequence(StopAskedType::Enum stopAskedType);

        /** @brief Manage the pausing of the sequence
            @param setToPause - If true, the sequence will try to pause
                              - If false, the sequence will try to resume (if the sequence is in
                                pause) the sequence (if the sequence is already running, nothing is
                                done)
            @return True if no problem occurs */
        bool managePauseAndResume(bool setToPause);

        /** @brief Test if the sequence running
            @return True if the sequence is running */
        bool isSequenceRunning() const { return _sequenceState.isRunning(); }

        /** @brief Get the string representation of the error which occurred in the sequence */
        const QString &getSequenceStringError() const { return _errorString; }

        /** @brief Get the node which has  */
        const QSharedPointer<ANode> &getGuiltyNode() const { return _guiltyNode; }

    //****************************************************************************************//
    //*                               Manage sequence elements                               *//
    //****************************************************************************************//
    public:
        /** @brief Add a node to the sequence
            @param node The sequence node to add */
        void addSequenceNode(const QSharedPointer<ASequenceNode> &node);

        /** @brief Remove the sequence node given from the NocesSequence
            @param node The node to remove */
        void removeSequenceNode(const QSharedPointer<ASequenceNode> &node);

        /** @brief Add a reference node to this NodesSequence
            @param referenceNode The reference node to add */
        void addReferenceNode(const QSharedPointer<AReferenceNode> &referenceNode);

        /** @brief Remove a reference node from this NodesSequence
            @param referenceNode The reference node to remove */
        void removeReferenceNode(const QSharedPointer<AReferenceNode> &referenceNode);

        /** @brief Get a node thanks to its instance name
            @param uniqueName The name of the node to search */
        QSharedPointer<ASequenceNode> getSequenceNode(const QString &uniqueName) const;

        /** @brief Get the sequence nodes name */
        QVector<QString> getSequenceNodesName() const;

        /** @brief Get a reference node by its name
            @param uniqueName The name of the reference node to find
            @return The reference node found or NULL */
        QSharedPointer<AReferenceNode> getReferenceNode(const QString &uniqueName) const;

        /** @brief Get a variable node by its name
            @param uniqueName The name of the variable node to find
            @return The variable node found or NULL */
        QSharedPointer<VariableNode> getVariableNode(const QString &uniqueName) const;

        /** @brief Unload all the elements of the NodesSequence */
        void unloadAll();

    //****************************************************************************************//
    //*                                     Global accessors                                 *//
    //****************************************************************************************//
    public:
        /** @brief Access to the references holder of the node */
        ReferencesHolder &accessReferencesHolder() { return _referencesHolder; }

        /** @brief Get the references holder of the node */
        const ReferencesHolder &getReferencesHolder() const { return _referencesHolder; }

        /** @brief Access the state machine linked to NodesSequence
            @param seqNodeKey The key only allowes authorized class instance to call this method */
        const QSharedPointer<QStateMachine> &accessStateMachine(const ASequenceNodeKey &seqNodeKey);

        /** @brief Access the state machine linked to NodesSequence */
        QSharedPointer<const QStateMachine> getStateMachine() const;

       /** @brief Get the sequence nodes of this NodesSequence */
        QVector<QSharedPointer<ASequenceNode> > getSequenceNodes() const;

        /** @brief Get the reference nodes of this NodesSequence */
        QVector<QSharedPointer<AReferenceNode>> getReferenceNodes() const;

        /** @brief Get the current state of the sequence */
        const SequenceStateHelper &getSequenceState() const { return _sequenceState; }

        /** @brief Get the unique token linked to the sequence */
        const QString &getUniqueToken() const { return _uniqueToken; }

        /** @brief Get the sequence log manager linked to the nodes sequence */
        const SequenceLogManager &getSequenceLogManager() const { return _sequenceLogManager; }

        /** @brief Search in all the containers nodes a nodes sequences with the unique token given
            @param uniqueToken The nodes sequence unique token to find
            @return The nodes sequence found or a nullptr */
        QSharedPointer<NodesSequence> findAChildNodesSequence(const QString &uniqueToken) const;

    private slots:
        /** @brief Called when the QStateMachine linked to this NodesSequence is finished */
        void onStateMachineFinished();

        /** @brief Called when the sequence exits in error
            @param guiltyNode The node which causes the error
            @param error The string information of the error */
        void onSequenceError(const ANode &guiltyNode, const QString &error);

    signals:
        /** @brief Emitted when the sequence has finished
            @note If an error occurred, it's possible to get the detail of the error by calling:
                  @ref getSequenceStringError or @ref getGuiltyNode
            @param success True if the sequence ended successfully */
        void finished(bool success);

        /** @brief This signal is emitted when we want to delete a node. Because we are working with
                   QSharedPointer, we need to delete all the references to the node in order to
                   delete it.
            @param nodeToDetachWith The node to detache with */
        void hasToDie(const QSharedPointer<ANode> &nodeToDetachWith);

    private:
        /** @brief This method helps to find the QSharedPointerof a node kept here thanks to its
                   content
            @param node The node to get its QSharedPointer representation
            @return The QSharedPointer of the node */
        QSharedPointer<ANode> findGuiltyNode(const ANode &node);

        /** @brief This method applies the external reference values to the internal references node
            @note If we are using a GroupNode and we have references nodes linked to the GroupNode
                  inputs, this method helps to give the value of the external references to the
                  internal reference of the GroupNode
            @return True if no problem occurs */
        bool applyExternalRefValuesToInternal();

        /** @brief This method applies the internal variable values to the external variable node
            @note If we are using a GroupNode and we have variable nodes linked to the GroupNode
                  outputs, this method helps to give the value from internal variable to the
                  external variable connected to the GroupNode
            @return True if no problem occurs */
        bool applyInternalVarValuesToExternal();

    private:
        const QString _uniqueToken;

        QWeakPointer<NodesSequence> _refToThisObject;
        QSharedPointer<QStateMachine> _stateMachine;
        ReferencesHolder _referencesHolder{ ReferencesHolderType::CanHaveInternalRefs };

        QString _errorString;
        QSharedPointer<ANode> _guiltyNode;

        SequenceStateHelper _sequenceState;

        SequenceLogManager _sequenceLogManager;

        QHash<QString, QSharedPointer<ASequenceNode>> _sequenceNodes;
        QHash<QString, QSharedPointer<AReferenceNode>> _referenceNodes;
};
