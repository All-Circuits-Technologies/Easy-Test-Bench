// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "comparators/atworefscomparator.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief Allow to compare the string values of two different references nodes */
class TwoRefsStringComparator : public ATwoRefsComparator<QString>
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(TwoRefsStringComparator)

    public:
        /** @brief Class constructor
            @param parent The parent class */
        Q_INVOKABLE explicit TwoRefsStringComparator(QObject *parent = nullptr);
};
