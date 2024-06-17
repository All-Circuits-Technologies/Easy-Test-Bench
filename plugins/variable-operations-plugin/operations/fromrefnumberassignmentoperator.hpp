// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "variable-operations-plugin/operations/afromrefoperator.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief This module takes the number value of a reference node and set the value in another
           variable.
    @note It's possible to make the value absolute or to change it to its opposite before assigning
          it to other variable. */
class FromRefNumberAssignmentOperator : public AFromRefOperator
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(FromRefNumberAssignmentOperator)

    private:
        /** @brief Operation to execute before assigning value to the variable */
        enum Operation
        {
            Opposite,
            Absolute,
            Same,
            Unknown
        };

    public:
        /** @brief Class constructor
            @param parent The parent class */
        Q_INVOKABLE explicit FromRefNumberAssignmentOperator(QObject *parent = nullptr);

    /** @brief Class destructor */
    virtual ~FromRefNumberAssignmentOperator() override = default;

    protected:
        /** @brief Class constructor
            @param moduleClassName The class name of the module
            @param parent The parent class */
        explicit FromRefNumberAssignmentOperator(const QString &moduleClassName,
                                                 QObject *parent = nullptr);

    protected:
        /** @see AFromRefOperator::initParamsFormat
            @note @ref operatorParamName : array<string> (Select a simple operation) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @see ABaseOperator::operate
            @note Does: result = [operation] refInput1 */
        virtual bool operate(const JsonArray &inputValues, JsonArray &outputValues) override;

        /** @see ABaseOperator::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;
    private:
        static const constexpr char * operatorParamName = "simpleOperation";
        static const constexpr char * operatorParamDesc = "Select a simple operation";

        static const constexpr char * optionOppKey = "oppositeKey";
        static const constexpr char * optionOppValue = "-";
        static const constexpr char * optionAbsKey = "absolute";
        static const constexpr char * optionAbsValue = "Absolute";
        static const constexpr char * optionSameKey = "sameKey";
        static const constexpr char * optionSameValue = "=";

        static const constexpr char * optionDefault = optionSameKey;

    private:
        Operation _numberOperation {Unknown};
};
