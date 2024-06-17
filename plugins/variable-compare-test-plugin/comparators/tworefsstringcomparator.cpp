// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "tworefsstringcomparator.hpp"


TwoRefsStringComparator::TwoRefsStringComparator(QObject *parent) :
    ATwoRefsComparator<QString>(staticMetaObject.className(), parent)
{
}
