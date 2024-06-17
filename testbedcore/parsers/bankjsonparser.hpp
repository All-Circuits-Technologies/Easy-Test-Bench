// SPDX-FileCopyrightText: 2019 - 2020 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "jsonutility/jsontypedefs.hpp"

#include "testbedcore/parsers/commonjsonparser.hpp"
#include "testbedcore/parsers/jsonparsersconstants.hpp"


/** @brief This class contains helping functions to parse objects returned by the banks */
class BankJsonParser
{
    public:
        /** @brief Class constructor */
        explicit BankJsonParser() = delete;

    public:
        /** @brief Helps to get the boolean value contained in a object pointed by the name given
            @note The json array has to contain objects formed like this:
                  {
                      "name": "[wantedObjectName]",
                      "value": "[valueWanted]"
                  }
            @see BankJsonParser::getKnownValue
            @param objectsArray The array which contains the object with the value wanted
            @param wantedObjectName The name of the object searched
            @param value The value got
            @param errorLabel If a string is given and if an error occurred, the QString will be
                              filled with the error detail
            @param canBeEmpty If false and if the object pointed by the name is not found, the
                              function returns in error.
                              If true and if the object pointed by the name is not found, the
                              function returns ok and the boolean value will be equal to the value
                              given
            @return True if no error occurs */
        static bool getBoolValue(const JsonArray &objectsArray,
                                 const QString &wantedObjectName,
                                 bool &value,
                                 QString *errorLabel = nullptr,
                                 bool canBeEmpty = false);

        /** @brief Helps to set a boolean value in the object pointed by the name given
            @note The json array has to contain objects formed like this:
                  {
                      "name": "[wantedObjectName]",
                      "value": "[valueWanted]"
                  }
            @note If there is no object named with the name given, a new object is created
            @see BankJsonParser::setKnownValue
            @param wantedObjectName The name of the object to set the value in
            @param value The value to set
            @param objectsArray The json Array where to modifiy (or create) the object wanted with
                                the value given
            @param errorLabel If a string is given and if an error occurred, the QString will be
                              filled with the error detail
            @return True if no error occurs */
        static bool setBoolValue(const QString &wantedObjectName,
                                 bool value,
                                 JsonArray &objectsArray,
                                 QString *errorLabel = nullptr);

        /** @brief Helps to get the string value contained in a object pointed by the name given
            @note The json array has to contain objects formed like this:
                  {
                      "name": "[wantedObjectName]",
                      "value": "[valueWanted]"
                  }
            @see BankJsonParser::getKnownValue
            @param objectsArray The array which contains the object with the value wanted
            @param wantedObjectName The name of the object searched
            @param value The value got
            @param errorLabel If a string is given and if an error occurred, the QString will be
                              filled with the error detail
            @param canBeEmpty If false and if the object pointed by the name is not found, the
                              function returns in error.
                              If true and if the object pointed by the name is not found, the
                              function returns ok and the string value will be equal to the value
                              given
            @return True if no error occurs */
        static bool getStringValue(const JsonArray &objectsArray,
                                   const QString &wantedObjectName,
                                   QString &value,
                                   QString *errorLabel = nullptr,
                                   bool canBeEmpty = false);

        /** @brief Helps to set a string value in the object pointed by the name given
            @note The json array has to contain objects formed like this:
                  {
                      "name": "[wantedObjectName]",
                      "value": "[valueWanted]"
                  }
            @note If there is no object named with the name given, a new object is created
            @see BankJsonParser::setKnownValue
            @param wantedObjectName The name of the object to set the value in
            @param value The value to set
            @param objectsArray The json Array where to modifiy (or create) the object wanted with
                                the value given
            @param errorLabel If a string is given and if an error occurred, the QString will be
                              filled with the error detail
            @return True if no error occurs */
        static bool setStringValue(const QString &wantedObjectName,
                                   const QString &value,
                                   JsonArray &objectsArray,
                                   QString *errorLabel = nullptr);

        /** @brief Helps to get the number value contained in a object pointed by the name given
            @note The json array has to contain objects formed like this:
                  {
                      "name": "[wantedObjectName]",
                      "value": "[valueWanted]"
                  }
            @see BankJsonParser::getKnownValue
            @param objectsArray The array which contains the object with the value wanted
            @param wantedObjectName The name of the object searched
            @param value The value got
            @param errorLabel If a string is given and if an error occurred, the QString will be
                              filled with the error detail
            @param canBeEmpty If false and if the object pointed by the name is not found, the
                              function returns in error.
                              If true and if the object pointed by the name is not found, the
                              function returns ok and the number value will be equal to the value
                              given
            @return True if no error occurs */
        static bool getNumberValue(const JsonArray &objectsArray,
                                   const QString &wantedObjectName,
                                   Number &value,
                                   QString *errorLabel = nullptr,
                                   bool canBeEmpty = false);

        /** @brief Helps to set a number value in the object pointed by the name given
            @note The json array has to contain objects formed like this:
                  {
                      "name": "[wantedObjectName]",
                      "value": "[valueWanted]"
                  }
            @note If there is no object named with the name given, a new object is created
            @see BankJsonParser::setKnownValue
            @param wantedObjectName The name of the object to set the value in
            @param value The value to set
            @param objectsArray The json Array where to modifiy (or create) the object wanted with
                                the value given
            @param errorLabel If a string is given and if an error occurred, the QString will be
                              filled with the error detail
            @return True if no error occurs */
        static bool setNumberValue(const QString &wantedObjectName,
                                   const Number &value,
                                   JsonArray &objectsArray,
                                   QString *errorLabel = nullptr);

        /** @brief Helps to get the array bool value contained in a object pointed by the name given
            @note The json array has to contain objects formed like this:
                  {
                      "name": "[wantedObjectName]",
                      "value": "[valueWanted]"
                  }
            @see BankJsonParser::getKnownValue
            @param objectsArray The array which contains the object with the value wanted
            @param wantedObjectName The name of the object searched
            @param value The value got
            @param errorLabel If a string is given and if an error occurred, the QString will be
                              filled with the error detail
            @param canBeEmpty If false and if the object pointed by the name is not found, the
                              function returns in error.
                              If true and if the object pointed by the name is not found, the
                              function returns ok and the array boolean value will be equal to the
                              value given
            @return True if no error occurs */
        static bool getArrayBoolValue(const JsonArray &objectsArray,
                                      const QString &wantedObjectName,
                                      QVector<bool> &value,
                                      QString *errorLabel = nullptr,
                                      bool canBeEmpty = false);

        /** @brief Helps to set an array bool value in the object pointed by the name given
            @note The json array has to contain objects formed like this:
                  {
                      "name": "[wantedObjectName]",
                      "value": "[valueWanted]"
                  }
            @note If there is no object named with the name given, a new object is created
            @see BankJsonParser::setKnownValue
            @param wantedObjectName The name of the object to set the value in
            @param value The value to set
            @param objectsArray The json Array where to modifiy (or create) the object wanted with
                                the value given
            @param errorLabel If a string is given and if an error occurred, the QString will be
                              filled with the error detail
            @return True if no error occurs */
        static bool setArrayBoolValue(const QString &wantedObjectName,
                                      const QVector<bool> &value,
                                      JsonArray &objectsArray,
                                      QString *errorLabel = nullptr);

        /** @brief Helps to get the array string value contained in a object pointed by the name
                   given
            @note The json array has to contain objects formed like this:
                  {
                      "name": "[wantedObjectName]",
                      "value": "[valueWanted]"
                  }
            @see BankJsonParser::getKnownValue
            @param objectsArray The array which contains the object with the value wanted
            @param wantedObjectName The name of the object searched
            @param value The value got
            @param errorLabel If a string is given and if an error occurred, the QString will be
                              filled with the error detail
            @param canBeEmpty If false and if the object pointed by the name is not found, the
                              function returns in error.
                              If true and if the object pointed by the name is not found, the
                              function returns ok and the array string value will be equal to the
                              value given
            @return True if no error occurs */
        static bool getArrayStringValue(const JsonArray &objectsArray,
                                        const QString &wantedObjectName,
                                        QVector<QString> &value,
                                        QString *errorLabel = nullptr,
                                        bool canBeEmpty = false);

        /** @brief Helps to set an array string value in the object pointed by the name given
            @note The json array has to contain objects formed like this:
                  {
                      "name": "[wantedObjectName]",
                      "value": "[valueWanted]"
                  }
            @note If there is no object named with the name given, a new object is created
            @see BankJsonParser::setKnownValue
            @param wantedObjectName The name of the object to set the value in
            @param value The value to set
            @param objectsArray The json Array where to modifiy (or create) the object wanted with
                                the value given
            @param errorLabel If a string is given and if an error occurred, the QString will be
                              filled with the error detail
            @return True if no error occurs */
        static bool setArrayStringValue(const QString &wantedObjectName,
                                        const QVector<QString> &value,
                                        JsonArray &objectsArray,
                                        QString *errorLabel = nullptr);

        /** @brief Helps to get the array number value contained in a object pointed by the name
                   given
            @note The json array has to contain objects formed like this:
                  {
                      "name": "[wantedObjectName]",
                      "value": "[valueWanted]"
                  }
            @see BankJsonParser::getKnownValue
            @param objectsArray The array which contains the object with the value wanted
            @param wantedObjectName The name of the object searched
            @param value The value got
            @param errorLabel If a string is given and if an error occurred, the QString will be
                              filled with the error detail
            @param canBeEmpty If false and if the object pointed by the name is not found, the
                              function returns in error.
                              If true and if the object pointed by the name is not found, the
                              function returns ok and the array boolean value will be equal to the
                              value given
            @return True if no error occurs */
        static bool getArrayNumberValue(const JsonArray &objectsArray,
                                        const QString &wantedObjectName,
                                        QVector<Number> &value,
                                        QString *errorLabel = nullptr,
                                        bool canBeEmpty = false);

        /** @brief Helps to set an array number value in the object pointed by the name given
            @note The json array has to contain objects formed like this:
                  {
                      "name": "[wantedObjectName]",
                      "value": "[valueWanted]"
                  }
            @note If there is no object named with the name given, a new object is created
            @see BankJsonParser::setKnownValue
            @param wantedObjectName The name of the object to set the value in
            @param value The value to set
            @param objectsArray The json Array where to modifiy (or create) the object wanted with
                                the value given
            @param errorLabel If a string is given and if an error occurred, the QString will be
                              filled with the error detail
            @return True if no error occurs */
        static bool setArrayNumberValue(const QString &wantedObjectName,
                                        const QVector<Number> &value,
                                        JsonArray &objectsArray,
                                        QString *errorLabel = nullptr);

        /** @brief Helps to set a value in the object pointed by the name given
            @note The json array has to contain objects formed like this:
                  {
                      "name": "[wantedObjectName]",
                      "value(s)": "[valueWanted]"
                  }
                  A 's' has to be set to "value" if the type of the value is a list (any list)
            @note If there is no object named with the name given, a new object is created
            @see BankJsonParser::setKnownValue
            @param wantedObjectName The name of the object to set the value in
            @param value The value to set
            @param typeManaged The type of the variant value
            @param objectsArray The json Array where to modifiy (or create) the object wanted with
                                the value given
            @param errorLabel If a string is given and if an error occurred, the QString will be
                              filled with the error detail
            @return True if no error occurs */
        static bool setVariantValue(const QString &wantedObjectName,
                                    const QVariant &value,
                                    TypeManaged::Enum typeManaged,
                                    JsonArray &objectsArray,
                                    QString *errorLabel = nullptr);

        /** @brief Get the value of attribute "type" contains in the object given, and then cast the
                   value from the string HMI label to the type managed
            @see ISequenceInstanceModule::getModuleParametersFormat
            @param paramObject The object to parse
            @param typeManaged The type managed got from the parameter format
            @return True if no error occurs */
        static bool getManagedTypeValueFromParamFormat(const JsonObject &paramObject,
                                                       TypeManaged::Enum &typeManaged);

        /** @brief Find an object pointed by tge name given
            @note The json array has to contain objects formed like this:
                  {
                      "name": "[wantedObjectName]",
                      "value(s)": "[valueWanted]"
                  }
                  A 's' has to be set to "value" if the type of the value is a list (any list)
            @param objectsArray The array which contains the object wanted
            @param wantedObjectName The name of the object wanted
            @param objectFound The object found
            @param canBeNotFound If true, the method will return true even if the object is not
                                 found, the objectFound will be unchanged
            @return False if a problem occurs or if the object is not found (except if the
                    canBeNotFound param is set to true) */
        static bool findObject(const JsonArray &objectsArray,
                               const QString &wantedObjectName,
                               JsonObject &objectFound,
                               bool canBeNotFound = false);

    public:
        /** @brief Helps to get the value (with its type managed) contained in a object pointed by
                   the name given
            @note The json array has to contain objects formed like this:
                  {
                      "name": "[wantedObjectName]",
                      "value": "[valueWanted]"
                  }
            @param objectsArray The array which contains the object with the value wanted
            @param wantedObjectName The name of the object searched
            @param value The value got
            @param errorLabel If a string is given and if an error occurred, the QString will be
                              filled with the error detail
            @param canBeEmpty If false and if the object pointed by the name is not found, the
                              function returns in error.
                              If true and if the object pointed by the name is not found, the
                              function returns ok and the value returned will be equal to the
                              value given
            @return True if no error occurs */
        template<class T>
        static bool getKnownValue(const JsonArray &objectsArray,
                                  const QString &wantedObjectName,
                                  T &value,
                                  QString *errorLabel = nullptr,
                                  bool canBeEmpty = false);

        /** @brief Helps to set a value (with a managed type) in the object pointed by the name
                   given
            @note The json array has to contain objects formed like this:
                  {
                      "name": "[wantedObjectName]",
                      "value": "[valueWanted]"
                  }
            @note If there is no object named with the name given, a new object is created
            @param wantedObjectName The name of the object to set the value in
            @param value The value to set
            @param objectsArray The json Array where to modifiy (or create) the object wanted with
                                the value given
            @param errorLabel If a string is given and if an error occurred, the QString will be
                              filled with the error detail
            @return True if no error occurs */
        template<class T>
        static bool setKnownValue(const QString &wantedObjectName,
                                  const T &value,
                                  JsonArray &objectsArray,
                                  QString *errorLabel = nullptr);

        /** @brief Helps to get the value of an attribute contained in a object pointed by
                   the name given
            @note The json array has to contain objects formed like this:
                  {
                      "name": "[wantedObjectName]",
                      "[attributeName]": "[value]"
                  }
            @param objectsArray The array which contains the object with the value wanted
            @param wantedObjectName The name of the object searched
            @param attributeName The attribute to get the value from
            @param value The value got
            @param errorLabel If a string is given and if an error occurred, the QString will be
                              filled with the error detail
            @param canBeEmpty If false and if the object pointed by the name is not found, the
                              function returns in error.
                              If true and if the object pointed by the name is not found, the
                              function returns ok and the value returned will be equal to the
                              value given
            @return True if no error occurs */
        template<class T>
        static bool getSpecificAttributeValue(const JsonArray &objectsArray,
                                              const QString &wantedObjectName,
                                              const QString &attributeName,
                                              T &value,
                                              QString *errorLabel = nullptr,
                                              bool canBeEmpty = false);

        /** @brief Helps to get the value of an attribute contained in the object given
            @note The object has to be formed like this:
                  {
                      "[attributeName]": "[value]"
                  }
            @param object The object to get the value of one of its attribute
            @param attributeName The attribute to get the value from
            @param value The value got
            @param objectName The object name (used to display more precise errors)
            @param errorLabel If a string is given and if an error occurred, the QString will be
                              filled with the error detail
            @param canBeEmpty If false and if the attribute name is not found, the function returns
                              in error.
                              If true and if the attribute name is not found, the function returns
                              ok and the value returned will be equal to the value given
            @return True if no error occurs */
        template<class T>
        static bool getObjSpecificAttributeValue(const JsonObject &object,
                                                 const QString &attributeName,
                                                 T &value,
                                                 const QString &objectName = QString(),
                                                 QString *errorLabel = nullptr,
                                                 bool canBeEmpty = false);

        /** @brief Helps to get the value of an attribute contained in the object given
            @note The object has to be formed like this:
                  {
                      "value(s)": "[valueWanted]"
                  }
                  A 's' has to be set to "value" if the type of the value is a list (any list)
            @param object The object to get the value of one of its attribute
            @param value The value got
            @param objectName The object name (used to display more precise errors)
            @param errorLabel If a string is given and if an error occurred, the QString will be
                              filled with the error detail
            @param canBeEmpty If false and if the object pointed by the name is not found, the
                              function returns in error.
                              If true and if the object pointed by the name is not found, the
                              function returns ok and the value returned will be equal to the
                              value given
            @return True if no error occurs */
        template<class T>
        static bool getObjectValue(const JsonObject &object,
                                   T &value,
                                   const QString &objectName = QString(),
                                   QString *errorLabel = nullptr,
                                   bool canBeEmpty = false);

        /** @brief Helps to set the value of an attribute contained in the object pointed by the
                   name given
            @note The json array has to contain objects formed like this:
                  {
                      "name": "[wantedObjectName]",
                      "value": "[valueWanted]"
                  }
            @note If there is no object named with the name given, a new object is created
            @param wantedObjectName The name of the object to set the value in
            @param attributeName The attribute to set the value into
            @param value The value to set
            @param objectsArray The json Array where to modifiy (or create) the object wanted with
                                the value given
            @param errorLabel If a string is given and if an error occurred, the QString will be
                              filled with the error detail
            @return True if no error occurs */
        template<class T>
        static bool setSpecificAttributeValue(const QString &wantedObjectName,
                                              const QString &attributeName,
                                              const T &value,
                                              JsonArray &objectsArray,
                                              QString *errorLabel = nullptr);
};

template<class T>
bool BankJsonParser::getKnownValue(const JsonArray &objectsArray,
                                   const QString &wantedObjectName,
                                   T &value,
                                   QString *errorLabel,
                                   bool canBeEmpty)
{
    static_assert(std::is_same<Number, T>::value            ||
                  std::is_same<bool, T>::value              ||
                  std::is_same<QString, T>::value           ||
                  std::is_same<QVector<Number>, T>::value   ||
                  std::is_same<QVector<bool>, T>::value     ||
                  std::is_same<QVector<QString>, T>::value,
                  "T must be a type managed");

    QString valueAttr = CommonJsonParser::getValueAttributeName<T>();

    return getSpecificAttributeValue(objectsArray,
                                     wantedObjectName,
                                     valueAttr,
                                     value,
                                     errorLabel,
                                     canBeEmpty);
}

template<class T>
bool BankJsonParser::setKnownValue(const QString &wantedObjectName,
                                   const T &value,
                                   JsonArray &objectsArray,
                                   QString *errorLabel)
{
    static_assert(std::is_same<Number, T>::value            ||
                  std::is_same<bool, T>::value              ||
                  std::is_same<QString, T>::value           ||
                  std::is_same<QVector<Number>, T>::value   ||
                  std::is_same<QVector<bool>, T>::value     ||
                  std::is_same<QVector<QString>, T>::value,
                  "T must be a type managed");

    QString valueAttr = CommonJsonParser::getValueAttributeName<T>();

    return setSpecificAttributeValue(wantedObjectName, valueAttr, value, objectsArray, errorLabel);
}

template<class T>
bool BankJsonParser::getSpecificAttributeValue(const JsonArray &objectsArray,
                                               const QString &wantedObjectName,
                                               const QString &attributeName,
                                               T &value,
                                               QString *errorLabel,
                                               bool canBeEmpty)
{
    static_assert(std::is_same<Number, T>::value            ||
                  std::is_same<bool, T>::value              ||
                  std::is_same<QString, T>::value           ||
                  std::is_same<QVector<Number>, T>::value   ||
                  std::is_same<QVector<bool>, T>::value     ||
                  std::is_same<QVector<QString>, T>::value,
                  "T must be a type managed");

    if(objectsArray.isEmpty() && !canBeEmpty)
    {
        MANAGE_PTR_LOG_ERR(errorLabel,
                           QObject::tr("(TR) The attribute: %1, of the object: %2, hasn't been "
                                       "found").arg(attributeName, wantedObjectName));
        return false;
    }

    JsonArray::const_iterator iter = objectsArray.cbegin();

    for(;iter != objectsArray.cend(); ++iter)
    {
        if(!iter->canConvert<JsonObject>())
        {
            MANAGE_PTR_LOG_ERR(errorLabel,
                               QObject::tr("(TR) Can't convert the JsonArray element: %1, to "
                                           "JsonObject").arg(iter->toString()));
            return false;
        }

        JsonObject object = iter->value<JsonObject>();

        QString valueName = object.value(JsonParsersConstants::Generic::NameAttribute).toString();

        if(valueName != wantedObjectName)
        {
            continue;
        }

        // The canBeEmpty parameter here is relative to the object, so we expect to find the
        // attribute named "value" or "values", if the object has been found
        RETURN_IF_FALSE(getObjSpecificAttributeValue(object,
                                                     attributeName,
                                                     value,
                                                     wantedObjectName,
                                                     errorLabel,
                                                     false));

        return true;
    }

    if(canBeEmpty)
    {
        // The object hasn't been found but we say that the value can be empty
        return true;
    }

    MANAGE_PTR_LOG_ERR(errorLabel,
                       QObject::tr("(TR) The wanted object: %1, hasn't been found")
                                                                            .arg(wantedObjectName));
    return false;
}

template<class T>
bool BankJsonParser::getObjSpecificAttributeValue(const JsonObject &object,
                                                  const QString &attributeName,
                                                  T &value,
                                                  const QString &objectName,
                                                  QString *errorLabel,
                                                  bool canBeEmpty)
{
    static_assert(std::is_same<Number, T>::value            ||
                  std::is_same<bool, T>::value              ||
                  std::is_same<QString, T>::value           ||
                  std::is_same<QVector<Number>, T>::value   ||
                  std::is_same<QVector<bool>, T>::value     ||
                  std::is_same<QVector<QString>, T>::value,
                  "T must be a type managed");

    if(!object.contains(attributeName))
    {
        if(canBeEmpty)
        {
            // The attribute hasn't been found but we say that the value can be empty
            return true;
        }

        MANAGE_PTR_LOG_ERR(errorLabel,
                           QObject::tr("(TR) The attribute: %1, of the object: %2, doesn't "
                                       "exist").arg(attributeName, objectName));
        return false;
    }

    if(!CommonJsonParser::getValue<T>(object.value(attributeName), value))
    {
        MANAGE_PTR_LOG_ERR(errorLabel,
                           QObject::tr("(TR) The attribute: %1, of the object: %2, isn't at the "
                                       "rigth type").arg(attributeName, objectName));
        return false;
    }

    return true;
}

template<>
inline bool BankJsonParser::getObjSpecificAttributeValue<JsonObject>(const JsonObject &object,
                                                                     const QString &attributeName,
                                                                     JsonObject &value,
                                                                     const QString &objectName,
                                                                     QString *errorLabel,
                                                                     bool canBeEmpty)
{
    if(!object.contains(attributeName))
    {
        if(canBeEmpty)
        {
            // The attribute hasn't been found but we say that the value can be empty
            return true;
        }

        MANAGE_PTR_LOG_ERR(errorLabel,
                           QObject::tr("(TR) The attribute: %1, of the object: %2, doesn't "
                                       "exist").arg(attributeName, objectName));
        return false;
    }

    const QVariant &varValue = object[attributeName];

    if(!varValue.canConvert<JsonObject>())
    {
        MANAGE_PTR_LOG_ERR(errorLabel,
                           QObject::tr("(TR) The attribute: %1, of the object: %2, isn't a "
                                       "JsonObject")
                                .arg(attributeName, objectName));
        return false;
    }

    value = varValue.value<JsonObject>();

    return true;
}

template<>
inline bool BankJsonParser::getObjSpecificAttributeValue<JsonArray>(const JsonObject &object,
                                                                    const QString &attributeName,
                                                                    JsonArray &value,
                                                                    const QString &objectName,
                                                                    QString *errorLabel,
                                                                    bool canBeEmpty)
{
    if(!object.contains(attributeName))
    {
        if(canBeEmpty)
        {
            // The attribute hasn't been found but we say that the value can be empty
            return true;
        }

        MANAGE_PTR_LOG_ERR(errorLabel,
                           QObject::tr("(TR) The attribute: %1, of the object: %2, doesn't "
                                       "exist").arg(attributeName, objectName));
        return false;
    }

    const QVariant &varValue = object[attributeName];

    if(!varValue.canConvert<JsonArray>())
    {
        MANAGE_PTR_LOG_ERR(errorLabel,
                           QObject::tr("(TR) The attribute: %1, of the object: %2, isn't a "
                                       "JsonArray")
                                .arg(attributeName, objectName));
        return false;
    }

    value = varValue.value<JsonArray>();

    return true;
}

template<class T>
bool BankJsonParser::getObjectValue(const JsonObject &object,
                                    T &value,
                                    const QString &objectName,
                                    QString *errorLabel,
                                    bool canBeEmpty)
{
    return getObjSpecificAttributeValue(object,
                                        CommonJsonParser::getValueAttributeName<T>(),
                                        value,
                                        objectName,
                                        errorLabel,
                                        canBeEmpty);
}

template<class T>
bool BankJsonParser::setSpecificAttributeValue(const QString &wantedObjectName,
                                               const QString &attributeName,
                                               const T &value,
                                               JsonArray &objectsArray,
                                               QString *errorLabel)
{
    static_assert(std::is_same<Number, T>::value            ||
                  std::is_same<bool, T>::value              ||
                  std::is_same<QString, T>::value           ||
                  std::is_same<QVector<Number>, T>::value   ||
                  std::is_same<QVector<bool>, T>::value     ||
                  std::is_same<QVector<QString>, T>::value,
                  "T must be a type managed");

    // If the element already exists, remove it
    JsonArray::iterator iter = objectsArray.begin();
    while(iter != objectsArray.end())
    {
        if(!iter->canConvert<JsonObject>())
        {
            MANAGE_PTR_LOG_ERR(errorLabel,
                               QObject::tr("(TR) Can't convert the JsonArray element: %1, to "
                                           "JsonObject").arg(iter->toString()));
            return false;
        }

        JsonObject object = iter->value<JsonObject>();

        QString valueName = object.value(JsonParsersConstants::Generic::NameAttribute).toString();

        if(valueName == wantedObjectName)
        {
            objectsArray.erase(iter);
            break;
        }

        ++iter;
    }

    QVariant varValue;

    if(!CommonJsonParser::setValue<T>(value, varValue))
    {
        MANAGE_PTR_LOG_ERR(errorLabel,
                           QObject::tr("(TR) Can't parse the attribute: %1, of the object: %2")
                                                                            .arg(attributeName,
                                                                                 wantedObjectName));
        return false;
    }

    objectsArray.append(QVariant::fromValue(JsonObject({
        { JsonParsersConstants::Generic::NameAttribute, wantedObjectName },
        { attributeName, varValue }
    })));

    return true;
}
