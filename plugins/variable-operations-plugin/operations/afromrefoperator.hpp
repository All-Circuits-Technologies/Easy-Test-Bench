// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "variable-operations-plugin/operations/abaseoperator.hpp"


/** @brief This class is a base for all the modules with takes values from one or several reference
           node */
class AFromRefOperator : public ABaseOperator
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param moduleClassName The class name of the module
            @param typeManaged The type managed by this comparator
            @param parent The parent class */
        explicit AFromRefOperator(const QString &moduleClassName,
                                  TypeManaged::Enum typeManaged,
                                  QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~AFromRefOperator() override = default;

    protected:
        /** @see ABaseOperator::initInputsFormat */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

    protected:
        static const constexpr char * inputPinName = "inputPin";
        static const constexpr char * inputPinDesc = "This is the element to assign in output";
};
