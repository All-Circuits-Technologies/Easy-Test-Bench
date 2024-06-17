// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "comparators/abasecomparator.hpp"

#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/parsers/commonjsonparser.hpp"
#include "testbedcore/testbedglobal.hpp"


/** @brief Base class for comparing a module parameter with the value of a given reference node
    @warning Because Q_OBJECT and templated classes are incompatible, we can't add the MACRO in the
             class. Therefore, if you inherit this class and want to use Q_OBJECT functionalities
             be aware that some can fail in strangeous ways */
template<class T>
class ARefAndParamComparator : public ABaseComparator
{
    static_assert(std::is_same<Number, T>::value            ||
                  std::is_same<bool, T>::value              ||
                  std::is_same<QString, T>::value           ||
                  std::is_same<QVector<Number>, T>::value   ||
                  std::is_same<QVector<bool>, T>::value     ||
                  std::is_same<QVector<QString>, T>::value,
                  "T must be a type managed");

    protected:
        /** @brief Class constructor
            @param moduleClassName The class name of the module
            @param parent The parent class */
        explicit ARefAndParamComparator(const QString &moduleClassName,
                                        QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~ARefAndParamComparator() override = default;

    protected:
        /** @brief Get param value set */
        const T &getParamValue() const { return _paramValue; }

    protected:
        /** @see ABaseComparator::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

        /** @see ABaseComparator::compare */
        virtual bool compare(const JsonArray &inputValues,
                             bool &result,
                             QString &errorLabel) override;

    protected:
        static const constexpr char * valueParamName = "valueParamName";
        static const constexpr char * valueParamDesc = "The value to compare with the ref node "
                                                       "given";

    private:
        T _paramValue;
};

template<class T>
ARefAndParamComparator<T>::ARefAndParamComparator(const QString &moduleClassName,
                                                  QObject *parent) :
    ABaseComparator(moduleClassName, CommonJsonParser::getManagedType<T>(), parent)
{
}
template<class T>
bool ARefAndParamComparator<T>::validateAndSetParameters(const JsonArray &parameters,
                                                         JsonArray &errors)
{
    RETURN_IF_FALSE(ABaseComparator::validateAndSetParameters(parameters, errors));

    QString errorStr;

    if(!BankJsonParser::getKnownValue<T>(parameters,
                                         valueParamName,
                                         _paramValue,
                                         &errorStr))
    {
        fillErrorArray(valueParamName, errorStr, errors);
        return false;
    }

    return true;
}

template<class T>
bool ARefAndParamComparator<T>::compare(const JsonArray &inputValues,
                                        bool &result,
                                        QString &errorLabel)
{
    T valueRef;

    RETURN_IF_FALSE(BankJsonParser::getKnownValue<T>(inputValues,
                                                     inputPinName,
                                                     valueRef,
                                                     &errorLabel));

    result = doCompareValues(valueRef, _paramValue);

    return true;
}
