// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "commonjsonparser.hpp"

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

#include "collectionutility/qvectorhelper.hpp"
#include "jsonutility/jsonhelper.hpp"
#include "jsonutility/jsontypedefs.hpp"
#include "testbedcore/errors/helpers/fileloadingerrorcausehelper.hpp"
#include "testbedcore/types/unit.hpp"


bool CommonJsonParser::getUnit(const QJsonObject &jsonObject, Unit &unit,
                               FileLoadingErrorHandler *errorHandler)
{
    QString unitSymbol;

    TEST_AND_RETURN_IF_FALSE(JsonHelper::getString(
                                 jsonObject,
                                 JsonParsersConstants::Sequence::UnitNameAttribute,
                                 unitSymbol,
                                 true),
                             FileLoadingErrorCauseHelper::addUnavailableAttributeError(
                                 JsonParsersConstants::Sequence::UnitNameAttribute,
                                 errorHandler););

    if(!unitSymbol.isEmpty())
    {
        QString unitPowerOfTen;

        TEST_AND_RETURN_IF_FALSE(JsonHelper::getString(
                                     jsonObject,
                                     JsonParsersConstants::Sequence::UnitPowerOf10NameAttribute,
                                     unitPowerOfTen,
                                     true),
                                 FileLoadingErrorCauseHelper::addUnavailableAttributeError(
                                     JsonParsersConstants::Sequence::UnitPowerOf10NameAttribute,
                                     errorHandler););

        qint8 powerOfTen = 0;

        if(!unitPowerOfTen.isEmpty())
        {
            bool ok = false;

            qint32 powerOfTenTmp = unitPowerOfTen.toInt(&ok);

            if(!ok)
            {
                FileLoadingErrorCauseHelper::addUnitPowerOfTenNotIntegerError(errorHandler);
                qWarning() << "The powerOfTen given is too big or it's not an integer: "
                           << unitPowerOfTen;
                return false;
            }

            if(powerOfTenTmp < powerOfTenMinLimit || powerOfTenTmp > powerOfTenMaxLimit)
            {
                FileLoadingErrorCauseHelper::addUnitPowerOfTenOverflowError(errorHandler);
                qWarning() << "The power of ten given is too big, overflow the limits of: "
                           << powerOfTenMinLimit << " and "
                           << powerOfTenMaxLimit;
                return false;
            }

            powerOfTen = static_cast<qint8>(powerOfTenTmp);
        }

        unit.setUnitType(unitSymbol);
        unit.setPowerOfTen(powerOfTen);
    }

    return true;
}

bool CommonJsonParser::getValue(const QJsonObject &jsonObject,
                                const QString &attributeName,
                                TypeManaged::Enum typeManaged,
                                QVariant &value,
                                bool canBeEmpty,
                                FileLoadingErrorHandler *errorHandler)
{
    if(typeManaged == TypeManaged::String)
    {
        QString strValue;

        TEST_AND_RETURN_IF_FALSE(JsonHelper::getString(
                                     jsonObject,
                                     attributeName,
                                     strValue,
                                     canBeEmpty),
                                 FileLoadingErrorCauseHelper::addInvalidValueError(
                                     TypeManaged::toString(typeManaged),
                                     attributeName,
                                     errorHandler););

        value = strValue;
    }
    else if(typeManaged == TypeManaged::Boolean)
    {
        bool boolValue = false;

        TEST_AND_RETURN_IF_FALSE(JsonHelper::getBoolean(
                                     jsonObject,
                                     attributeName,
                                     boolValue,
                                     canBeEmpty),
                                 FileLoadingErrorCauseHelper::addInvalidValueError(
                                     TypeManaged::toString(typeManaged),
                                     attributeName,
                                     errorHandler););
        value = boolValue;
    }
    else if(typeManaged == TypeManaged::Number)
    {
        QJsonValue::Type type;

        TEST_AND_RETURN_IF_FALSE(JsonHelper::getValueType(
                                     jsonObject,
                                     attributeName,
                                     type,
                                     canBeEmpty),
                                 FileLoadingErrorCauseHelper::addInvalidValueError(
                                     TypeManaged::toString(typeManaged),
                                     attributeName,
                                     errorHandler););
        Number number;

        if(type == QJsonValue::String)
        {
            QString strValue;

            TEST_AND_RETURN_IF_FALSE(JsonHelper::getString(
                                         jsonObject,
                                         attributeName,
                                         strValue,
                                         canBeEmpty),
                                     FileLoadingErrorCauseHelper::addInvalidValueError(
                                         TypeManaged::toString(typeManaged),
                                         attributeName,
                                         errorHandler););

            number = Number(strValue);
        }
        else if(type == QJsonValue::Double)
        {
            double dValue;

            TEST_AND_RETURN_IF_FALSE(JsonHelper::getDouble(
                                         jsonObject,
                                         attributeName,
                                         dValue,
                                         canBeEmpty),
                                     FileLoadingErrorCauseHelper::addInvalidValueError(
                                         TypeManaged::toString(typeManaged),
                                         attributeName,
                                         errorHandler););

            number = Number::fromDouble(dValue, Number::MaxAllowedCommaShift);
        }

        if(!number.isValid() && !canBeEmpty)
        {
            FileLoadingErrorCauseHelper::addInvalidValueError(TypeManaged::toString(typeManaged),
                                                              attributeName,
                                                              errorHandler);

            qWarning() << "The number given is invalid: " << attributeName;
            return false;
        }

        value = QVariant::fromValue(number);
    }
    else if(typeManaged == TypeManaged::StringList)
    {
        QStringList strValues;

        TEST_AND_RETURN_IF_FALSE(JsonHelper::getStringList(
                                     jsonObject,
                                     attributeName,
                                     strValues,
                                     canBeEmpty),
                                 FileLoadingErrorCauseHelper::addInvalidValueError(
                                     TypeManaged::toString(typeManaged),
                                     attributeName,
                                     errorHandler););

        JsonArray values;
        TEST_AND_RETURN_IF_FALSE(QVectorHelper::castToQVariantVector(strValues, values),
                                 FileLoadingErrorCauseHelper::addInvalidValueError(
                                     TypeManaged::toString(typeManaged),
                                     attributeName,
                                     errorHandler););

        value = QVariant::fromValue(values);
    }
    else if(typeManaged == TypeManaged::BooleanList)
    {
        QList<bool> strValues;

        TEST_AND_RETURN_IF_FALSE(JsonHelper::getBooleanList(
                                     jsonObject,
                                     attributeName,
                                     strValues,
                                     canBeEmpty),
                                 FileLoadingErrorCauseHelper::addInvalidValueError(
                                     TypeManaged::toString(typeManaged),
                                     attributeName,
                                     errorHandler););

        JsonArray values;
        TEST_AND_RETURN_IF_FALSE(QVectorHelper::castToQVariantVector(strValues, values),
                                 FileLoadingErrorCauseHelper::addInvalidValueError(
                                     TypeManaged::toString(typeManaged),
                                     attributeName,
                                     errorHandler););

        value = QVariant::fromValue(values);
    }
    else if(typeManaged == TypeManaged::NumberList)
    {
        QJsonArray array;
        TEST_AND_RETURN_IF_FALSE(JsonHelper::getArray(jsonObject, attributeName, array, canBeEmpty),
                                 FileLoadingErrorCauseHelper::addInvalidValueError(
                                     TypeManaged::toString(typeManaged),
                                                           attributeName,
                                                           errorHandler););

        JsonArray numbers;

        QJsonArray::const_iterator iter = array.constBegin();
        for(; iter != array.constEnd(); ++iter)
        {
            Number number;

            if(iter->type() == QJsonValue::String)
            {
                number = Number(iter->toString());
            }
            else if(iter->type() == QJsonValue::Double)
            {
                number = Number::fromDouble(iter->toDouble(), Number::MaxAllowedCommaShift);
            }

            if(!number.isValid())
            {
                qWarning() << "The number given is invalid: " << attributeName;
                FileLoadingErrorCauseHelper::addInvalidValueError(
                    TypeManaged::toString(typeManaged),
                    attributeName,
                    errorHandler);
                return false;
            }

            numbers.append(QVariant::fromValue(number));
        }

        value = QVariant::fromValue(numbers);
    }
    else
    {
        FileLoadingErrorCauseHelper::addUnknownValueTypeError(TypeManaged::toString(typeManaged),
                                                              attributeName,
                                                              errorHandler);
        qWarning() << "The type is unknown can't get value: " << attributeName;
        return false;
    }

    return true;
}

bool CommonJsonParser::getValue(const QJsonObject &jsonObject,
                                const QString &attributeName,
                                QVariant &value,
                                bool canBeEmpty,
                                FileLoadingErrorHandler *errorHandler)
{
    QJsonValue jsonValue = jsonObject.value(attributeName);

    if(jsonValue.isNull())
    {
        if(!canBeEmpty)
        {
            FileLoadingErrorCauseHelper::addUnavailableValueError(attributeName, errorHandler);
            qWarning() << "There is no value at attribute: " << attributeName;
            return false;
        }

        value = QVariant();
        return true;
    }

    TypeManaged::Enum typeManaged = TypeManaged::Unknown;

    if(jsonValue.isBool())
    {
        typeManaged = TypeManaged::Boolean;
    }
    else if(jsonValue.isDouble())
    {
        typeManaged = TypeManaged::Number;
    }
    else if(jsonValue.isString())
    {
        // Can be number or string
        Number tryToTest = Number(jsonValue.toString());

        if(tryToTest.isValid())
        {
            typeManaged = TypeManaged::Number;
        }
        else
        {
            typeManaged = TypeManaged::String;
        }
    }
    else if(jsonValue.isArray())
    {
        QJsonArray array = jsonValue.toArray();
        QJsonArray::const_iterator iter = array.constBegin();

        for(; iter != array.constEnd(); iter++)
        {
            if(iter->isBool() && (typeManaged == TypeManaged::BooleanList ||
                                  typeManaged == TypeManaged::Unknown))
            {
                typeManaged = TypeManaged::BooleanList;
            }
            else if(iter->isDouble() && (typeManaged == TypeManaged::NumberList ||
                                         typeManaged == TypeManaged::Unknown))
            {
                typeManaged = TypeManaged::NumberList;
            }
            else if(iter->isString() && (typeManaged == TypeManaged::StringList ||
                                         typeManaged == TypeManaged::NumberList ||
                                         typeManaged == TypeManaged::Unknown))
            {
                // Can be number or string
                Number tryToTest = Number(jsonValue.toString());

                if(tryToTest.isValid() && (typeManaged == TypeManaged::NumberList ||
                                           typeManaged == TypeManaged::Unknown))
                {
                    typeManaged = TypeManaged::NumberList;
                }
                else if(typeManaged == TypeManaged::StringList ||
                        typeManaged == TypeManaged::Unknown)
                {
                    typeManaged = TypeManaged::StringList;
                }
                else
                {
                    typeManaged = TypeManaged::Unknown;
                }
            }

            if(typeManaged == TypeManaged::Unknown)
            {
                qWarning() << "All the elements in the list haven't the same type, or there is"
                              " a null element";
                break;
            }
        }
    }

    return getValue(jsonObject, attributeName, typeManaged, value, canBeEmpty, errorHandler);
}

bool CommonJsonParser::testIfCanBeConverted(TypeManaged::Enum type, const QVariant &value)
{
    switch(type)
    {
        case TypeManaged::Boolean:
            return testIfCanBeConverted<bool>(value);

        case TypeManaged::String:
            return testIfCanBeConverted<QString>(value);

        case TypeManaged::Number:
            return testIfCanBeConverted<Number>(value);

        case TypeManaged::BooleanList:
            return testIfCanBeConverted<QVector<bool>>(value);

        case TypeManaged::StringList:
            return testIfCanBeConverted<QVector<QString>>(value);

        case TypeManaged::NumberList:
            return testIfCanBeConverted<QVector<Number>>(value);

        case TypeManaged::Unknown:
            return false;
    }

    return false;
}

bool CommonJsonParser::castParamFormatTypeToManagedType(const QString &paramType,
                                                        TypeManaged::Enum &typeManaged,
                                                        FileLoadingErrorHandler *errorHandler)
{
    typeManaged = TypeManaged::Unknown;

    if(paramType == JsonParsersConstants::Hmi::StringTypeAttribute  ||
       paramType == JsonParsersConstants::Hmi::RadioListTypeAttribute)
    {
        typeManaged = TypeManaged::String;
    }
    else if(paramType == JsonParsersConstants::Hmi::BoolTypeAttribute)
    {
        typeManaged = TypeManaged::Boolean;
    }
    else if(paramType == JsonParsersConstants::Hmi::NumberTypeAttribute ||
            paramType == JsonParsersConstants::Hmi::DateTypeAttribute   ||
            paramType == JsonParsersConstants::Hmi::TimeTypeAttribute   ||
               paramType == JsonParsersConstants::Hmi::ColorTypeAttribute)
    {
        typeManaged = TypeManaged::Number;
    }
    else if(paramType == JsonParsersConstants::Hmi::SelectTypeAttribute         ||
            paramType == JsonParsersConstants::Hmi::CheckBoxListTypeAttribute   ||
            paramType == JsonParsersConstants::Hmi::StringListTypeAttribute)
    {
        typeManaged = TypeManaged::StringList;
    }
    else if(paramType == JsonParsersConstants::Hmi::BoolListTypeAttribute)
    {
        typeManaged = TypeManaged::BooleanList;
    }
    else if(paramType == JsonParsersConstants::Hmi::NumberListTypeAttribute)
    {
        typeManaged = TypeManaged::NumberList;
    }
    else
    {
        FileLoadingErrorCauseHelper::addUnknownTypeManagedError(paramType, errorHandler);
        qWarning() << "The parameter format type isn't known: " << paramType;
        return false;
    }

    return true;
}

QString CommonJsonParser::getValueAttributeName(TypeManaged::Enum type)
{
    QString valueAttr = JsonParsersConstants::Sequence::ValueNameAttribute;

    if(TypeManaged::isList(type))
    {
        valueAttr = JsonParsersConstants::Sequence::ValuesNameAttribute;
    }

    return valueAttr;
}
