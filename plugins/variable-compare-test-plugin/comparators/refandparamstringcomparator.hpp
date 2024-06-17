// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "comparators/arefandparamcomparator.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief Allow to compare a string parameter and the string value of a reference node */
class RefAndParamStringComparator : public ARefAndParamComparator<QString>
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(RefAndParamStringComparator)

    public:
        /** @brief Class constructor
            @param parent The parent class */
        Q_INVOKABLE explicit RefAndParamStringComparator(QObject *parent = nullptr);

    protected:
        /** @see ARefAndParamComparator::initParamsFormat
            @note @ref valueParamName : number (The value to compare with the ref node given) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;
};
