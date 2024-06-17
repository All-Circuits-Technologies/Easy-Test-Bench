// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "decisionaleventvalue.hpp"

#include "testbedcore/customevents.hpp"


DecisionalEventValue::DecisionalEventValue(const QVariant &value) :
    QEvent(CustomEvents::castToEventType(CustomEvents::DecisionalEventValue)),
    _value(value)
{
}
