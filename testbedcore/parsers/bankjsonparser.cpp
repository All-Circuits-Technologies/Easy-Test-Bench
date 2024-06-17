// SPDX-FileCopyrightText: 2019 - 2020 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "bankjsonparser.hpp"

#include "testbedcore/parsers/jsonparsersconstants.hpp"


bool BankJsonParser::getBoolValue(const JsonArray &objectsArray,
                                  const QString &wantedObjectName,
                                  bool &value,
                                  QString *errorLabel,
                                  bool canBeEmpty)
{
    return getKnownValue<bool>(objectsArray, wantedObjectName, value, errorLabel, canBeEmpty);
}

bool BankJsonParser::setBoolValue(const QString &wantedObjectName,
                                  bool value,
                                  JsonArray &objectsArray,
                                  QString *errorLabel)
{
    return setKnownValue<bool>(wantedObjectName, value, objectsArray, errorLabel);
}

bool BankJsonParser::getStringValue(const JsonArray &objectsArray,
                                    const QString &wantedObjectName,
                                    QString &value,
                                    QString *errorLabel,
                                    bool canBeEmpty)
{
    return getKnownValue<QString>(objectsArray, wantedObjectName, value, errorLabel, canBeEmpty);
}

bool BankJsonParser::setStringValue(const QString &wantedObjectName,
                                    const QString &value,
                                    JsonArray &objectsArray,
                                    QString *errorLabel)
{
    return setKnownValue<QString>(wantedObjectName, value, objectsArray, errorLabel);
}

bool BankJsonParser::getNumberValue(const JsonArray &objectsArray,
                                    const QString &wantedObjectName,
                                    Number &value,
                                    QString *errorLabel,
                                    bool canBeEmpty)
{
    return getKnownValue<Number>(objectsArray, wantedObjectName, value, errorLabel, canBeEmpty);
}

bool BankJsonParser::setNumberValue(const QString &wantedObjectName,
                                    const Number &value,
                                    JsonArray &objectsArray,
                                    QString *errorLabel)
{
    return setKnownValue<Number>(wantedObjectName, value, objectsArray, errorLabel);
}

bool BankJsonParser::getArrayBoolValue(const JsonArray &objectsArray,
                                       const QString &wantedObjectName,
                                       QVector<bool> &value,
                                       QString *errorLabel,
                                       bool canBeEmpty)
{
    return getKnownValue<QVector<bool>>(objectsArray,
                                        wantedObjectName,
                                        value,
                                        errorLabel,
                                        canBeEmpty);
}

bool BankJsonParser::setArrayBoolValue(const QString &wantedObjectName,
                                       const QVector<bool> &value,
                                       JsonArray &objectsArray,
                                       QString *errorLabel)
{
    return setKnownValue<QVector<bool>>(wantedObjectName, value, objectsArray, errorLabel);
}

bool BankJsonParser::getArrayStringValue(const JsonArray &objectsArray,
                                         const QString &wantedObjectName,
                                         QVector<QString> &value,
                                         QString *errorLabel,
                                         bool canBeEmpty)
{
    return getKnownValue<QVector<QString>>(objectsArray,
                                           wantedObjectName,
                                           value,
                                           errorLabel,
                                           canBeEmpty);
}

bool BankJsonParser::setArrayStringValue(const QString &wantedObjectName,
                                         const QVector<QString> &value,
                                         JsonArray &objectsArray,
                                         QString *errorLabel)
{
    return setKnownValue<QVector<QString>>(wantedObjectName, value, objectsArray, errorLabel);
}

bool BankJsonParser::getArrayNumberValue(const JsonArray &objectsArray,
                                         const QString &wantedObjectName,
                                         QVector<Number> &value,
                                         QString *errorLabel,
                                         bool canBeEmpty)
{
    return getKnownValue<QVector<Number>>(objectsArray,
                                          wantedObjectName,
                                          value,
                                          errorLabel,
                                          canBeEmpty);
}

bool BankJsonParser::setArrayNumberValue(const QString &wantedObjectName,
                                         const QVector<Number> &value,
                                         JsonArray &objectsArray,
                                         QString *errorLabel)
{
    return setKnownValue<QVector<Number>>(wantedObjectName, value, objectsArray, errorLabel);
}

bool BankJsonParser::setVariantValue(const QString &wantedObjectName,
                                     const QVariant &value,
                                     TypeManaged::Enum typeManaged,
                                     JsonArray &objectsArray,
                                     QString *errorLabel)
{
    if(typeManaged == TypeManaged::Number)
    {
        Number number;
        if(!CommonJsonParser::getValue(value, number))
        {
            return false;
        }

        return setKnownValue(wantedObjectName, number, objectsArray, errorLabel);
    }

    if(typeManaged == TypeManaged::String)
    {
        QString strValue;
        if(!CommonJsonParser::getValue(value, strValue))
        {
            return false;
        }

        return setKnownValue(wantedObjectName, strValue, objectsArray, errorLabel);
    }

    if(typeManaged == TypeManaged::Boolean)
    {
        bool boolValue;
        if(!CommonJsonParser::getValue(value, boolValue))
        {
            return false;
        }

        return setKnownValue(wantedObjectName, boolValue, objectsArray, errorLabel);
    }

    if(typeManaged == TypeManaged::NumberList)
    {
        QVector<Number> numberArrayValue;
        if(!CommonJsonParser::getValue(value, numberArrayValue))
        {
            return false;
        }

        return setKnownValue(wantedObjectName, numberArrayValue, objectsArray, errorLabel);
    }

    if(typeManaged == TypeManaged::StringList)
    {
        QVector<QString> stringArrayValue;
        if(!CommonJsonParser::getValue(value, stringArrayValue))
        {
            return false;
        }

        return setKnownValue(wantedObjectName, stringArrayValue, objectsArray, errorLabel);
    }

    if(typeManaged == TypeManaged::BooleanList)
    {
        QVector<bool> boolArrayValue;
        if(!CommonJsonParser::getValue(value, boolArrayValue))
        {
            return false;
        }

        return setKnownValue(wantedObjectName, boolArrayValue, objectsArray, errorLabel);
    }

    qWarning() << "Can't set an unknown variant value";
    return false;
}

bool BankJsonParser::getManagedTypeValueFromParamFormat(const JsonObject &paramObject,
                                                        TypeManaged::Enum &typeManaged)
{
    typeManaged = TypeManaged::Unknown;

    QString type;

    if(!paramObject.contains(JsonParsersConstants::Generic::TypeName))
    {
        qWarning() << "There is no attribute: " << JsonParsersConstants::Generic::TypeName << ", "
                   << "in the object given";
        return false;
    }

    type = paramObject[JsonParsersConstants::Generic::TypeName].toString();

    if(type == "string" || type == "radioList")
    {
        typeManaged = TypeManaged::String;
    }
    else if(type == "bool")
    {
        typeManaged = TypeManaged::Boolean;
    }
    else if(type == "number" || type == "date" || type == "time" || type == "color")
    {
        typeManaged = TypeManaged::Number;
    }
    else if(type == "select" || type == "checkBoxList")
    {
        typeManaged = TypeManaged::StringList;
    }
    else
    {
        qWarning() << "The parameter format type isn't known: " << type;
        return false;
    }

    return true;
}

bool BankJsonParser::findObject(const JsonArray &objectsArray,
                                const QString &wantedObjectName,
                                JsonObject &objectFound,
                                bool canBeNotFound)
{
    JsonArray::const_iterator citer = objectsArray.cbegin();
    for(; citer != objectsArray.cend(); ++citer)
    {
        if(!citer->canConvert<JsonObject>())
        {
            qWarning() << "Can't convert the object: " << *citer << ", to JsonObject";
            return false;
        }

        JsonObject object = citer->value<JsonObject>();
        
        QString valueName = object.value(JsonParsersConstants::Generic::NameAttribute).toString();

        if(valueName == wantedObjectName)
        {
            objectFound = object;
            return true;
        }
    }

    // If we are here, it means that no JsonObject has been found with the wanted object name
    return canBeNotFound;
}
