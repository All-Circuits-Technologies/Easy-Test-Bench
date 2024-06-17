// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "sequence/decisionaltransition/adecisionaltransition.hpp"

#include <QVector>


/** @brief A logical transition allows to have multiple transitions tested for only one target */
class ALogicalOperatorDecisionalTransition : public ADecisionalTransition
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param decisionalOp The decisional operator of the transition
            @param sourceState The source state of the transition */
        explicit ALogicalOperatorDecisionalTransition(DecisionalOperator::Enum decisionalOp,
                                                      QState *sourceState = nullptr);

        /** @brief Class destructor */
        virtual ~ALogicalOperatorDecisionalTransition() override;

    public:
        /** @brief Set the transitions to link with this one
            @warning Ownership of those transitions will be given to
                     ALogicalOperatorDecisionalTransition. Do not delete them afterwards
            @warning Given transitions may be deleted by ALogicalOperatorDecisionalTransition at any
                     time, including immediately. You should never try to re-use pointer of given
                     vector. Just through this vector away without deleting or using pointers it
                     contains (you loose their ownership and they may be already freed)
            @note The transitions given can't be an AND or OR operator
            @param transitions The transitions to link with the logical one
            @return True if no problem occurs */
        bool setTransitions(const QVector<ADecisionalTransition *> &transitions);

        /** @brief Get the transitions linked with this logical transition */
        const QVector<ADecisionalTransition *> &getTransitions() const { return _transitions; }

    public:
        /** @brief Set the value to compare with
            @warning The method has to be implemented but that makes no sens to use if for
                     Logical transition, so don't use it
            @note In this kind of DecisionalTransition, that makes no sens
            @param valueToCompareWith The value to compare with
            @return Always returns false */
        virtual bool setValueToCompareWith(const QVariant &valueToCompareWith) override;

    private:
        QVector<ADecisionalTransition *> _transitions;
};
