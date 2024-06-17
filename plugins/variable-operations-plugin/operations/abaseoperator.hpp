// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "abstractpluginelements/defaultsequenceinstancemodule.hpp"

#include "testbedcore/types/typemanaged.hpp"


/** @brief Base class for all the modules linked to arithmetic operations */
class ABaseOperator : public DefaultSequenceInstanceModule
{
    Q_OBJECT

    public:
        /** @brief Class construtor
            @param moduleClassName The class name of the module
            @param outputType The type of the output
            @param parent The parent class */
        explicit ABaseOperator(const QString &moduleClassName,
                               TypeManaged::Enum outputType,
                               QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~ABaseOperator() override = default;

    protected:
        /** @see DefaultSequenceInstanceModule::initOutputsFormat
            @note @ref outputPin : any (The result of the assignment) */
        virtual bool initOutputsFormat(JsonArray &outputsFormat) const override;

        /** @brief Call to do the arithmetic operation
            @param inputValues The entry values of the operation
            @param outputValues The output values of the operation
            @return True if no problem occurs */
        virtual bool operate(const JsonArray &inputValues, JsonArray &outputValues) = 0;

        /** @brief Get the type managed by the class */
        TypeManaged::Enum getTypeManaged() const { return _outputType; }

    protected:
        /** @brief Helps to add an input or output element where its type depends of the managed
                   type given
            @param inOutName The name of the input/output element
            @param inOutDesc The description of the input/output element
            @param inOutType The type managed linked to the input/output
            @param inOutFormat The format of inputs or outputs
            @return True if no problem occurs */
        static bool addInOutFormat(const QString &inOutName,
                                   const QString &inOutDesc,
                                   TypeManaged::Enum inOutType,
                                   JsonArray &inOutFormat);

    private slots:
        /** @see DefaultSequenceInstanceModule::processModule */
        virtual void processModule(const JsonArray &jsonObjectInputValues) override;

    protected:
        static const constexpr char * outputPinName = "outputPin";
        static const constexpr char * outputPinDesc = "The result of the assignment";

    private:
        TypeManaged::Enum _outputType;
};
