// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QAbstractTransition>

#include <QVariant>

#include "sequence/decisionaltransition/decisionaloperator.hpp"


/** @brief Abstract transition used by decisional node.
    @note All the decisional transitions compare two variants. One is set in the object and an
          another is received via a special event (@ref DecisionalEventValue) */
class ADecisionalTransition : public QAbstractTransition
{
    Q_OBJECT

    public:
        /** @brief Object constructor
            @param decisionalOp The decisional operator of the transition
            @param sourceState The source state of the transition */
        explicit ADecisionalTransition(DecisionalOperator::Enum decisionalOp,
                                       QState *sourceState = nullptr);

        /** @brief Class destructor */
        virtual ~ADecisionalTransition() override = default;

    public:
        /** @brief Get the value to compare with */
        const QVariant &getValueToCompareWith() const { return _valueToCompareWith; }

        /** @brief Say if the transition is testing equality */
        bool isTestingEquality() const;

        /** @brief Say if the the transition is a logical operator */
        bool isLogicalOperator() const;

        /** @brief Give the comparison applyed to the value received */
        DecisionalOperator::Enum getOperator() const { return _decisionalOperator; }

        /** @brief Say that the decisional transition is managed by the decistion transition parent.
                   Therefore, the transition won't look over the events received */
        void setMasteredByParent(bool masteredByParent) { _masteredByParent = masteredByParent; }

    public:
        /** @brief Set the value to compare with */
        virtual bool setValueToCompareWith(const QVariant &valueToCompareWith) = 0;

        /** @brief Test the value to compare with and this value
            @param value The value to compare with the "value to compare with"
            @return True if the comparison succeeds*/
        virtual bool testValue(const QVariant &value) const = 0;

    protected:
        /** @brief Set the value to compare with */
        void setValue(const QVariant &valueToCompareWith)
        { _valueToCompareWith = valueToCompareWith; }

    protected:
        /** @brief This method is called when an event is received
                   If the DecisionalEventValue is received and if no other transition has succeeded
                   it's test, test the value received
            @note Override Qt method to manage a special event
            @note Emit @see ADecisionalTransition::testResult event
            @param event The event to test
            @return True if the test succeeds and if the transition can be fired in order to go to
                    the next node */
        virtual bool eventTest(QEvent *event) override;

        /** @brief This method is called when the transition is triggered
            @note It's necessary to override this method, but there is nothing to do here.
            @param event The event is what caused the transition to trigger */
        virtual void onTransition(QEvent *event) override;

    signals:
        /** @brief Emitted when the test is processed and give its result
            @param success True if the test has succeeded */
        void testResult(bool success);

    private:
        QVariant _valueToCompareWith;
        DecisionalOperator::Enum _decisionalOperator{DecisionalOperator::Unknown};
        bool _masteredByParent{false};
};
