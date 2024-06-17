// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "sequence/decisionaltransition/adecisionaltransition.hpp"


/** @brief Test if the value got from event is lesser than the one given "to compare with" */
class LesserThanDecisionalTransition : public ADecisionalTransition
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param testEquality True if the transition also test the equality (greater or equal)
            @param sourceState The source state of the transition */
        explicit LesserThanDecisionalTransition(bool testEquality, QState *sourceState = nullptr);

    public:
        /** @see ADecisionalTransition::setValueToCompareWith */
        virtual bool setValueToCompareWith(const QVariant &valueToCompareWith) override;

        /** @brief Test if the value are lesser than the "value to compare with"
            @note To be clearer, test if the param given value <(=) _valueToCompareWith
            @param value The value to compare with the "value to compare with"
            @return True if the comparison succeeds */
        virtual bool testValue(const QVariant &value) const override;
};
