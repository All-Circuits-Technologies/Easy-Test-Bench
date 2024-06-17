// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "tworefsnumberarraycomparator.hpp"


TwoRefsNumberArrayComparator::TwoRefsNumberArrayComparator(QObject *parent) :
    ATwoRefsComparator<QVector<Number>>(staticMetaObject.className(), parent)
{
}
