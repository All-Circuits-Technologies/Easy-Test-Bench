// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "sequence/decisionaltransition/alogicaloperatordecisionaltransition.hpp"

#include <QVector>


/** @brief OR decisional transition helps to manage multiple tests for the same transition node */
class OrDecisionalTransition : public ALogicalOperatorDecisionalTransition
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param sourceState The source state of the transition */
        explicit OrDecisionalTransition(QState *sourceState = nullptr);

        /** @brief Test if at least one transition linked to this one, succeed
            @param value The value to send to the others transitions, in order to compare with their
                   "values to compare with"
            @return True if at least one of the comparisons succeeds */
        virtual bool testValue(const QVariant &value) const override;
};
