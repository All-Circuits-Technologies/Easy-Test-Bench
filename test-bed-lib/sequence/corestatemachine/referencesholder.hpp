// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>
#include <QVector>

#include "sequence/corestatemachine/innodeport.hpp"
#include "sequence/corestatemachine/outnodeport.hpp"
#include "sequence/corestatemachine/referencesholdertype.hpp"


/** @brief This class holds input and output references of a node */
class ReferencesHolder : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param refHolderType The type of referencesHolder
            @param parent The class parent */
        explicit ReferencesHolder(ReferencesHolderType::Enum refHolderType,
                                  QObject *parent = nullptr);

        /** @brief Add input node to the holder
            @param inputNode The input node port to add */
        void addInput(const InNodePort &inputNode) { _inputs.append(inputNode); }

        /** @brief Get input ports linked to the holder */
        const QVector<InNodePort> &getInputs() const { return _inputs; }

        /** @brief Access input ports linked to the holder */
        QVector<InNodePort> &accessInputs() { return _inputs; }

        /** @brief Add output node to the holder
            @param outputNode The output node port to add */
        void addOutput(const OutNodePort &outputNode) { _outputs.append(outputNode); }

        /** @brief Get output ports linked to the holder */
        const QVector<OutNodePort> &getOutputs() const { return _outputs; }

        /** @brief Access output ports linked to the holder */
        QVector<OutNodePort> &accessOutputs() { return _outputs; }

        /** @brief Get the type of referencesHolder */
        ReferencesHolderType::Enum getHolderType() const { return _referenceHolderType; }

        /** @brief Equality operator, test if the other ReferencesHolder is equal to this one
            @param refHolder The other ReferencesHolder to test with
            @return True if the two ReferencesHolder are equals */
        bool operator==(const ReferencesHolder &refHolder) const;

    private:
        ReferencesHolderType::Enum _referenceHolderType;
        QVector<InNodePort> _inputs;
        QVector<OutNodePort> _outputs;
};
