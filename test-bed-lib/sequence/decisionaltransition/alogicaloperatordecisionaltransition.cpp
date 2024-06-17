// SPDX-FileCopyrightText: 2019 - 2020 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "alogicaloperatordecisionaltransition.hpp"

#include <QDebug>
#include <QtAlgorithms>


ALogicalOperatorDecisionalTransition::ALogicalOperatorDecisionalTransition(
                                                            DecisionalOperator::Enum decisionalOp,
                                                            QState *sourceState) :
    ADecisionalTransition(decisionalOp, sourceState)
{
}

ALogicalOperatorDecisionalTransition::~ALogicalOperatorDecisionalTransition()
{
    qDeleteAll(_transitions);
}

bool ALogicalOperatorDecisionalTransition::setTransitions(
                                                const QVector<ADecisionalTransition *> &transitions)
{
    qDeleteAll(_transitions);

    _transitions.clear();

    _transitions = transitions;

    for(ADecisionalTransition *transition : transitions)
    {
        transition->setMasteredByParent(true);
    }

    return true;
}

bool ALogicalOperatorDecisionalTransition::setValueToCompareWith(const QVariant &valueToCompareWith)
{
    Q_UNUSED(valueToCompareWith)
    qWarning() << "This method must not be called, that makes no sens";
    return false;
}
