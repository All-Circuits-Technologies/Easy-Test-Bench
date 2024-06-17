// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "comparators/arefandparamcomparator.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief Allow to compare a boolean parameter and the boolean value of a reference node */
class RefAndParamBoolComparator : public ARefAndParamComparator<bool>
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(RefAndParamBoolComparator)

    public:
        /** @brief Class constructor
            @param parent The parent class */
        Q_INVOKABLE explicit RefAndParamBoolComparator(QObject *parent = nullptr);

    protected:
        /** @see ARefAndParamComparator::initParamsFormat
            @note @ref valueParamName : boolean (The value to compare with the ref node given) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;
};
