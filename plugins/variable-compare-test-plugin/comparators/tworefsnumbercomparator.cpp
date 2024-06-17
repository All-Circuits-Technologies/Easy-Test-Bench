// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "tworefsnumbercomparator.hpp"


TwoRefsNumberComparator::TwoRefsNumberComparator(QObject *parent) :
    ATwoRefsComparator<Number>(staticMetaObject.className(), parent)
{
}
