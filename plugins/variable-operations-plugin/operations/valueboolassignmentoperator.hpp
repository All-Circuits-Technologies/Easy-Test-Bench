// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "variable-operations-plugin/operations/avalueassignmentoperator.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief This class is useful for assigning a boolean value (given by parameter) to a reference
           node */
class ValueBoolAssignmentOperator : public AValueAssignmentOperator<bool>
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(ValueBoolAssignmentOperator)

    public:
        /** @brief Class constructor
            @param parent Class parent */
        Q_INVOKABLE explicit ValueBoolAssignmentOperator(QObject *parent = nullptr);

    protected:
        /** @see AValueAssignmentOperator::initParamsFormat
            @note @ref valueParamName: boolean (Value to set in output variable) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;
};
