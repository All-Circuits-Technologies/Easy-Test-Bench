// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "anddecisionaltransition.hpp"

#include <QDebug>


AndDecisionalTransition::AndDecisionalTransition(QState *sourceState) :
    ALogicalOperatorDecisionalTransition(DecisionalOperator::And, sourceState)
{
}

bool AndDecisionalTransition::testValue(const QVariant &value) const
{
    bool tested = true;

    for(const ADecisionalTransition *transition : getTransitions())
    {
        tested = tested && transition->testValue(value);
    }

    return tested;
}
