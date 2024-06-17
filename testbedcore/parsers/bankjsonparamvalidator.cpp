// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "bankjsonparamvalidator.hpp"

#include <QDate>
#include <QDateTime>
#include <QRegularExpression>
#include <QTime>

#include "bankjsonparser.hpp"
#include "testbedcore/testbedglobal.hpp"


bool BankJsonParamValidator::validateModuleParameters(const JsonArray &parametersFormat,
                                                      const JsonArray &parametersValues,
                                                      JsonArray &errorArray)
{
    JsonArray::const_iterator citer = parametersFormat.cbegin();
    for(; citer != parametersFormat.cend(); ++citer)
    {
        if(!citer->canConvert<JsonObject>())
        {
            fillErrorArray("Wrong parameter format", errorArray);
            qWarning() << "Can't convert the variant: " << citer->toString() << ", to JsonObject";
            return false;
        }

        JsonObject object = citer->value<JsonObject>();
        RETURN_IF_FALSE(BankJsonParamValidator::validateParameterValue(object,
                                                                       parametersValues,
                                                                       errorArray));
    }

    return true;
}

void BankJsonParamValidator::fillErrorArray(const QString &parameterName,
                                            const QString &errorStr,
                                            JsonArray &errorArray)
{
    errorArray.append(QVariant::fromValue(
        JsonObject({ { JsonParsersConstants::Generic::NameAttribute, parameterName },
                     { JsonParsersConstants::Hmi::ErrorLabelAttribute, errorStr } })) );
}

void BankJsonParamValidator::fillErrorArray(const QString &errorStr,
                                            JsonArray &errorArray)
{
    fillErrorArray(JsonParsersConstants::Hmi::GenericErrorAttribute, errorStr, errorArray);
}

bool BankJsonParamValidator::validateParameterValue(const JsonObject &parameterFormat,
                                                    const JsonArray &parametersValues,
                                                    JsonArray &errorArray)
{
    QString error;

    QString objectName;
    if(!BankJsonParser::getObjSpecificAttributeValue(parameterFormat,
                                                     JsonParsersConstants::Generic::NameAttribute,
                                                     objectName,
                                                     QString(),
                                                     &error))
    {
        fillErrorArray(error, errorArray);
        return false;
    }

    QString type;
    if(!BankJsonParser::getObjSpecificAttributeValue(parameterFormat,
                                                     JsonParsersConstants::Generic::TypeName,
                                                     type,
                                                     objectName,
                                                     &error))
    {
        fillErrorArray(objectName, error, errorArray);
        return false;
    }

    bool required = false;

    if(parameterFormat.contains(JsonParsersConstants::Hmi::RequiredAttribute))
    {
        if(!BankJsonParser::getObjSpecificAttributeValue(
                parameterFormat,
                JsonParsersConstants::Hmi::RequiredAttribute,
                required,
                objectName,
                &error))
        {
            fillErrorArray(objectName, error, errorArray);
            return false;
        }
    }

    JsonObject objectFound;
    if(!BankJsonParser::findObject(parametersValues, objectName, objectFound, !required))
    {
        fillErrorArray(objectName,
                       QString("Can't find the object named: %1").arg(objectName),
                       errorArray);
        return false;
    }

    if(objectFound.isEmpty())
    {
        // If we are here, it means that the object hasn't been found and it wasn't required
        return true;
    }

    if(type == JsonParsersConstants::Hmi::StringTypeAttribute           ||
       type == JsonParsersConstants::Hmi::DateTypeAttribute             ||
       type == JsonParsersConstants::Hmi::RadioListTypeAttribute        ||
       type == JsonParsersConstants::Hmi::RadioListButtonsTypeAttribute ||
       type == JsonParsersConstants::Hmi::OkButtonTypeAttribute         ||
       type == JsonParsersConstants::Hmi::ColorTypeAttribute)
    {
        QString value;
        RETURN_IF_FALSE(getStringValue(objectFound, objectName, value, errorArray, required));

        if(value.isNull())
        {
            // If we are here, it means that the value wasn't required and its value is equal to
            // null
            return true;
        }

        if(type == JsonParsersConstants::Hmi::StringTypeAttribute)
        {
            return validateStringParameterValue(parameterFormat, objectName, value, errorArray);
        }

        if(type == JsonParsersConstants::Hmi::DateTypeAttribute)
        {
            return validateDateParameterValue(parameterFormat, objectName, value, errorArray);
        }

        if(type == JsonParsersConstants::Hmi::RadioListTypeAttribute)
        {
            return validateListElemsParameterValue(parameterFormat,
                                                   objectName,
                                                   JsonParsersConstants::Hmi::ListAttribute,
                                                   value,
                                                   errorArray);
        }

        if(type == JsonParsersConstants::Hmi::RadioListButtonsTypeAttribute)
        {
            return validateListElemsParameterValue(parameterFormat,
                                                   objectName,
                                                   JsonParsersConstants::Hmi::ListAttribute,
                                                   value,
                                                   errorArray);
        }

        if(type == JsonParsersConstants::Hmi::OkButtonTypeAttribute)
        {
            return validateOkButtonParameterValue(parameterFormat, objectName, value, errorArray);
        }
        
        if(type == JsonParsersConstants::Hmi::ColorTypeAttribute)
        {
            return validateColorParameterValue(parameterFormat, objectName, value, errorArray);
        }
    }
    
    if(type == JsonParsersConstants::Hmi::BoolTypeAttribute)
    {
        // No more elements to test
        bool value;
        return getBoolValue(objectFound, objectName, value, errorArray, required);
    }

    if(type == JsonParsersConstants::Hmi::BoolListTypeAttribute)
    {
        QVector<bool> values;
        return getArrayBoolValue(objectFound, objectName, values, errorArray, required);
    }

    if(type == JsonParsersConstants::Hmi::NumberTypeAttribute   ||
       type == JsonParsersConstants::Hmi::TimeTypeAttribute     ||
       type == JsonParsersConstants::Hmi::DateTimeTypeAttribute)
    {
        Number value;
        RETURN_IF_FALSE(getNumberValue(objectFound, objectName, value, errorArray, required));

        if(!value.isValid())
        {
            // If we are here, it means that the value wasn't required and its value is equal to
            // null
            return true;
        }

        if(type == JsonParsersConstants::Hmi::NumberTypeAttribute)
        {
            return validateNumberParameterValue(parameterFormat, objectName, value, errorArray);
        }

        if(type == JsonParsersConstants::Hmi::TimeTypeAttribute)
        {
            return validateTimeParameterValue(parameterFormat, objectName, value, errorArray);
        }

        if(type == JsonParsersConstants::Hmi::DateTimeTypeAttribute)
        {
            return validateDateTimeParameterValue(parameterFormat, objectName, value, errorArray);
        }
    }

    if(type == JsonParsersConstants::Hmi::NumberListTypeAttribute)
    {
        QVector<Number> values;
        RETURN_IF_FALSE(getArrayNumberValue(objectFound, objectName, values, errorArray, required));

        return validateNumberListParameterValue(parameterFormat, objectName, values, errorArray);
    }

    if(type == JsonParsersConstants::Hmi::StringListTypeAttribute   ||
       type == JsonParsersConstants::Hmi::SelectTypeAttribute       ||
       type == JsonParsersConstants::Hmi::CheckBoxListTypeAttribute)
    {
        QVector<QString> values;
        RETURN_IF_FALSE(getArrayStringValue(objectFound, objectName, values, errorArray, required));

        if(type == JsonParsersConstants::Hmi::StringListTypeAttribute)
        {
            return validateStringListParameterValue(parameterFormat,
                                                    objectName,
                                                    values,
                                                    errorArray);
        }

        if(values.isEmpty())
        {
            // If we are here, it means that the value wasn't required and its values is empty
            return true;
        }

        if(type == JsonParsersConstants::Hmi::SelectTypeAttribute)
        {
            return validateSelectParameterValue(parameterFormat, objectName, values, errorArray);
        }

        if(type == JsonParsersConstants::Hmi::CheckBoxListTypeAttribute)
        {
            return validateListElemsParameterValue(parameterFormat,
                                                   objectName,
                                                   JsonParsersConstants::Hmi::ListAttribute,
                                                   values,
                                                   errorArray);
        }
    }

    qWarning() << "The parameter type is unknown: " << type;
    fillErrorArray(objectName, QString("The parameter type is unknown: %1").arg(type), errorArray);
    return false;
}

bool BankJsonParamValidator::validateStringParameterValue(const JsonObject &parameterFormat,
                                                          const QString &paramName,
                                                          const QString &value,
                                                          JsonArray &errorArray)
{
    QString error;

    if(parameterFormat.contains(JsonParsersConstants::Hmi::MinLengthAttribute))
    {
        int minLength = 0;
        RETURN_IF_FALSE(getIntValue(parameterFormat[JsonParsersConstants::Hmi::MinLengthAttribute],
                                    paramName,
                                    JsonParsersConstants::Hmi::MinLengthAttribute,
                                    minLength,
                                    errorArray));

        if(value.length() < minLength)
        {
            qWarning() << "The length of the string: " << value << " of the object: " << paramName
                       << ", is lesser than the " << JsonParsersConstants::Hmi::MinLengthAttribute
                       << ": " << QString::number(minLength);
            fillErrorArray(paramName,
                           QString("The value entered: %1 is under the minimum value given: %2")
                               .arg(value).arg(minLength),
                           errorArray);
            return false;
        }
    }

    if(parameterFormat.contains(JsonParsersConstants::Hmi::MaxLengthAttribute))
    {
        int maxLength = 0;
        RETURN_IF_FALSE(getIntValue(parameterFormat[JsonParsersConstants::Hmi::MaxLengthAttribute],
                                    paramName,
                                    JsonParsersConstants::Hmi::MaxLengthAttribute,
                                    maxLength,
                                    errorArray));

        if(value.length() > maxLength)
        {
            qWarning() << "The length of the string: " << value << " of the object: " << paramName
                       << ", is greather than the "
                       << JsonParsersConstants::Hmi::MaxLengthAttribute << ": "
                       << QString::number(maxLength);
            fillErrorArray(paramName,
                           QString("The value entered: %1 is upper the maximum value given: %2")
                               .arg(value).arg(maxLength),
                           errorArray);
            return false;
        }
    }

    if(parameterFormat.contains(JsonParsersConstants::Hmi::PatternRegexAttribute))
    {
        QString pattern =
                    parameterFormat[JsonParsersConstants::Hmi::PatternRegexAttribute].toString();

        QRegularExpression regexp(pattern);

        if(!regexp.isValid())
        {
            error = QString("Invalid regexp: %1").arg(regexp.errorString());
            qWarning() << error;
            fillErrorArray(paramName, error, errorArray);
            return false;
        }

        if(!regexp.match(value).hasMatch())
        {
            qWarning() << "The value: " << value << ", of the object: " << paramName
                       << ", doesn't match the regular expression: " << pattern;
            fillErrorArray(paramName,
                           QString("The value: %1, doesn't match the regular expression: %2")
                               .arg(value, pattern),
                           errorArray);
            return false;
        }
    }

    return true;
}

bool BankJsonParamValidator::validateStringListParameterValue(const JsonObject &parameterFormat,
                                                              const QString &paramName,
                                                              const QVector<QString> &values,
                                                              JsonArray &errorArray)
{
    if(parameterFormat.contains(JsonParsersConstants::Hmi::MinListLengthAttribute))
    {
        int minListLength = 0;
        RETURN_IF_FALSE(getIntValue(
            parameterFormat[JsonParsersConstants::Hmi::MinListLengthAttribute],
            paramName,
            JsonParsersConstants::Hmi::MinListLengthAttribute,
            minListLength,
            errorArray));

        if(values.length() < minListLength)
        {
            qWarning() << "The length of the string list: " << values << " of the object: "
                       << paramName << ", is lesser than the "
                       << JsonParsersConstants::Hmi::MinListLengthAttribute
                       << ": " << QString::number(minListLength);
            fillErrorArray(paramName,
                           QString("The values entered are under the minimum expected values "
                                   "given: %1").arg(minListLength),
                           errorArray);
            return false;
        }
    }

    if(parameterFormat.contains(JsonParsersConstants::Hmi::MaxListLengthAttribute))
    {
        int maxListLength = 0;
        RETURN_IF_FALSE(getIntValue(
            parameterFormat[JsonParsersConstants::Hmi::MaxListLengthAttribute],
            paramName,
            JsonParsersConstants::Hmi::MaxListLengthAttribute,
            maxListLength,
            errorArray));

        if(values.length() > maxListLength)
        {
            qWarning() << "The length of the string: " << values << " of the object: " << paramName
                       << ", is greather than the "
                       << JsonParsersConstants::Hmi::MaxListLengthAttribute << ": "
                       << QString::number(maxListLength);
            fillErrorArray(paramName,
                           QString("The values entered are above the maximum value given: %1")
                               .arg(maxListLength),
                           errorArray);
            return false;
        }
    }

    for(const QString &value : values)
    {
        RETURN_IF_FALSE(validateStringParameterValue(parameterFormat,
                                                     paramName,
                                                     value,
                                                     errorArray));
    }

    return true;
}

bool BankJsonParamValidator::validateNumberParameterValue(const JsonObject &parameterFormat,
                                                          const QString &paramName,
                                                          const Number &value,
                                                          JsonArray &errorArray)
{
    QString error;

    if(parameterFormat.contains(JsonParsersConstants::Hmi::MinAttribute))
    {
        Number min;
        if(!BankJsonParser::getObjSpecificAttributeValue(parameterFormat,
                                                         JsonParsersConstants::Hmi::MinAttribute,
                                                         min,
                                                         paramName,
                                                         &error))
        {
            qWarning() << error;
            fillErrorArray(paramName, error, errorArray);
            return false;
        }

        if(!min.isValid())
        {
            qWarning() << "The attribute: " << QString(JsonParsersConstants::Hmi::MinAttribute)
                       << ", of the object: " << paramName << ", is invalid";
            fillErrorArray(paramName,
                           QString("The attribute: %1, is invalid")
                               .arg(JsonParsersConstants::Hmi::MinAttribute),
                           errorArray);
            return false;
        }

        if(value < min)
        {
            qWarning() << "The number: " << value << " of the object: " << paramName
                       << ", is lesser than the " << JsonParsersConstants::Hmi::MinAttribute
                       << ": " << min;
            fillErrorArray(paramName,
                           QString("The number: %1, is lesser than the %2: %3")
                               .arg(value).arg(JsonParsersConstants::Hmi::MinAttribute).arg(min),
                           errorArray);
            return false;
        }
    }

    if(parameterFormat.contains(JsonParsersConstants::Hmi::MaxAttribute))
    {
        Number max;
        if(!BankJsonParser::getObjSpecificAttributeValue(parameterFormat,
                                                         JsonParsersConstants::Hmi::MaxAttribute,
                                                         max,
                                                         paramName,
                                                         &error))
        {
            qWarning() << error;
            fillErrorArray(paramName, error, errorArray);
            return false;
        }

        if(!max.isValid())
        {
            qWarning() << "The attribute: " << QString(JsonParsersConstants::Hmi::MaxAttribute)
                       << ", of the object: " << paramName << ", is invalid";
            fillErrorArray(paramName,
                           QString("The attribute: %1, is invalid")
                               .arg(JsonParsersConstants::Hmi::MaxAttribute),
                           errorArray);
            return false;
        }

        if(value > max)
        {
            qWarning() << "The number: " << value << " of the object: " << paramName
                       << ", is greather than the " << JsonParsersConstants::Hmi::MaxAttribute
                       << ": " << max;
            fillErrorArray(paramName,
                           QString("The number: %1, is greather than the %2: %3")
                               .arg(value).arg(JsonParsersConstants::Hmi::MaxAttribute).arg(max),
                           errorArray);
            return false;
        }
    }

    if(parameterFormat.contains(JsonParsersConstants::Hmi::StepAttribute))
    {
        Number step;
        if(!BankJsonParser::getObjSpecificAttributeValue(parameterFormat,
                                                         JsonParsersConstants::Hmi::StepAttribute,
                                                         step,
                                                         paramName,
                                                         &error))
        {
            qWarning() << error;
            fillErrorArray(paramName, error, errorArray);
            return false;
        }

        if(!step.isValid())
        {
            qWarning() << "The attribute: " << QString(JsonParsersConstants::Hmi::StepAttribute)
                       << ", of the object: " << paramName << ", is invalid";
            fillErrorArray(paramName,
                           QString("The attribute: %1, is invalid")
                               .arg(JsonParsersConstants::Hmi::StepAttribute),
                           errorArray);
            return false;
        }

        if(step.getShiftNbAfterComma() < value.getShiftNbAfterComma())
        {
            qWarning() << "The value number: " << value << ", of the object: " << paramName
                       << " has a number of shift superior of the step value: " << step;
            fillErrorArray(paramName,
                           QString("The value number: %1 has a number of shift superior of the "
                                   "step value: %2").arg(value).arg(step),
                           errorArray);
            return false;
        }

        if((value % step) != Number(0))
        {
            qWarning() << "The value number: " << value << ", of the object: " << paramName <<
                          " is not a factor of: " << step;
            fillErrorArray(paramName,
                           QString("The value number: %1 is not a factor of: %2")
                               .arg(value).arg(step),
                           errorArray);
            return false;
        }
    }

    return true;
}

bool BankJsonParamValidator::validateNumberListParameterValue(const JsonObject &parameterFormat,
                                                              const QString &paramName,
                                                              const QVector<Number> &values,
                                                              JsonArray &errorArray)
{
    for(auto citer = values.cbegin(); citer != values.cend(); ++citer)
    {
        RETURN_IF_FALSE(validateNumberParameterValue(parameterFormat,
                                                     paramName,
                                                     *citer,
                                                     errorArray));
    }

    return true;
}

bool BankJsonParamValidator::validateDateParameterValue(const JsonObject &parameterFormat,
                                                        const QString &paramName,
                                                        const QString &value,
                                                        JsonArray &errorArray)
{
    QDate dateValue = QDate::fromString(value, Qt::ISODateWithMs);

    if(!dateValue.isValid())
    {
        qWarning() << "The date parameter value is not a valid ISO Date: " << value;
        fillErrorArray(paramName, "The date parameter value is not a valid ISODate", errorArray);
        return false;
    }

    QString error;

    if(parameterFormat.contains(JsonParsersConstants::Hmi::MinAttribute))
    {
        QString min;
        if(!BankJsonParser::getObjSpecificAttributeValue(parameterFormat,
                                                         JsonParsersConstants::Hmi::MinAttribute,
                                                         min,
                                                         paramName,
                                                         &error))
        {
            qWarning() << error;
            fillErrorArray(paramName, error, errorArray);
            return false;
        }

        QDate dateMin = QDate::fromString(min, Qt::ISODateWithMs);

        if(!dateMin.isValid())
        {
            qWarning() << "The date " << JsonParsersConstants::Hmi::MinAttribute
                       << " attribute is not a valid ISO Date: " << value;
            fillErrorArray(paramName,
                           QString("The date %1 attribute is not a valid ISODate")
                               .arg(JsonParsersConstants::Hmi::MinAttribute),
                           errorArray);
            return false;
        }

        if(dateValue < dateMin)
        {
            qWarning() << "The date: " << value << " of the object: " << paramName
                       << ", is lesser than the " << JsonParsersConstants::Hmi::MinAttribute
                       << ": " << min;
            fillErrorArray(paramName,
                           QString("The number: %1, is lesser than the %2: %3")
                               .arg(value).arg(JsonParsersConstants::Hmi::MinAttribute).arg(min),
                           errorArray);
            return false;
        }
    }

    if(parameterFormat.contains(JsonParsersConstants::Hmi::MaxAttribute))
    {
        QString max;
        if(!BankJsonParser::getObjSpecificAttributeValue(parameterFormat,
                                                         JsonParsersConstants::Hmi::MaxAttribute,
                                                         max,
                                                         paramName,
                                                         &error))
        {
            qWarning() << error;
            fillErrorArray(paramName, error, errorArray);
            return false;
        }

        QDate dateMax = QDate::fromString(max, Qt::ISODateWithMs);

        if(!dateMax.isValid())
        {
            qWarning() << "The date " << JsonParsersConstants::Hmi::MaxAttribute
                       << " attribute is not a valid ISO Date: " << value;
            fillErrorArray(paramName,
                           QString("The date %1 attribute is not a valid ISODate")
                               .arg(JsonParsersConstants::Hmi::MaxAttribute),
                           errorArray);
            return false;
        }

        if(dateValue > dateMax)
        {
            qWarning() << "The number: " << value << " of the object: " << paramName
                       << ", is greather than the " << JsonParsersConstants::Hmi::MaxAttribute
                       << ": " << max;
            fillErrorArray(paramName,
                           QString("The number: %1, is greather than the %2: %3")
                               .arg(value).arg(JsonParsersConstants::Hmi::MaxAttribute).arg(max),
                           errorArray);
            return false;
        }
    }

    return true;
}

bool BankJsonParamValidator::validateTimeParameterValue(const JsonObject &parameterFormat,
                                                   const QString &paramName,
                                                   const Number &value,
                                                   JsonArray &errorArray)
{
    bool ok = false;
    int timeInt = value.toInt32(&ok);

    if(!ok)
    {
        qWarning() << "The number given is not suitable in order to be parsed to QTime: " << value;
        fillErrorArray(paramName, "Wrong time value", errorArray);
        return false;
    }

    QTime timeValue = QTime::fromMSecsSinceStartOfDay(timeInt);

    if(!timeValue.isValid())
    {
        qWarning() << "The time parameter value is not a valid QTime: " << value;
        fillErrorArray(paramName, "The time parameter value is not a valid QTime", errorArray);
        return false;
    }

    QString error;

    if(parameterFormat.contains(JsonParsersConstants::Hmi::MinAttribute))
    {
        Number min;
        if(!BankJsonParser::getObjSpecificAttributeValue(parameterFormat,
                                                         JsonParsersConstants::Hmi::MinAttribute,
                                                         min,
                                                         paramName,
                                                         &error))
        {
            qWarning() << error;
            fillErrorArray(paramName, error, errorArray);
            return false;
        }

        int timeMinInt = min.toInt32(&ok);

        if(!ok)
        {
            qWarning() << "The " << JsonParsersConstants::Hmi::MinAttribute << " number given is "
                       << "not suitable in order to be parsed to QTime: " << timeMinInt;
            fillErrorArray(paramName,
                           QString("Wrong %1 time value")
                               .arg(JsonParsersConstants::Hmi::MinAttribute),
                           errorArray);
            return false;
        }

        QTime timeMin = QTime::fromMSecsSinceStartOfDay(timeMinInt);

        if(!timeMin.isValid())
        {
            qWarning() << "The time " << JsonParsersConstants::Hmi::MinAttribute
                       << " attribute is not a valid QTime: " << timeMin;
            fillErrorArray(paramName,
                           QString("The time %1 attribute is not a valid QTime")
                               .arg(JsonParsersConstants::Hmi::MinAttribute),
                           errorArray);
            return false;
        }

        if(timeValue < timeMin)
        {
            qWarning() << "The time: " << value << " of the object: " << paramName
                       << ", is lesser than the " << JsonParsersConstants::Hmi::MinAttribute
                       << ": " << min;
            fillErrorArray(paramName,
                           QString("The number: %1, is lesser than the %2: %3")
                               .arg(value).arg(JsonParsersConstants::Hmi::MinAttribute).arg(min),
                           errorArray);
            return false;
        }
    }

    if(parameterFormat.contains(JsonParsersConstants::Hmi::MaxAttribute))
    {
        Number max;
        if(!BankJsonParser::getObjSpecificAttributeValue(parameterFormat,
                                                         JsonParsersConstants::Hmi::MaxAttribute,
                                                         max,
                                                         paramName,
                                                         &error))
        {
            qWarning() << error;
            fillErrorArray(paramName, error, errorArray);
            return false;
        }

        int timeMaxInt = max.toInt32(&ok);

        if(!ok)
        {
            qWarning() << "The " << JsonParsersConstants::Hmi::MaxAttribute << " number given is "
                       << "not suitable in order to be parsed to QTime: " << timeMaxInt;
            fillErrorArray(paramName,
                           QString("Wrong %1 time value")
                               .arg(JsonParsersConstants::Hmi::MaxAttribute),
                           errorArray);
            return false;
        }

        QTime timeMax = QTime::fromMSecsSinceStartOfDay(timeMaxInt);

        if(!timeMax.isValid())
        {
            qWarning() << "The time " << JsonParsersConstants::Hmi::MaxAttribute
                       << " attribute is not a valid QTime: " << timeMax;
            fillErrorArray(paramName,
                           QString("The time %1 attribute is not a valid QTime")
                               .arg(JsonParsersConstants::Hmi::MaxAttribute),
                           errorArray);
            return false;
        }

        if(timeValue < timeMax)
        {
            qWarning() << "The time: " << value << " of the object: " << paramName
                       << ", is greather than the " << JsonParsersConstants::Hmi::MaxAttribute
                       << ": " << max;
            fillErrorArray(paramName,
                           QString("The number: %1, is greather than the %2: %3")
                               .arg(value).arg(JsonParsersConstants::Hmi::MaxAttribute).arg(max),
                           errorArray);
            return false;
        }
    }

    return true;
}

bool BankJsonParamValidator::validateDateTimeParameterValue(const JsonObject &parameterFormat,
                                                            const QString &paramName,
                                                            const Number &value,
                                                            JsonArray &errorArray)
{
    bool ok = false;
    qint64 dateTimeInt = value.toInt64(&ok);

    if(!ok)
    {
        qWarning() << "The number given is not suitable in order to be parsed to QDateTime: "
                   << value;
        fillErrorArray(paramName, "Wrong time value", errorArray);
        return false;
    }

    QDateTime dateTimeValue = QDateTime::fromMSecsSinceEpoch(dateTimeInt);

    if(!dateTimeValue.isValid())
    {
        qWarning() << "The dateTime parameter value is not a valid QDateTime: " << value;
        fillErrorArray(paramName, "The time parameter value is not a valid QDateTime", errorArray);
        return false;
    }

    QString error;

    if(parameterFormat.contains(JsonParsersConstants::Hmi::MinAttribute))
    {
        Number min;
        if(!BankJsonParser::getObjSpecificAttributeValue(parameterFormat,
                                                         JsonParsersConstants::Hmi::MinAttribute,
                                                         min,
                                                         paramName,
                                                         &error))
        {
            qWarning() << error;
            fillErrorArray(paramName, error, errorArray);
            return false;
        }

        qint64 dateTimeMinInt = min.toInt64(&ok);

        if(!ok)
        {
            qWarning() << "The " << JsonParsersConstants::Hmi::MinAttribute << " number given is "
                       << "not suitable in order to be parsed to QDateTime: " << dateTimeMinInt;
            fillErrorArray(paramName,
                           QString("Wrong %1 datetime value")
                               .arg(JsonParsersConstants::Hmi::MinAttribute),
                           errorArray);
            return false;
        }

        QDateTime dateTimeMin = QDateTime::fromMSecsSinceEpoch(dateTimeMinInt);

        if(!dateTimeMin.isValid())
        {
            qWarning() << "The time " << JsonParsersConstants::Hmi::MinAttribute
                       << " attribute is not a valid QDateTime: " << dateTimeMin;
            fillErrorArray(paramName,
                           QString("The time %1 attribute is not a valid QDateTime")
                               .arg(JsonParsersConstants::Hmi::MinAttribute),
                           errorArray);
            return false;
        }

        if(dateTimeValue < dateTimeMin)
        {
            qWarning() << "The dateTime: " << value << " of the object: " << paramName
                       << ", is lesser than the " << JsonParsersConstants::Hmi::MinAttribute
                       << ": " << min;
            fillErrorArray(paramName,
                           QString("The number: %1, is lesser than the %2: %3")
                               .arg(value).arg(JsonParsersConstants::Hmi::MinAttribute).arg(min),
                           errorArray);
            return false;
        }
    }

    if(parameterFormat.contains(JsonParsersConstants::Hmi::MaxAttribute))
    {
        Number max;
        if(!BankJsonParser::getObjSpecificAttributeValue(parameterFormat,
                                                         JsonParsersConstants::Hmi::MaxAttribute,
                                                         max,
                                                         paramName,
                                                         &error))
        {
            qWarning() << error;
            fillErrorArray(paramName, error, errorArray);
            return false;
        }

        qint64 dateTimeMaxInt = max.toInt64(&ok);

        if(!ok)
        {
            qWarning() << "The " << JsonParsersConstants::Hmi::MaxAttribute << " number given is "
                       << "not suitable in order to be parsed to QDateTime: " << dateTimeMaxInt;
            fillErrorArray(paramName,
                           QString("Wrong %1 datetime value")
                               .arg(JsonParsersConstants::Hmi::MaxAttribute),
                           errorArray);
            return false;
        }

        QDateTime dateTimeMax = QDateTime::fromMSecsSinceEpoch(dateTimeMaxInt);

        if(!dateTimeMax.isValid())
        {
            qWarning() << "The time " << JsonParsersConstants::Hmi::MaxAttribute
                       << " attribute is not a valid QDateTime: " << dateTimeMax;
            fillErrorArray(paramName,
                           QString("The time %1 attribute is not a valid QDateTime")
                               .arg(JsonParsersConstants::Hmi::MaxAttribute),
                           errorArray);
            return false;
        }

        if(dateTimeValue < dateTimeMax)
        {
            qWarning() << "The dateTime: " << value << " of the object: " << paramName
                       << ", is greather than the " << JsonParsersConstants::Hmi::MaxAttribute
                       << ": " << max;
            fillErrorArray(paramName,
                           QString("The number: %1, is greather than the %2: %3")
                               .arg(value).arg(JsonParsersConstants::Hmi::MaxAttribute).arg(max),
                           errorArray);
            return false;
        }
    }

    return true;
}

bool BankJsonParamValidator::validateSelectParameterValue(const JsonObject &parameterFormat,
                                                          const QString &paramName,
                                                          const QVector<QString> &values,
                                                          JsonArray &errorArray)
{
    bool multiple = false;
    QString error;

    if(parameterFormat.contains(JsonParsersConstants::Hmi::MultipleAttribute) &&
       !BankJsonParser::getObjSpecificAttributeValue(parameterFormat,
                                                     JsonParsersConstants::Hmi::MultipleAttribute,
                                                     multiple,
                                                     paramName,
                                                     &error))
    {
        qWarning() << error;
        fillErrorArray(paramName, error, errorArray);
        return false;
    }

    if(!multiple && values.length() > 1)
    {
        qWarning() << "The select doesn't accept multiple values and multiple values have been "
                   << "given: " << values;
        fillErrorArray(paramName,
                       "Multiple values have been given, but it wasn't authorized",
                       errorArray);
        return false;
    }

    RETURN_IF_FALSE(validateListElemsParameterValue(parameterFormat,
                                                    paramName,
                                                    JsonParsersConstants::Hmi::OptionsAttribute,
                                                    values,
                                                    errorArray));

    return true;
}

bool BankJsonParamValidator::validateOkButtonParameterValue(const JsonObject &parameterFormat,
                                                            const QString &paramName,
                                                            const QString &value,
                                                            JsonArray &errorArray)
{
    QString key;
    QString error;

    if(!BankJsonParser::getObjSpecificAttributeValue(parameterFormat,
                                                     JsonParsersConstants::Hmi::KeyAttribute,
                                                     key,
                                                     paramName,
                                                     &error))
    {
        qWarning() << error;
        fillErrorArray(paramName, error, errorArray);
        return false;
    }

    if(key != value)
    {
        qWarning() << "The key given for the okButton object: " << paramName << " is incorrect: "
                   << value;
        fillErrorArray(paramName,
                       QString("The key given for the okButton is incorrect: %1").arg(value),
                       errorArray);
        return false;
    }

    return true;
}

bool BankJsonParamValidator::validateColorParameterValue(const JsonObject &parameterFormat,
                                                         const QString &paramName,
                                                         const QString &value,
                                                         JsonArray &errorArray)
{
    Q_UNUSED(parameterFormat)
    Q_UNUSED(paramName)
    Q_UNUSED(value)
    Q_UNUSED(errorArray)

    return true;
}

bool BankJsonParamValidator::validateListElemsParameterValue(const JsonObject &parameterFormat,
                                                             const QString &paramName,
                                                             const QString &attributeListName,
                                                             const QVector<QString> &values,
                                                             JsonArray &errorArray)
{
    QVector<QString> availableKeys;

    RETURN_IF_FALSE(getAvailableKeysFromListParam(parameterFormat,
                                                  attributeListName,
                                                  paramName,
                                                  errorArray,
                                                  availableKeys));

    for(const QString &value : values)
    {
        if(!availableKeys.contains(value))
        {
            qWarning() << "Unknown option key: " << value << ", for the list element";
            fillErrorArray(paramName, QString("Unknown option value: %1").arg(value), errorArray);
            return false;
        }
    }

    return true;
}

bool BankJsonParamValidator::validateListElemsParameterValue(const JsonObject &parameterFormat,
                                                             const QString &paramName,
                                                             const QString &attributeListName,
                                                             const QString &value,
                                                             JsonArray &errorArray)
{
    return validateListElemsParameterValue(parameterFormat,
                                           paramName,
                                           attributeListName,
                                           QVector<QString>({ value }),
                                           errorArray);
}

bool BankJsonParamValidator::getStringValue(const JsonObject &parameterValue,
                                            const QString &objectName,
                                            QString &value,
                                            JsonArray &errorArray,
                                            bool valueIsRequired)
{
    QString error;
    if(!BankJsonParser::getObjectValue(parameterValue, value, objectName, &error))
    {
        qWarning() << error;
        fillErrorArray(objectName, error, errorArray);
        return false;
    }

    if(value.isNull() && valueIsRequired)
    {
        qWarning() << "The value was required but it's null";
        fillErrorArray(objectName, "The value was required but it's null", errorArray);
        return false;
    }

    return true;
}

bool BankJsonParamValidator::getArrayStringValue(const JsonObject &parameterValue,
                                                 const QString &objectName,
                                                 QVector<QString> &values,
                                                 JsonArray &errorArray,
                                                 bool valueIsRequired)
{
    QString error;
    if(!BankJsonParser::getObjectValue(parameterValue, values, objectName, &error))
    {
        qWarning() << error;
        fillErrorArray(objectName, error, errorArray);
        return false;
    }

    if(values.isEmpty() && valueIsRequired)
    {
        qWarning() << "At least one value was required but the list is empty";
        fillErrorArray(objectName,
                       "The value was required but no value has been given",
                       errorArray);
        return false;
    }

    return true;
}

bool BankJsonParamValidator::getBoolValue(const JsonObject &parameterValue,
                                          const QString &objectName,
                                          bool &value,
                                          JsonArray &errorArray,
                                          bool valueIsRequired)
{
    QString error;
    if(!BankJsonParser::getObjectValue(parameterValue, value, objectName, &error, !valueIsRequired))
    {
        qWarning() << error;
        fillErrorArray(objectName, error, errorArray);
        return false;
    }

    return true;
}

bool BankJsonParamValidator::getArrayBoolValue(const JsonObject &parameterValue,
                                               const QString &objectName,
                                               QVector<bool> &values,
                                               JsonArray &errorArray,
                                               bool valueIsRequired)
{
    QString error;
    if(!BankJsonParser::getObjectValue(parameterValue, values, objectName, &error))
    {
        qWarning() << error;
        fillErrorArray(objectName, error, errorArray);
        return false;
    }

    if(values.isEmpty() && valueIsRequired)
    {
        qWarning() << "At least one value was required but the list is empty";
        fillErrorArray(objectName,
                       "The value was required but no value has been given",
                       errorArray);
        return false;
    }

    return true;
}

bool BankJsonParamValidator::getNumberValue(const JsonObject &parameterValue,
                                            const QString &objectName,
                                            Number &value,
                                            JsonArray &errorArray,
                                            bool valueIsRequired)
{
    QString error;
    if(!BankJsonParser::getObjectValue(parameterValue, value, objectName, &error))
    {
        qWarning() << error;
        fillErrorArray(objectName, error, errorArray);
        return false;
    }

    if(!value.isValid() && valueIsRequired)
    {
        qWarning() << "The value was required but it's invalid";
        fillErrorArray(objectName, "The value was required but it's invalid", errorArray);
        return false;
    }

    return true;
}

bool BankJsonParamValidator::getArrayNumberValue(const JsonObject &parameterValue,
                                                 const QString &objectName,
                                                 QVector<Number> &values,
                                                 JsonArray &errorArray,
                                                 bool valueIsRequired)
{
    QString error;
    if(!BankJsonParser::getObjectValue(parameterValue, values, objectName, &error))
    {
        qWarning() << error;
        fillErrorArray(objectName, error, errorArray);
        return false;
    }

    if(values.isEmpty() && valueIsRequired)
    {
        qWarning() << "The values were required but it's invalid";
        fillErrorArray(objectName, "The values were required but it's invalid", errorArray);
        return false;
    }

    return true;
}

bool BankJsonParamValidator::getAvailableKeysFromListParam(const JsonObject &parameterFormat,
                                                           const QString &attributeListName,
                                                           const QString &paramName,
                                                           JsonArray &errorArray,
                                                           QVector<QString> &availableKeys)
{
    QString error;

    JsonArray list;
    if(!BankJsonParser::getObjSpecificAttributeValue(parameterFormat,
                                                     attributeListName,
                                                     list,
                                                     paramName,
                                                     &error))
    {
        qWarning() << "The parameter object: " << paramName << ", has no attribute: "
                   << attributeListName;
        fillErrorArray(paramName, "Wrong parameter format", errorArray);
        return false;
    }

    JsonArray::const_iterator citer = list.cbegin();
    for(; citer != list.cend(); ++citer)
    {
        if(!citer->canConvert<JsonObject>())
        {
            qWarning() << "The parameter object: " << paramName << ", is wrongly formed for: "
                       << attributeListName;
            fillErrorArray(paramName, "The parameter format is wrongly formed", errorArray);
            return false;
        }

        JsonObject option = citer->value<JsonObject>();

        QString keyOption;
        if(!BankJsonParser::getObjSpecificAttributeValue(option,
                                                         JsonParsersConstants::Hmi::KeyAttribute,
                                                         keyOption,
                                                         paramName,
                                                         &error))
        {
            qWarning() << "The parameter object: " << paramName << ", has an: "
                       << attributeListName << " attribute, with an invalid element";
            fillErrorArray(paramName, "Wrong option element in parameter format", errorArray);
            return false;
        }

        availableKeys.append(keyOption);
    }

    return true;
}

bool BankJsonParamValidator::getIntValue(const QVariant &value,
                                         const QString &paramName,
                                         const QString &formatParamAttributeName,
                                         int &intValue,
                                         JsonArray &errorArray)
{
    bool ok = false;
    int tmpIntValue = value.toInt(&ok);

    if(!ok)
    {
        qWarning() << "The parameter format of the object: " << paramName << " has a wrong "
                   << "value type: " << value.toString() << ", for the attribute: "
                   << formatParamAttributeName;
        fillErrorArray(paramName,
                       QString("The attribute: %1, has wrong format: %2")
                           .arg(formatParamAttributeName, value.toString()),
                       errorArray);
        return false;
    }

    intValue = tmpIntValue;
    return true;
}
