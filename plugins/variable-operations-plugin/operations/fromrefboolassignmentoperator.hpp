// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "variable-operations-plugin/operations/afromrefoperator.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief This module takes the boolean value of a reference node and set the value in another
           variable.
    @note It's possible to make the value opposite before assigning it to other variable. */
class FromRefBoolAssignmentOperator : public AFromRefOperator
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(FromRefBoolAssignmentOperator)

    public:
        /** @brief Class constructor
            @param parent The parent class */
        Q_INVOKABLE explicit FromRefBoolAssignmentOperator(QObject *parent = nullptr);

    protected:
        /** @see AFromRefOperator::initParamsFormat
            @note @ref assingParamName : array<string> (Select the assignment modifier) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @see ABaseOperator::operate
            @note Does: result = [operation] refInput1 */
        virtual bool operate(const JsonArray &inputValues, JsonArray &outputValues) override;

        /** @see ABaseOperator::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

    private:
        static const constexpr char * assingParamName = "assignmentModifier";
        static const constexpr char * assignParamDesc = "Select the assignment modifier";

        static const constexpr char * optionSameKey = "sameKey";
        static const constexpr char * optionNotKey = "notKey";
        static const constexpr char * optionSameValue = "=";
        static const constexpr char * optionNotValue = "!=";

        static const constexpr char * optionDefault = optionSameKey;

    private:
        bool _sameValue{true};
};
