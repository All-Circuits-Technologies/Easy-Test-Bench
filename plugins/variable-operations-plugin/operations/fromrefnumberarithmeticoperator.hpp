// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "variable-operations-plugin/operations/fromrefnumberassignmentoperator.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"
#include "numberutility/number.hpp"


/** @brief This class allowes to do an arithmetic operation between a number given in a parameter
           and the value got from a reference node and set the result to variable node. */
class FromRefNumberArithmeticOperator : public FromRefNumberAssignmentOperator
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(FromRefNumberArithmeticOperator)

    private:
        /** @brief Operation to exectute before assigning value to the veriable */
        enum Operator
        {
            Addition,
            Substraction,
            Multiply,
            Divide,
            Unknown
        };

    public:
        /** @brief Class constructor
            @param parent The parent class */
        Q_INVOKABLE explicit FromRefNumberArithmeticOperator(QObject *parent = nullptr);

    /** @brief Class destructor */
    virtual ~FromRefNumberArithmeticOperator() override = default;

    protected:
        /** @brief Class constructor
            @param moduleClassName The class name of the module
            @param parent The parent class */
        explicit FromRefNumberArithmeticOperator(const QString &moduleClassName,
                                                 QObject *parent = nullptr);

    protected:
        /** @see ABaseOperator::operate
            @note Does: result = refInput1 [operation] paramValue */
        virtual bool operate(const JsonArray &inputValues, JsonArray &outputValues) override;

        /** @see AFromRefOperator::initParamsFormat
            @note @ref operatorParamName : array<string> (Select the arithmetic operator)
            @note @ref valueParamName : number (Enter the arithmetic value to operate with (this
                                                number is at the right of the operation)) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @see ABaseOperator::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

        /** @brief Apply the arithmetic operation of the numbers
            @note The operation is done like this :
                  result = basicNumber [operation] otherNumber
            @param basicNumber The first element of the operation
            @param otherNumber The second element of the operation
            @param outputValues The output values of the operation
            @return True if no problem occurs */
        bool arithmeticOperation(const Number &basicNumber,
                                 const Number &otherNumber,
                                 JsonArray &outputValues);

        /** @brief Validate and set the parameters into the module
            @see This method is used for managing this class and its derived classes
            @see ISequenceInstanceModule::setModuleParameters
            @param parameters The parameters to test given by the test lib
            @param errors If the parameters haven't been correctly set, this array is filled with
                          informations to display to the user; and so he can understand what to
                          correct
            @return True if no problem occurs */
        bool sharedValidateAndSetParameters(const JsonArray &parameters, JsonArray &errors);

        /** @brief Contains the shared parameters format with derive classes
            @param paramsFormat The shared paramters format
            @return True if no problem occurs */
        bool sharedParamsFormat(JsonArray &paramsFormat) const;

    private:
        static const constexpr char * operatorParamName = "arithmeticOperator";
        static const constexpr char * operatorParamDesc = "Select the arithmetic operator";
        static const constexpr char * optionAddKey = "addKey";
        static const constexpr char * optionAddValue = "+";
        static const constexpr char * optionSubKey = "subKey";
        static const constexpr char * optionSubValue = "-";
        static const constexpr char * optionMultKey = "multKey";
        static const constexpr char * optionMultValue = "*";
        static const constexpr char * optionDivKey = "divKey";
        static const constexpr char * optionDivValue = "/";

        static const constexpr char * optionDefault = optionAddKey;

        static const constexpr char * valueParamName = "arithmeticValue";
        static const constexpr char * valueParamDesc = "Enter the arithmetic value to operate with "
                                                       "(this number is at the right of the "
                                                       "operation)";

    private:
        Operator _arithmOperator {Unknown};
        Number _valueToOperateWith;
};
