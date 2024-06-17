// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "sequence/decisionaltransition/adecisionaltransition.hpp"


/** @brief Test equality between the value got from event and the value to compare with */
class EqualityDecisionalTransition : public ADecisionalTransition
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param testEquality If true, test if the two values are equals
                                If false, test if the two values are not equals
            @param sourceState The source state of the transition */
        explicit EqualityDecisionalTransition(bool testEquality, QState *sourceState = nullptr);

    public:
        /** @see ADecisionalTransition::setValueToCompareWith */
        virtual bool setValueToCompareWith(const QVariant &valueToCompareWith) override;

        /** @brief Test if the value are equals (or not, depends of the _testNotEquality value)
            @param value The value to compare with the "value to compare with"
            @return True if the comparison succeeds*/
        virtual bool testValue(const QVariant &value) const override;
};
