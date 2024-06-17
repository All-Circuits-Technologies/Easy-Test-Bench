// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "tworefsboolcomparator.hpp"


TwoRefsBoolComparator::TwoRefsBoolComparator(QObject *parent) :
    ATwoRefsComparator<bool>(staticMetaObject.className(), parent)
{
}
