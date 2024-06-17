// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "ordecisionaltransition.hpp"

#include <QDebug>


OrDecisionalTransition::OrDecisionalTransition(QState *sourceState) :
    ALogicalOperatorDecisionalTransition(DecisionalOperator::Or, sourceState)
{
}

bool OrDecisionalTransition::testValue(const QVariant &value) const
{
    bool tested = false;

    for(const ADecisionalTransition *transition : getTransitions())
    {
        tested = tested || transition->testValue(value);
    }

    return tested;
}
