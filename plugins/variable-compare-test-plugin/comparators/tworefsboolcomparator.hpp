// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "comparators/atworefscomparator.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief Allow to compare the boolean values of two different references nodes */
class TwoRefsBoolComparator : public ATwoRefsComparator<bool>
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(TwoRefsBoolComparator)

    public:
        /** @brief Class constructor
            @param parent The parent class */
        Q_INVOKABLE explicit TwoRefsBoolComparator(QObject *parent = nullptr);
};
