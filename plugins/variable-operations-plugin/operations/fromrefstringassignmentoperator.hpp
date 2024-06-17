// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "variable-operations-plugin/operations/afromrefoperator.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief This module takes the string value of a reference node and set the value in another
           variable. */
class FromRefStringAssignmentOperator : public AFromRefOperator
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(FromRefStringAssignmentOperator)

    public:
        /** @brief Class constructor
            @param parent The parent class */
        Q_INVOKABLE explicit FromRefStringAssignmentOperator(QObject *parent = nullptr);

    protected:
        /** @see ABaseOperator::operate
            @note Does: result = refInput1 */
        virtual bool operate(const JsonArray &inputValues, JsonArray &outputValues) override;
};
