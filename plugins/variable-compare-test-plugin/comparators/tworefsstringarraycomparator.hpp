// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "comparators/atworefscomparator.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief Allow to compare the string array values of two different references nodes */
class TwoRefsStringArrayComparator : public ATwoRefsComparator<QVector<QString>>
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(TwoRefsStringArrayComparator)

    public:
        /** @brief Class constructor
            @param parent The parent class */
        Q_INVOKABLE explicit TwoRefsStringArrayComparator(QObject *parent = nullptr);
};
