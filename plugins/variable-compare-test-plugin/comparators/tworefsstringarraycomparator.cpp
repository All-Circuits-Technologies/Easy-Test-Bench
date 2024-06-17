// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "tworefsstringarraycomparator.hpp"


TwoRefsStringArrayComparator::TwoRefsStringArrayComparator(QObject *parent) :
    ATwoRefsComparator<QVector<QString>>(staticMetaObject.className(), parent)
{
}
