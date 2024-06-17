// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "lesserthandecisionaltransition.hpp"

#include "numberutility/number.hpp"


LesserThanDecisionalTransition::LesserThanDecisionalTransition(bool testEquality,
                                                               QState *sourceState) :
    ADecisionalTransition(testEquality ? DecisionalOperator::LesserOrEqualTo
                                       : DecisionalOperator::LesserThan,
                          sourceState)
{
}

bool LesserThanDecisionalTransition::setValueToCompareWith(const QVariant &valueToCompareWith)
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

bool LesserThanDecisionalTransition::testValue(const QVariant &value) const
{
    return isTestingEquality() ? (value <= getValueToCompareWith())
                               : (value <  getValueToCompareWith());
}
