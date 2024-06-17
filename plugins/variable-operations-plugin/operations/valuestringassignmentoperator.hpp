// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "variable-operations-plugin/operations/avalueassignmentoperator.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief This class is useful for assigning a string value (given by parameter) to a reference
           node */
class ValueStringAssignmentOperator : public AValueAssignmentOperator<QString>
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(ValueStringAssignmentOperator)

    public:
        /** @brief Class constructor
            @param parent Class parent */
        Q_INVOKABLE explicit ValueStringAssignmentOperator(QObject *parent = nullptr);

    protected:
        /** @see AValueAssignmentOperator::initParamsFormat
            @note @ref valueParamName: string (Value to set in output variable) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;
};
