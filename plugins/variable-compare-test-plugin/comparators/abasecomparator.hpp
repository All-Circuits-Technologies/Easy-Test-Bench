// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "abstractpluginelements/defaultsequenceinstancemodule.hpp"

#include "numberutility/number.hpp"
#include "testbedcore/types/typemanaged.hpp"


/** @brief Base class for all the modules linked to comparator operations */
class ABaseComparator : public DefaultSequenceInstanceModule
{
    Q_OBJECT

    protected:
        /** @brief Class constructor
            @param moduleClassName The class name of the module
            @param typeManaged The type managed by this comparator
            @param parent The parent class */
        explicit ABaseComparator(const QString &moduleClassName,
                                 TypeManaged::Enum typeManaged,
                                 QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~ABaseComparator() override = default;

    protected:
        /** @see DefaultSequenceInstanceModule::initParamsFormat
            @note @ref comparisonOperatorParamName : array<string> (The comparator used)
                  @ref emitErrorIfFailParamName : boolean (If the option is active and an error
                                                           occurs in the module, the module will
                                                           fail) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @see DefaultSequenceInstanceModule::initInputsFormat
            @note @ref inputPinName : any (This is the element to compare with) */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

        /** @see DefaultSequenceInstanceModule::initOutputsFormat
            @note @ref outputSuccessPinName : boolean (This output contains the result of the
                                                       comparison.)
            @note @ref outputErrorMsgPinName : string (This output contains the msg error link to
                                                       the comparison.) */
        virtual bool initOutputsFormat(JsonArray &outputsFormat) const override;

        /** @see DefaultSequenceInstanceModule::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

        /** @brief Call to do the comparing
            @note The method @ref ABaseComparator::doCompareValues facilitate the comparing and
                  manage it for derived classes.
                  Therefore when overriding this method, you have only to get values and test them
                  with the method: @ref ABaseComparator::doCompareValues
            @param inputValues The entry values of the comparing
            @param result The result of the comparing (True if the comparing returns true)
            @param errorLabel If an error occured in the method, this label contains the detail of
                              the error.
            @return True if no problem occurs */
        virtual bool compare(const JsonArray &inputValues, bool &result, QString &errorLabel) = 0;

    protected:
        /** @brief Helps to add an input element where its type depends of the managed type given
            @param inputName The name of the input element
            @param inputDesc The description of the input element
            @param managedType The type managed linked to the input
            @param inputsFormat The format of inputs
            @return True if no problem occurs */
        static bool addInputFormat(const QString &inputName,
                                   const QString &inputDesc,
                                   TypeManaged::Enum managedType,
                                   JsonArray &inputsFormat);

    protected:
        /** @brief Get the type managed by the comparator */
        TypeManaged::Enum getTypeManaged() const { return _typeManaged; }

    protected:
        /** @brief This method compare the two values given and following the comparison operator
                   chosen by the user
            @note The test is done like that : result = firstValue [comparisonOperator] secondValue
            @param firstValue The first value of the comparison
            @param secondValue The second value of the comparison
            @return The result of the comparing */
        template<class T>
        bool doCompareValues(const T &firstValue, const T &secondValue);

    private slots:
        /** @see DefaultSequenceInstanceModule::processModule */
        virtual void processModule(const JsonArray &jsonObjectInputValues) override;

    private:
        /** @brief Helpful method to save in the right format the result of the comparison
            @param comparisonResult The comparison result
            @param errorLabel The error label if an error occured
            @param outputValues The output values filled with the info given
            @return True if no problem occurs */
        bool manageOutputVariables(bool comparisonResult,
                                   const QString &errorLabel,
                                   JsonArray &outputValues);

    private:
        /** @brief Test if the type given is managing extended comparisons
            @param typeManaged The type managed to test
            @return True if the type given is managing the extended comparisons */
        static bool isManagingExtendedComparisons(TypeManaged::Enum typeManaged);

    protected:
        static const constexpr char * comparisonOperatorParamName = "comparisonOperator";
        static const constexpr char * comparisonOperatorParamDesc = "Select the comparison "
                                                                     "operator";
        static const constexpr char * optionEqualKey = "equalKey";
        static const constexpr char * optionEqualValue = "==";
        static const constexpr char * optionNotEqualKey = "notEqualKey";
        static const constexpr char * optionNotEqualValue = "!=";
        static const constexpr char * optionLessKey = "lessKey";
        static const constexpr char * optionLessValue = "<";
        static const constexpr char * optionLessOrEqualKey = "lessOrEqualKey";
        static const constexpr char * optionLessOrEqualValue = "<=";
        static const constexpr char * optionGreaterKey = "greaterKey";
        static const constexpr char * optionGreaterValue = ">";
        static const constexpr char * optionGreaterOrEqualKey = "greaterOrEqualKey";
        static const constexpr char * optionGreaterOrEqualValue = ">=";

        static const constexpr char * optionDefault = optionEqualKey;

        static const constexpr char * emitErrorIfFailParamName = "exitIfFail";
        static const constexpr char * emitErrorIfFailParamDesc = "If the option is active and an "
                                                                 "error occurs in the module, "
                                                                 "the module will fail";

        static const constexpr char * inputPinName = "inputPin";
        static const constexpr char * inputPinDesc = "This is the element to compare with";

        static const constexpr char * outputSuccessPinName = "outputSuccessPin";
        static const constexpr char * outputSuccessPinDesc = "This output contains the result of "
                                                             "the comparison.";

        static const constexpr char * outputErrorMsgPinName = "outputErrorMsgPin";
        static const constexpr char * outputErrorMsgPinDesc = "This output contains the msg error "
                                                              "link to the comparison.";

    private:
        /** @brief The available comparison operators */
        enum ComparisonOperator
        {
            Equal,
            NotEqual,
            //Extended comparison
            LesserThan,
            LesserThanOrEqualTo,
            GreaterThan,
            GreaterThanOrEqualTo
        };

    private:
        const TypeManaged::Enum _typeManaged;

        ComparisonOperator _comparisonOperator{ComparisonOperator::Equal};
        bool _emitErrorIfFail{true};
};

template<class T>
bool ABaseComparator::doCompareValues(const T &firstValue, const T &secondValue)
{
    static_assert(std::is_same<Number, T>::value            ||
                  std::is_same<bool, T>::value              ||
                  std::is_same<QString, T>::value           ||
                  std::is_same<QVector<Number>, T>::value   ||
                  std::is_same<QVector<bool>, T>::value     ||
                  std::is_same<QVector<QString>, T>::value,
                  "T must be a type managed");

    bool result = false;

    if(_comparisonOperator == Equal)
    {
        result = (firstValue == secondValue);
    }
    else if(_comparisonOperator == NotEqual)
    {
        result = (firstValue != secondValue);
    }
    else if(_comparisonOperator == LesserThan)
    {
        result = (firstValue < secondValue);
    }
    else if(_comparisonOperator == LesserThanOrEqualTo)
    {
        result = (firstValue <= secondValue);
    }
    else if(_comparisonOperator == GreaterThan)
    {
        result = (firstValue > secondValue);
    }
    else if(_comparisonOperator == GreaterThanOrEqualTo)
    {
        result = (firstValue >= secondValue);
    }

    return result;
}
