// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include "collectionutility/qvectorhelper.hpp"
#include "testbedcore/parsers/jsonparsersconstants.hpp"
#include "testbedcore/testbedglobal.hpp"
#include "numberutility/number.hpp"
#include "testbedcore/types/typemanaged.hpp"

class FileLoadingErrorHandler;
class QJsonObject;
class Unit;


/** @brief Useful methods to help the parsing from JSON elements to specific type for this
           library */
class CommonJsonParser
{
    public:
        /** @brief Get the unit from JSON object
            @note The method searches in the JSON object the attributes:
                  - JsonParsersConstants::Sequence::unitNameAttribute           and
                  - JsonParsersConstants::Sequence::unitPowerOf10NameAttribute
            @param jsonObject The json object to parse
            @param unit The unit to parse
            @param errorHandler An optional file loading error handler pointer
            @return True if no error occurs */
        static bool getUnit(const QJsonObject &jsonObject, Unit &unit,
                            FileLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Get a value managed by a library
            @param jsonObject The json object to parse
            @param attributeName The attribute name where to find the value
            @param typeManaged The type of the value to get
            @param value The value got from JSON object
            @param canBeEmpty If false and if the attribute is undefined (not found), the function
                              returns in error.
                              If true and if the attribute is undefined (not found), the function
                              returns ok and the string value will be NULL
            @param errorHandler An optional file loading error handler pointer
            @return True if no error occurs */
        static bool getValue(const QJsonObject &jsonObject,
                             const QString &attributeName,
                             TypeManaged::Enum typeManaged,
                             QVariant &value,
                             bool canBeEmpty = false,
                             FileLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Get a value managed by a library
            @param jsonObject The json object to parse
            @param attributeName The attribute name where to find the value
            @param value The value got from JSON object
            @param canBeEmpty If false and if the attribute is undefined (not found), the function
                              returns in error.
                              If true and if the attribute is undefined (not found), the function
                              returns ok and the string value will be NULL
            @param errorHandler An optional file loading error handler pointer
            @return True if no error occurs */
        static bool getValue(const QJsonObject &jsonObject,
                             const QString &attributeName,
                             QVariant &value,
                             bool canBeEmpty = false,
                             FileLoadingErrorHandler *errorHandler = nullptr);

        /** @brief Test if the value can be converted to the type given
            @param type The type to test with
            @param value The value to test
            @return True if the value can be converted to the type given */
        static bool testIfCanBeConverted(TypeManaged::Enum type, const QVariant &value);

        /** @brief Cast parameter format type given by plugins to TypeManaged
            @note To see the parameter format type managed see the method under
            @see ISequenceInstanceModule::getModuleParametersFormat
            @param paramType The parameter format type returned by plugins
            @param typeManaged The type managed parsed
            @param errorHandler An optional file loading error handler pointer
            @return True if no problem occurs */
        static bool castParamFormatTypeToManagedType(const QString &paramType,
                                                     TypeManaged::Enum &typeManaged,
                                                     FileLoadingErrorHandler *errorHandler
                                                     = nullptr);

        /** @brief Get the attribute name linked to the type given
            @note If the type is a QVector<T>, the method will return "values"
                  If the type is a T, the method will return "value"
            @param type The type to test
            @return The attribute name linked to the type given */
        static QString getValueAttributeName(TypeManaged::Enum type);

    public:
        /** @brief Test if the type linked has the form: T == QVector<...>
            @note Only usable with types managed @ref TypeManaged::Enum
            @return True if the type linked is a QVector<...> */
        template<class T>
        static bool isValueArray();

        /** @brief Transform the QVariant to the type given
            @note Only usable with types managed @ref TypeManaged::Enum
            @param varValue The variant to get the value from
            @param value The value got from the variant
            @return True if no problem occurs */
        template<class T>
        static bool getValue(const QVariant &varValue, T &value);

        /** @brief Transform the value in the type given to the QVariant
            @note Only usable with types managed @ref TypeManaged::Enum
            @param value The value to transform
            @param varValue The variant got from the value given
            @return True if no problem occurs */
        template<class T>
        static bool setValue(const T &value, QVariant &varValue);

        /** @brief Test if the QVariant can be converted to the type linked
            @note Only usable with types managed @ref TypeManaged::Enum
            @param varValue The variant to test
            @return True if no problem occurs */
        template<class T>
        static bool testIfCanBeConverted(const QVariant &varValue);

        /** @brief Test the type given to the method and return the TypeManaged linked
            @note Only usable with types managed @ref TypeManaged::Enum
            @return The TypeManaged linked to the type given */
        template<class T>
        static TypeManaged::Enum getManagedType();

        /** @brief Get the attribute name linked to the type linked
            @note If the type is a QVector<T>, the method will return "values"
                  If the type is a T, the method will return "value"
            @return The attribute name linked to the type given */
        template<class T>
        static QString getValueAttributeName();

    private:
        /** @brief Transform the QVariant to the type given
            @note This method only deals the types which are not lists
            @note Only usable with types managed @ref TypeManaged::Enum
            @param varValue The variant to get the value from
            @param value The value got from the variant
            @return True if no problem occurs */
        template<class T>
        static bool getSimpleValue(const QVariant &varValue, T &value);

        /** @brief Transform the QVariant to the type given
            @note This method only deals the list types
            @note Only usable with types managed @ref TypeManaged::Enum
            @param varValue The variant to get the value from
            @param value The value got from the variant
            @return True if no problem occurs */
        template<class T>
        static bool getVectorValue(const QVariant &varValue, T &value);

        /** @brief Transform the value in the type given to the QVariant
            @note This method only deals the types which are not lists
            @note Only usable with types managed @ref TypeManaged::Enum
            @param value The value to transform
            @param varValue The variant got from the value given
            @return True if no problem occurs */
        template<class T>
        static bool setSimpleValue(const T &value, QVariant &varValue);

        /** @brief Transform the value in the type given to the QVariant
            @note This method only deals the list types
            @note Only usable with types managed @ref TypeManaged::Enum
            @param value The value to transform
            @param varValue The variant got from the value given
            @return True if no problem occurs */
        template<class T>
        static bool setVectorValue(const T &value, QVariant &varValue);

    private:

        /** @brief Represent the max limit of the power of ten in order to get SI prefix
                   representation for unit */
        static const constexpr qint8 powerOfTenMinLimit = -24;

        /** @brief Represent the min limit of the power of ten in order to get SI prefix
                   representation for unit */
        static const constexpr qint8 powerOfTenMaxLimit =  24;
};

template<class T>
bool CommonJsonParser::isValueArray()
{
    return (std::is_same<QVector<Number>, T>::value     ||
            std::is_same<QVector<bool>, T>::value       ||
            std::is_same<QVector<QString>, T>::value);
}

template<class T>
bool CommonJsonParser::getValue(const QVariant &varValue,
                                T &value)
{
    static_assert(std::is_same<Number, T>::value            ||
                  std::is_same<bool, T>::value              ||
                  std::is_same<QString, T>::value           ||
                  std::is_same<QVector<Number>, T>::value   ||
                  std::is_same<QVector<bool>, T>::value     ||
                  std::is_same<QVector<QString>, T>::value,
                  "T must be a type managed");

    Q_UNUSED(varValue)
    Q_UNUSED(value)

    return false;
}

template<>
inline bool CommonJsonParser::getValue<bool>(const QVariant &varValue, bool &value)
{
    return getSimpleValue<bool>(varValue, value);
}

template<>
inline bool CommonJsonParser::getValue<Number>(const QVariant &varValue, Number &value)
{
    return getSimpleValue<Number>(varValue, value);
}

template<>
inline bool CommonJsonParser::getValue<QString>(const QVariant &varValue, QString &value)
{
    return getSimpleValue<QString>(varValue, value);
}

template<>
inline bool CommonJsonParser::getValue<QVector<Number>>(const QVariant &varValue,
                                                        QVector<Number> &value)
{
    return getVectorValue<QVector<Number>>(varValue, value);
}

template<>
inline bool CommonJsonParser::getValue<QVector<bool>>(const QVariant &varValue,
                                                      QVector<bool> &value)
{
    return getVectorValue<QVector<bool>>(varValue, value);
}

template<>
inline bool CommonJsonParser::getValue<QVector<QString>>(const QVariant &varValue,
                                                         QVector<QString> &value)
{
    return getVectorValue<QVector<QString>>(varValue, value);
}

template<class T>
bool CommonJsonParser::getSimpleValue(const QVariant &varValue, T &value)
{
    static_assert(std::is_same<Number, T>::value   ||
                  std::is_same<bool, T>::value     ||
                  std::is_same<QString, T>::value,
                  "T must be a type managed");

    if(!testIfCanBeConverted<T>(varValue))
    {
        qWarning() << "Can't convert the simple variant: " << varValue << ", to "
                   << typeid(T).name();
        return false;
    }

    value = varValue.value<T>();

    return true;
}

template<class T>
bool CommonJsonParser::getVectorValue(const QVariant &varValue, T &value)
{
    static_assert(std::is_same<QVector<Number>, T>::value   ||
                  std::is_same<QVector<bool>, T>::value     ||
                  std::is_same<QVector<QString>, T>::value,
                  "T must be a type managed");

    if(!testIfCanBeConverted<T>(varValue))
    {
        qWarning() << "Can't convert the variant array: " << varValue << ", to "
                   << typeid(T).name();
        return false;
    }

    QVector<QVariant> varValues = varValue.value<QVector<QVariant>>();

    if(!QVectorHelper::castFromQVariantVector(varValues, value))
    {
        qWarning() << "Can't cast the variant: " << varValue << ", to " << typeid(T).name();
        return false;
    }

    return true;
}

template<class T>
bool CommonJsonParser::setValue(const T &value, QVariant &varValue)
{
    static_assert(std::is_same<Number, T>::value            ||
                  std::is_same<bool, T>::value              ||
                  std::is_same<QString, T>::value           ||
                  std::is_same<QVector<Number>, T>::value   ||
                  std::is_same<QVector<bool>, T>::value     ||
                  std::is_same<QVector<QString>, T>::value,
                  "T must be a type managed");

    Q_UNUSED(value)
    Q_UNUSED(varValue)

    return false;
}

template<>
inline bool CommonJsonParser::setValue<Number>(const Number &value, QVariant &varValue)
{
    return setSimpleValue<Number>(value, varValue);
}

template<>
inline bool CommonJsonParser::setValue<bool>(const bool &value, QVariant &varValue)
{
    return setSimpleValue<bool>(value, varValue);
}

template<>
inline bool CommonJsonParser::setValue<QString>(const QString &value, QVariant &varValue)
{
    return setSimpleValue<QString>(value, varValue);
}

template<>
inline bool CommonJsonParser::setValue<QVector<Number>>(const QVector<Number> &value,
                                                        QVariant &varValue)
{
    return setVectorValue<QVector<Number>>(value, varValue);
}

template<>
inline bool CommonJsonParser::setValue<QVector<bool>>(const QVector<bool> &value,
                                                      QVariant &varValue)
{
    return setVectorValue<QVector<bool>>(value, varValue);
}

template<>
inline bool CommonJsonParser::setValue<QVector<QString>>(const QVector<QString> &value,
                                                         QVariant &varValue)
{
    return setVectorValue<QVector<QString>>(value, varValue);
}

template<class T>
bool CommonJsonParser::setSimpleValue(const T &value, QVariant &varValue)
{
    static_assert(std::is_same<Number, T>::value            ||
                  std::is_same<bool, T>::value              ||
                  std::is_same<QString, T>::value,
                  "T must be a type managed");

    varValue = QVariant::fromValue(value);
    return true;
}

template<class T>
bool CommonJsonParser::setVectorValue(const T &value, QVariant &varValue)
{
    static_assert(std::is_same<QVector<Number>, T>::value   ||
                  std::is_same<QVector<bool>, T>::value     ||
                  std::is_same<QVector<QString>, T>::value,
                  "T must be a type managed");

    QVector<QVariant> list;
    RETURN_IF_FALSE(QVectorHelper::castToQVariantVector(value, list));
    varValue = QVariant::fromValue(list);

    return true;
}

template<class T>
bool CommonJsonParser::testIfCanBeConverted(const QVariant &varValue)
{
    static_assert(std::is_same<Number, T>::value            ||
                  std::is_same<bool, T>::value              ||
                  std::is_same<QString, T>::value           ||
                  std::is_same<QVector<Number>, T>::value   ||
                  std::is_same<QVector<bool>, T>::value     ||
                  std::is_same<QVector<QString>, T>::value,
                  "T must be a type managed");

    if(std::is_same<QVector<Number>, T>::value ||
       std::is_same<QVector<bool>, T>::value ||
       std::is_same<QVector<QString>, T>::value)
    {
        RETURN_IF_FALSE(varValue.canConvert<QVector<QVariant>>());

        QVector<QVariant> values = varValue.value<QVector<QVariant>>();
        QVector<QVariant>::const_iterator iter = values.cbegin();

        for(;iter != values.cend(); ++iter)
        {
            if(std::is_same<QVector<Number>, T>::value)
            {
                RETURN_IF_FALSE(iter->canConvert<Number>());
            }
            else if(std::is_same<QVector<bool>, T>::value)
            {
                RETURN_IF_FALSE(iter->type() == QVariant::Bool);
            }
            else if(std::is_same<QVector<QString>, T>::value)
            {
                RETURN_IF_FALSE(iter->canConvert<QString>());
            }
        }

        return true;
    }

    if(std::is_same<bool, T>::value && (varValue.type() != QVariant::Bool))
    {
        return false;
    }

    return varValue.canConvert<T>();
}

template<class T>
TypeManaged::Enum CommonJsonParser::getManagedType()
{
    static_assert(std::is_same<Number, T>::value            ||
                  std::is_same<bool, T>::value              ||
                  std::is_same<QString, T>::value           ||
                  std::is_same<QVector<Number>, T>::value   ||
                  std::is_same<QVector<bool>, T>::value     ||
                  std::is_same<QVector<QString>, T>::value,
                  "T must be a type managed");

    TypeManaged::Enum managedType = TypeManaged::Unknown;

    if(std::is_same<Number, T>::value)
    {
        managedType = TypeManaged::Number;
    }
    else if(std::is_same<bool, T>::value)
    {
        managedType = TypeManaged::Boolean;
    }
    else if(std::is_same<QString, T>::value)
    {
        managedType = TypeManaged::String;
    }
    else if(std::is_same<QVector<Number>, T>::value)
    {
        managedType = TypeManaged::NumberList;
    }
    else if(std::is_same<QVector<bool>, T>::value)
    {
        managedType = TypeManaged::BooleanList;
    }
    else if(std::is_same<QVector<QString>, T>::value)
    {
        managedType = TypeManaged::StringList;
    }

    return managedType;
}

template<class T>
QString CommonJsonParser::getValueAttributeName()
{
    static_assert(std::is_same<Number, T>::value            ||
                  std::is_same<bool, T>::value              ||
                  std::is_same<QString, T>::value           ||
                  std::is_same<QVector<Number>, T>::value   ||
                  std::is_same<QVector<bool>, T>::value     ||
                  std::is_same<QVector<QString>, T>::value,
                  "T must be a type managed");

    QString valueAttr = JsonParsersConstants::Sequence::ValueNameAttribute;

    if(CommonJsonParser::isValueArray<T>())
    {
        valueAttr = JsonParsersConstants::Sequence::ValuesNameAttribute;
    }

    return valueAttr;
}
