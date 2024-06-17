// SPDX-FileCopyrightText: 2019 - 2020 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "adecisionaltransition.hpp"

#include <QDebug>

#include "sequence/decisionaltransition/decisionaleventvalue.hpp"
#include "testbedcore/customevents.hpp"


ADecisionalTransition::ADecisionalTransition(DecisionalOperator::Enum decisionalOp,
                                             QState *sourceState) :
    QAbstractTransition(sourceState),
    _decisionalOperator(decisionalOp)
{
}

bool ADecisionalTransition::isTestingEquality() const
{
    return (_decisionalOperator == DecisionalOperator::EqualTo ||
            _decisionalOperator == DecisionalOperator::LesserOrEqualTo ||
            _decisionalOperator == DecisionalOperator::GreaterOrEqualTo);
}

bool ADecisionalTransition::isLogicalOperator() const
{
    return (_decisionalOperator == DecisionalOperator::Or ||
            _decisionalOperator == DecisionalOperator::And);
}

bool ADecisionalTransition::eventTest(QEvent *event)
{
    if(_masteredByParent)
    {
        // Parent transition will manage the test
        return false;
    }

    if(!event)
    {
        // Because Qt is calling this method, this case, where event is null, should never happen.
        // But, we never know...
        return false;
    }

    if(event->type() != CustomEvents::castToEventType(CustomEvents::DecisionalEventValue))
    {
        return false;
    }

    DecisionalEventValue *eventValue = dynamic_cast<DecisionalEventValue*>(event);

    if(eventValue->isTriggered())
    {
        // An another test has already worked
        return false;
    }

    bool triggered = testValue(eventValue->getValue());

    eventValue->setTriggered(triggered);

    emit testResult(triggered);

    return triggered;
}

void ADecisionalTransition::onTransition(QEvent *event)
{
    Q_UNUSED(event)
    // Nothing to do here
}
