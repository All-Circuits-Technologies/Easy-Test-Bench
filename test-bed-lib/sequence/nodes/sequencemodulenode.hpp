// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "sequence/nodes/atargetingnode.hpp"

#include "jsonutility/jsontypedefs.hpp"
#include "sequence/corestatemachine/referencesholder.hpp"
#include "testbedcore/types/sequencestatetype.hpp"
#include "testbedcore/types/stopaskedtype.hpp"

class ISequenceInstanceModule;


/** @brief The sequence module node is the core of the system and contains the callings to the
           bank modules */
class SequenceModuleNode : public ATargetingNode
{
    Q_OBJECT

    public:
        /** @brief Class constuctor
            @param sequenceParent The NodesSequence parent of this node
            @param parent The class parent */
        explicit SequenceModuleNode(const QSharedPointer<NodesSequence> &sequenceParent,
                                    QObject *parent = nullptr);

        /** @brief Class destructor **/
        virtual ~SequenceModuleNode() override;

        /** @brief Access to the references holder of the node */
        ReferencesHolder &accessReferencesHolder() { return _referencesHolder; }

        /** @brief Get the references holder of the node */
        const ReferencesHolder &getReferencesHolder() const { return _referencesHolder; }

        /** @brief Set the state of the sequence module: if it's enabled or not */
        void setEnable(bool enable) { _enable = enable; }

        /** @brief Test if it's the node is enabled or not */
        bool isEnable() const { return _enable; }

        /** @brief Set the bank interface linked to this sequence
            @param errorHandler An optional sequence loading error handler pointer*/
        bool setSequenceBankIft(ISequenceInstanceModule *seqBankItf,
                                SequenceLoadingErrorHandler *errorhandler = nullptr);

        /** @brief Get the bank interface linked to this sequence */
        const ISequenceInstanceModule *getSequenceBankItf() const { return _sequenceItfModule; }

    protected slots:
        /** @see ASequenceNode::nodeActivity */
        virtual void nodeActivity() override;

        /** @brief Called when the module has finished
            @param success True if the module has ended with success
            @param jsonObjectOutputValues The output values returned by the module */
        void onModuleFinished(bool success, const JsonArray &jsonObjectOutputValues);

    private slots:
        /** @brief Called when the module sequence state changed
            @param newModuleState The current module state
            @param previousModuleState The previous module state before the current one */
        void onNewModuleState(SequenceStateType::Enum newModuleState,
                              SequenceStateType::Enum previousModuleState);

        /** @brief Called when the sequence state changed
            @param newState The current state
            @param previousState The previous state before the current one */
        void onNewSequenceState(SequenceStateType::Enum newState,
                                SequenceStateType::Enum previousState);

        /** @brief Called when a stop has been asked for the sequence
            @param stopAskedType Give a contact to the stop asking */
        void onStopAskedInSequence(StopAskedType::Enum stopAskedType);

    private:
        /** @brief Manage the connect and disconnects between this class and the sequence module
                   given
            @param seqItfModule The module to connect or disconnect with
            @param connectSignals True to connect the signals, False to disconnect the same
                                  signals */
        void manageConnectionSeqModuleSignals(ISequenceInstanceModule *seqItfModule,
                                              bool connectSignals = true);

        /** @brief Manage the writting of the module output values to the Output variable node
            @param outputValues The values got from the sequence module
            @return True if no problem occurs */
        bool manageWriteOutputValues(const JsonArray &outputValues);

        /** @brief Stop the sequence module instance (if it's not) linked and then, when it's
                   stopped, delete it. */
        void stopAndDeleteSequenceModule();

    private:
        QString _bankName;
        QString _moduleName;
        ReferencesHolder _referencesHolder{ ReferencesHolderType::OnlyExternalRefs };
        ISequenceInstanceModule *_sequenceItfModule{nullptr};
        bool _enable{true};
};
