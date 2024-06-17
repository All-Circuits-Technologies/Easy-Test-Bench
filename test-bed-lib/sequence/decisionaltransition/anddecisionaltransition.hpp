// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "sequence/decisionaltransition/alogicaloperatordecisionaltransition.hpp"

#include <QVector>


/** @brief AND decisional transition helps to manage multiple tests for the same transition node */
class AndDecisionalTransition : public ALogicalOperatorDecisionalTransition
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param sourceState The source state of the transition */
        explicit AndDecisionalTransition(QState *sourceState = nullptr);

        /** @brief Test if all the transitions linked succeed
            @param value The value to send to the others transitions, in order to compare with their
                   "values to compare with"
            @return True if all the comparisons succeeds */
        virtual bool testValue(const QVariant &value) const override;
};
