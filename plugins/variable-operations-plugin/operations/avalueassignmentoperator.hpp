// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "variable-operations-plugin/operations/abaseoperator.hpp"

#include "testbedcore/parsers/bankjsonparser.hpp"


/** @brief This class is a base for all the modules where the value to set to the reference is given
           by a parameter
    @warning Because Q_OBJECT and templated classes are incompatible, we can't add the MACRO in the
             class. Therefore, if you inherit this class and want to use Q_OBJECT functionalities
             be aware that some can fail in strangeous ways */
template<class T>
class AValueAssignmentOperator : public ABaseOperator
{
    static_assert(std::is_same<Number, T>::value            ||
                  std::is_same<bool, T>::value              ||
                  std::is_same<QString, T>::value           ||
                  std::is_same<QVector<Number>, T>::value   ||
                  std::is_same<QVector<bool>, T>::value     ||
                  std::is_same<QVector<QString>, T>::value,
                  "T must be a type managed");

    public:
        /** @brief Class constructor
            @param moduleClassName The class name of the module
            @param parent The parent class */
        explicit AValueAssignmentOperator(const QString &moduleClassName,
                                          QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~AValueAssignmentOperator() override = default;

    protected:
        /** @brief Get the current value */
        const T &getValue() const { return _value; }

        /** @brief Validate and set the parameters into the module
            @see ISequenceInstanceModule::setModuleParameters
            @param parameters The parameters to test given by the test lib
            @param errors If the parameters haven't been correctly set, this array is filled with
                          informations to display to the user; and so he can understand what to
                          correct
            @return True if no problem occurs */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

        /** @brief Call to do the arithmetic operation
            @param inputValues The entry values of the operation
            @param outputValues The output values of the operation
            @return True if no problem occurs */
        virtual bool operate(const JsonArray &inputValues, JsonArray &outputValues) override;

    protected:
        static const constexpr char * valueParamName = "assignmentModifier";
        static const constexpr char * valueParamDesc = "Value to set in output variable";

    private:
        T _value;
};

template<class T>
AValueAssignmentOperator<T>::AValueAssignmentOperator(const QString &moduleClassName,
                                                      QObject *parent) :
    ABaseOperator(moduleClassName, CommonJsonParser::getManagedType<T>(), parent)
{
}

template<class T>
bool AValueAssignmentOperator<T>::validateAndSetParameters(const JsonArray &parameters,
                                                           JsonArray &errors)
{
    RETURN_IF_FALSE(ABaseOperator::validateAndSetParameters(parameters, errors));

    QString errorStr;

    if(!BankJsonParser::getKnownValue(parameters, valueParamName, _value, &errorStr))
    {
        fillErrorArray(valueParamName, errorStr, errors);
        return false;
    }

    return true;
}

template<class T>
bool AValueAssignmentOperator<T>::operate(const JsonArray &inputValues, JsonArray &outputValues)
{
    Q_UNUSED(inputValues)
    return BankJsonParser::setKnownValue<T>(outputPinName, _value, outputValues);
}
