// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include <QObject>


class BmsGadgetJsonLoaderTest : public QObject
{
    Q_OBJECT

    public:
        BmsGadgetJsonLoaderTest();
        ~BmsGadgetJsonLoaderTest();

    private slots:
        void initTestCase();
        void cleanupTestCase();
        void test_createqobject_data();
        void test_createqobject();
};
