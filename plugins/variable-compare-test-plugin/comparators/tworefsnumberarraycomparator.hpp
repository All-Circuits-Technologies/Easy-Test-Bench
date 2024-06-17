// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "comparators/atworefscomparator.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief Allow to compare the number array values of two different references nodes */
class TwoRefsNumberArrayComparator : public ATwoRefsComparator<QVector<Number>>
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(TwoRefsNumberArrayComparator)

    public:
        /** @brief Class constructor
            @param parent The parent class */
        Q_INVOKABLE explicit TwoRefsNumberArrayComparator(QObject *parent = nullptr);
};
