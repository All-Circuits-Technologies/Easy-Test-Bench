// SPDX-FileCopyrightText: 2019 - 2020 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "jsonutility/jsontypedefs.hpp"

class Number;


/** @brief The class contains usefull methods to verify and validate parameters of a sequence
           module */
class BankJsonParamValidator
{
    private:
        /** @brief Private class constructor */
        BankJsonParamValidator() {}

    public:
        /** @brief Verify and validate the sequence module parameters
            @param parametersFormat The parameters format to validate the parameters with
            @param parametersValues The parameters values to verify and validate
            @param errorArray The JsonArray to fill with the error
            @return True if the parameters values are valids */
        static bool validateModuleParameters(const JsonArray &parametersFormat,
                                             const JsonArray &parametersValues,
                                             JsonArray &errorArray);

        /** @brief Help to fill the parameters JsonArray error
            @param parameterName The parameter which has caused an error
            @param errorStr The error caused by the parameter
            @param errorArray The JsonArray to fill with the error */
        static void fillErrorArray(const QString &parameterName,
                                   const QString &errorStr,
                                   JsonArray &errorArray);

        /** @brief Help to fill the parameters JsonArray error for generic error
            @param errorStr The error caused by the parameter
            @param errorArray The JsonArray to fill with the error */
        static void fillErrorArray(const QString &errorStr,
                                   JsonArray &errorArray);

    private:
        /** @brief Verify and validate if the parameter format given has a value correctly formed
                   in the list attached
            @param parameterFormat The format of one expected parameter
            @param parametersValues The parameters values to verify and validate
            @param errorArray The JsonArray to fill with the error
            @return True if the parameter format given has found a value which matches the format */
        static bool validateParameterValue(const JsonObject &parameterFormat,
                                           const JsonArray &parametersValues,
                                           JsonArray &errorArray);

        /** @brief Verify and validate if the value given is correct
            @param parameterFormat The format expected of the parameter value
            @param paramName The name of the parameter
            @param value The string value to validate
            @param errorArray The JsonArray to fill with the error
            @return True if the parameter value is valid */
        static bool validateStringParameterValue(const JsonObject &parameterFormat,
                                                 const QString &paramName,
                                                 const QString &value,
                                                 JsonArray &errorArray);

        /** @brief Verify and validate if the value given is correct
            @param parameterFormat The format expected of the parameter value
            @param paramName The name of the parameter
            @param values The string list value to validate
            @param errorArray The JsonArray to fill with the error
            @return True if the parameter value is valid */
        static bool validateStringListParameterValue(const JsonObject &parameterFormat,
                                                     const QString &paramName,
                                                     const QVector<QString> &values,
                                                     JsonArray &errorArray);

        /** @brief Verify and validate if the value given is correct
            @param parameterFormat The format expected of the parameter value
            @param paramName The name of the parameter
            @param value The number value to validate
            @param errorArray The JsonArray to fill with the error
            @return True if the parameter value is valid */
        static bool validateNumberParameterValue(const JsonObject &parameterFormat,
                                                 const QString &paramName,
                                                 const Number &value,
                                                 JsonArray &errorArray);

        /** @brief Verify and validate if the value given is correct
            @param parameterFormat The format expected of the parameter value
            @param paramName The name of the parameter
            @param values The number list value to validate
            @param errorArray The JsonArray to fill with the error
            @return True if the parameter value is valid */
        static bool validateNumberListParameterValue(const JsonObject &parameterFormat,
                                                     const QString &paramName,
                                                     const QVector<Number> &values,
                                                     JsonArray &errorArray);

        /** @brief Verify and validate if the value given is correct
            @note The data value is formed with the ISO Date pattern: yyyy-MM-dd
            @param parameterFormat The format expected of the parameter value
            @param paramName The name of the parameter
            @param value The date value to validate
            @param errorArray The JsonArray to fill with the error
            @return True if the parameter value is valid */
        static bool validateDateParameterValue(const JsonObject &parameterFormat,
                                               const QString &paramName,
                                               const QString &value,
                                               JsonArray &errorArray);

        /** @brief Verify and validate if the value given is correct
            @note The time value is the milliseconds since start of the day
            @param parameterFormat The format expected of the parameter value
            @param paramName The name of the parameter
            @param value The time value to validate
            @param errorArray The JsonArray to fill with the error
            @return True if the parameter value is valid */
        static bool validateTimeParameterValue(const JsonObject &parameterFormat,
                                               const QString &paramName,
                                               const Number &value,
                                               JsonArray &errorArray);

        /** @brief Verify and validate if the value given is correct
            @note The time value is the milliseconds since epoch
            @param parameterFormat The format expected of the parameter value
            @param paramName The name of the parameter
            @param value The datetime value to validate
            @param errorArray The JsonArray to fill with the error
            @return True if the parameter value is valid */
        static bool validateDateTimeParameterValue(const JsonObject &parameterFormat,
                                                   const QString &paramName,
                                                   const Number &value,
                                                   JsonArray &errorArray);

        /** @brief Verify and validate if the value given is correct
            @param parameterFormat The format expected of the parameter value
            @param paramName The name of the parameter
            @param values The select values to validate
            @param errorArray The JsonArray to fill with the error
            @return True if the parameter values are valids */
        static bool validateSelectParameterValue(const JsonObject &parameterFormat,
                                                 const QString &paramName,
                                                 const QVector<QString> &values,
                                                 JsonArray &errorArray);

        /** @brief Verify and validate if the value given is correct
            @param parameterFormat The format expected of the parameter value
            @param paramName The name of the parameter
            @param value The ok button value to validate
            @param errorArray The JsonArray to fill with the error
            @return True if the parameter value is valid */
        static bool validateOkButtonParameterValue(const JsonObject &parameterFormat,
                                                   const QString &paramName,
                                                   const QString &value,
                                                   JsonArray &errorArray);

        /** @brief Verify and validate if the value given is correct
            @warning To verify if the color is correct it's necessary to use QColor, but QColor is a
                     member of the dll GUI. Because this dll is not added by default, we cannot test
                     the color given; therefore this method don't test if the color is valid.
            @param parameterFormat The format expected of the parameter value
            @param paramName The name of the parameter
            @param value The color value to validate
            @param errorArray The JsonArray to fill with the error
            @return True if the parameter value is valid */
        static bool validateColorParameterValue(const JsonObject &parameterFormat,
                                                const QString &paramName,
                                                const QString &value,
                                                JsonArray &errorArray);

        /** @brief Verify and validate if the values given are valids and expected
            @note The method tests if all the values are in the list of expected values
            @param parameterFormat The format expected of the parameter value
            @param paramName The name of the parameter
            @param attributeListName The name of the attribute to get the parameter format
            @param values The values to validate
            @param errorArray The JsonArray to fill with the error
            @return True if the parameter values are valids */
        static bool validateListElemsParameterValue(const JsonObject &parameterFormat,
                                                    const QString &paramName,
                                                    const QString &attributeListName,
                                                    const QVector<QString> &values,
                                                    JsonArray &errorArray);

        /** @brief Verify and validate if the value given is valid and expected
            @note The method tests if the value is in the list of expected values
            @param parameterFormat The format expected of the parameter value
            @param paramName The name of the parameter
            @param attributeListName The name of the attribute to get the parameter format
            @param values The values to validate
            @param errorArray The JsonArray to fill with the error
            @return True if the parameter values are valids */
        static bool validateListElemsParameterValue(const JsonObject &parameterFormat,
                                                    const QString &paramName,
                                                    const QString &attributeListName,
                                                    const QString &value,
                                                    JsonArray &errorArray);

        /** @brief Get the string value of the parameter
            @param parameterValue The object which contains the parameter value
            @param objectName The parameter object name
            @param value The value got from the object
            @param errorArray The JsonArray to fill with the error
            @param valueIsRequired True if the value is required, and so an error is returned
                                   if the value is null
            @return True if an error occurs, or if the value is required and it's null */
        static bool getStringValue(const JsonObject &parameterValue,
                                   const QString &objectName,
                                   QString &value,
                                   JsonArray &errorArray,
                                   bool valueIsRequired = true);

        /** @brief Get the array string value of the parameter
            @param parameterValue The object which contains the parameter value
            @param objectName The parameter object name
            @param values The values got from the object
            @param errorArray The JsonArray to fill with the error
            @param valueIsRequired True if the values are required, and so an error is returned
                                   if the list is empty
            @return True if an error occurs, or if the values are required and the list is null */
        static bool getArrayStringValue(const JsonObject &parameterValue,
                                        const QString &objectName,
                                        QVector<QString> &values,
                                        JsonArray &errorArray,
                                        bool valueIsRequired = true);

        /** @brief Get the bool value of the parameter
            @param parameterValue The object which contains the parameter value
            @param objectName The parameter object name
            @param value The value got from the object
            @param errorArray The JsonArray to fill with the error
            @param valueIsRequired True if the value is required, and so an error is returned
                                   if the value is null
            @return True if an error occurs, or if the value is required and it's null */
        static bool getBoolValue(const JsonObject &parameterValue,
                                 const QString &objectName,
                                 bool &value,
                                 JsonArray &errorArray,
                                 bool valueIsRequired = true);

        /** @brief Get the array bool value of the parameter
            @param parameterValue The object which contains the parameter value
            @param objectName The parameter object name
            @param values The values got from the object
            @param errorArray The JsonArray to fill with the error
            @param valueIsRequired True if the values are required, and so an error is returned
                                   if the list is empty
            @return True if an error occurs, or if the values are required and the list is null */
        static bool getArrayBoolValue(const JsonObject &parameterValue,
                                      const QString &objectName,
                                      QVector<bool> &values,
                                      JsonArray &errorArray,
                                      bool valueIsRequired = true);

        /** @brief Get the number value of the parameter
            @param parameterValue The object which contains the parameter value
            @param objectName The parameter object name
            @param value The value got from the object
            @param errorArray The JsonArray to fill with the error
            @param valueIsRequired True if the value is required, and so an error is returned
                                   if the value is null
            @return True if an error occurs, or if the value is required and it's null */
        static bool getNumberValue(const JsonObject &parameterValue,
                                   const QString &objectName,
                                   Number &value,
                                   JsonArray &errorArray,
                                   bool valueIsRequired = true);

        /** @brief Get the array number value of the parameter
            @param parameterValue The object which contains the parameter value
            @param objectName The parameter object name
            @param values The values got from the object
            @param errorArray The JsonArray to fill with the error
            @param valueIsRequired True if the values are required, and so an error is returned
                                   if the list is empty
            @return True if an error occurs, or if the values are required and the list is null */
        static bool getArrayNumberValue(const JsonObject &parameterValue,
                                        const QString &objectName,
                                        QVector<Number> &values,
                                        JsonArray &errorArray,
                                        bool valueIsRequired = true);

        /** @brief Get the available keys got from a parameter list
            @note We expect that the value will be contained in this list
            @param parameterFormat The format expected of the parameter value
            @param attributeListName The name of the attribute to get the parameter format
            @param paramName The name of the parameter
            @param errorArray The JsonArray to fill with the error
            @param availableKeys The available keys got from the parameter list
            @return True if an error occurs */
        static bool getAvailableKeysFromListParam(const JsonObject &parameterFormat,
                                                  const QString &attributeListName,
                                                  const QString &paramName,
                                                  JsonArray &errorArray,
                                                  QVector<QString> &availableKeys);

        /** @brief Get an integer value from a variant
            @param value The value to convert
            @param paramName The name of the parameter
            @param formatParamAttributeName The name of the attribute concerned by the getting
            @param intValue The integer value got from the vairant
            @param errorArray The JsonArray to fill with the error
            @return True if an error occurs, or if the value couldn't be parsed to an integer */
        static bool getIntValue(const QVariant &value,
                                const QString &paramName,
                                const QString &formatParamAttributeName,
                                int &intValue,
                                JsonArray &errorArray);
};
