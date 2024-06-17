// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "fakecommoninterface.hpp"

class ABaseOperator;


class VariableOperationsPlugin : public QObject
{
    Q_OBJECT

    public:
        VariableOperationsPlugin();
        ~VariableOperationsPlugin();

    private slots:
        void initTestCase();
        void cleanupTestCase();

        void test_valueBoolAssignmentOperator_data();
        void test_valueBoolAssignmentOperator();

        void test_valueNumberAssignmentOperator_data();
        void test_valueNumberAssignmentOperator();

        void test_valueStringAssignmentOperator_data();
        void test_valueStringAssignmentOperator();

        void test_fromRefBoolAssignmentOperator_data();
        void test_fromRefBoolAssignmentOperator();

        void test_fromRefNumberAssignmentOperator_data();
        void test_fromRefNumberAssignmentOperator();

        void test_fromRefStringAssignmentOperator_data();
        void test_fromRefStringAssignmentOperator();

        void test_fromRefNumberArithmeticOperator_data();
        void test_fromRefNumberArithmeticOperator();

        void test_fromMultiRefNumberArithmeticOperator_data();
        void test_fromMultiRefNumberArithmeticOperator();
    private:
        void testOperator(ABaseOperator * baseType);

    private:
        FakeCommonInterface _interface;
};
