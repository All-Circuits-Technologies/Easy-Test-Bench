// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "comparators/abasecomparator.hpp"

#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/parsers/commonjsonparser.hpp"
#include "testbedcore/testbedglobal.hpp"


/** @brief Base class for comparing the values of two references nodes together
    @warning Because Q_OBJECT and templated classes are incompatible, we can't add the MACRO in the
             class. Therefore, if you inherit this class and want to use Q_OBJECT functionalities
             be aware that some can fail in strangeous ways */
template<class T>
class ATwoRefsComparator : public ABaseComparator
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
        explicit ATwoRefsComparator(const QString &moduleClassName, QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~ATwoRefsComparator() override = default;

    public:
        /** @see ABaseComparator::initInputsFormat
            @note @ref inputSecPinName : any (This is the second element to compare the first input
                                              with) */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

    protected:
        /** @see ABaseComparator::compare */
        virtual bool compare(const JsonArray &inputValues,
                             bool &result,
                             QString &errorLabel) override;

    private:
        static const constexpr char * inputSecPinName = "inputSecPin";
        static const constexpr char * inputSecPinDesc = "This is the second element to compare the "
                                                        "first input with";
};

template<class T>
ATwoRefsComparator<T>::ATwoRefsComparator(const QString &moduleClassName, QObject *parent) :
    ABaseComparator(moduleClassName,
                    CommonJsonParser::getManagedType<T>(),
                    parent)
{
}

template<class T>
bool ATwoRefsComparator<T>::initInputsFormat(JsonArray &inputsFormat) const
{
    RETURN_IF_FALSE(ABaseComparator::initInputsFormat(inputsFormat));

    return addInputFormat(inputSecPinName, inputSecPinDesc, getTypeManaged(), inputsFormat);
}

template<class T>
bool ATwoRefsComparator<T>::compare(const JsonArray &inputValues, bool &result, QString &errorLabel)
{
    T valueFirstRef;

    RETURN_IF_FALSE(BankJsonParser::getKnownValue<T>(inputValues,
                                                     inputPinName,
                                                     valueFirstRef,
                                                     &errorLabel));

    T valueSecondRef;

    RETURN_IF_FALSE(BankJsonParser::getKnownValue<T>(inputValues,
                                                     inputSecPinName,
                                                     valueSecondRef,
                                                     &errorLabel));

    result = doCompareValues(valueFirstRef, valueSecondRef);

    return true;
}
