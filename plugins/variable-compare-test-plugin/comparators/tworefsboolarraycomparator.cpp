// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "tworefsboolarraycomparator.hpp"


TwoRefsBoolArrayComparator::TwoRefsBoolArrayComparator(QObject *parent) :
    ATwoRefsComparator<QVector<bool>>(staticMetaObject.className(), parent)
{
}
