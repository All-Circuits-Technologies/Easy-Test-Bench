// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "comparators/arefandparamcomparator.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief Allow to compare a number parameter and the number value of a reference node */
class RefAndParamNumberComparator : public ARefAndParamComparator<Number>
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(RefAndParamNumberComparator)

    public:
        /** @brief Class constructor
            @param parent The parent class */
        Q_INVOKABLE explicit RefAndParamNumberComparator(QObject *parent = nullptr);

    protected:
        /** @see ARefAndParamComparator::initParamsFormat
            @note @ref valueParamName : number (The value to compare with the ref node given) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;
};
