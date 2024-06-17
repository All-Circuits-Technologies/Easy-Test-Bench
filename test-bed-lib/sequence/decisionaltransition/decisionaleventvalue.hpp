// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QEvent>

#include <QVariant>


/** @brief This event contains the value to compare in the decisional transition */
class DecisionalEventValue : public QEvent
{
    public:
        /** @brief Class constructor
            @param value The value to transmit to decisional transition */
        explicit DecisionalEventValue(const QVariant &value);

        /** @brief Get the event value */
        const QVariant &getValue() const { return _value; }

        /** @brief Set the trigger in order to say if at least one transition has succeeded its
                   comparison test (and so it's no more necessary to test the node with other
                   transition) */
        void setTriggered(bool triggered) { _triggered = triggered; }

        /** @brief Say if at least one transition has succeeded its comparison test (and so it's no
                   more necessary to test the node with other transition) */
        bool isTriggered() const { return _triggered; }

    private:
        QVariant _value;
        bool _triggered{false};
};
