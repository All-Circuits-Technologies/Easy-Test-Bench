// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "equalitydecisionaltransition.hpp"


EqualityDecisionalTransition::EqualityDecisionalTransition(bool testEquality, QState *sourceState) :
    ADecisionalTransition(testEquality ? DecisionalOperator::EqualTo :
                                         DecisionalOperator::NotEqualTo, sourceState)
{
}

bool EqualityDecisionalTransition::setValueToCompareWith(const QVariant &valueToCompareWith)
{
    setValue(valueToCompareWith);
    return true;
}

bool EqualityDecisionalTransition::testValue(const QVariant &value) const
{
    return isTestingEquality() ? (value == getValueToCompareWith())
                               : (value != getValueToCompareWith());
}
