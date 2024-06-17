// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "greaterthandecisionaltransition.hpp"

#include "numberutility/number.hpp"


GreaterThanDecisionalTransition::GreaterThanDecisionalTransition(bool testEquality,
                                                                 QState *sourceState) :
    ADecisionalTransition(testEquality ? DecisionalOperator::GreaterOrEqualTo
                                       : DecisionalOperator::GreaterThan,
                          sourceState)
{
}

bool GreaterThanDecisionalTransition::setValueToCompareWith(const QVariant &valueToCompareWith)
{
    if(!valueToCompareWith.canConvert<Number>())
    {
        qWarning() << "The value: " << valueToCompareWith
                   << ", is not a number can't set the value to the decisional transition";
        return false;
    }

    setValue(valueToCompareWith);

    return true;
}

bool GreaterThanDecisionalTransition::testValue(const QVariant &value) const
{
    return isTestingEquality() ? (value >= getValueToCompareWith())
                               : (value >  getValueToCompareWith());
}
