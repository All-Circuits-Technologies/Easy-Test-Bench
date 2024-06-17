// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "variable-operations-plugin/operations/fromrefnumberarithmeticoperator.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"
#include "numberutility/number.hpp"


/** @brief This class allowes to do an arithmetic operation between two numbers got from two
           reference nodes and set the result to variable node. */
class FromMultiRefNumberArithmeticOperator : public FromRefNumberArithmeticOperator
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(FromMultiRefNumberArithmeticOperator)

    public:
        /** @brief Class constructor
            @param parent The class parent*/
        Q_INVOKABLE explicit FromMultiRefNumberArithmeticOperator(QObject *parent = nullptr);

    protected:
        /** @see ABaseOperator::operate
            @note Does: result = refInput1 [operation] refInput2 */
        virtual bool operate(const JsonArray &inputValues, JsonArray &outputValues) override;

        /** @see AFromRefOperator::initParamsFormat
            @see FromRefNumberArithmeticOperator::sharedParamsFormat */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @see ABaseOperator::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

        /** @see AFromRefOperator::initInputsFormat
            @note @ref inputSecPinName : number (The number to use for operating with the first
                                                 input) */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

    private:
        static const constexpr char * inputSecPinName = "inputSecPin";
        static const constexpr char * inputSecPinDesc = "The number to use for operating with the "
                                                        "first input";
};
