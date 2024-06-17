// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QDate>
#include <QDateTime>
#include <QTime>

#include "jsonutility/jsontypedefs.hpp"
#include "numberutility/number.hpp"
#include "testbedcore/types/typemanaged.hpp"
#include "testbedcore/types/unit.hpp"


/** @brief This class contains helping functions to create JsonObject Bank parameters */
class BankJsonFormatter
{
    private:
        /** @brief Class constructor */
        BankJsonFormatter() {}

    public:
        /** @brief Help to fill the string parameter format given with the required attributes
            @see ISequenceInstanceModule::getModuleParametersFormat
            @note The JsonObject given is cleaned before to add attributes
            @param name The name of the parameter format
            @param description The description of the parameter format
            @param param The parameter format to fill
            @return True if no problem occurs */
        static bool formatParamStringRequired(const QString &name,
                                              const QString &description,
                                              JsonObject &param);

        /** @brief Add the description attribute into the param given
            @see ISequenceInstanceModule::getModuleParametersFormat
            @param description The description of the parameter format
            @param param The parameter format to fill
            @return True if no problem occurred */
        static bool formatParamDescription(const QString &description, JsonObject &param);

        /** @brief Add the min and max length attribute to a string parameter format
            @see ISequenceInstanceModule::getModuleParametersFormat
            @param param The parameter format to fill
            @param minLength The minimum length expected for the parameter string value
            @param maxLength The maximum length expected for the parameter string value
            @return True if no problem occurs */
        static bool formatParamMinMaxString(JsonObject &param,
                                            int minLength = -1,
                                            int maxLength = -1);

        /** @brief Add the "clear input if error" information to a string parameter format
            @see ISequenceInstanceModule::getModuleParametersFormat
            @param clearInputIfError If true, the input is clear when the form is validated and an
                                     error is detected.
            @param param The parameter format to fill
            @return True if no problem occurred */
        static bool formatParamClearInputIfError(bool clearInputIfError, JsonObject &param);

        /** @brief Add the pattern regex attribute to a string parameter format
            @see ISequenceInstanceModule::getModuleParametersFormat
            @param patternRegex The pattern regexp to verify the parameter string value
            @param param The parameter format to fill
            @return True if no problem occurs */
        static bool formatParamPatternRegex(const QString &patternRegex, JsonObject &param);

        /** @brief Add the timeout attribute to a parameter format
            @see ISequenceInstanceModule::getModuleParametersFormat
            @param timeoutInSec The timeout of the widget displaying
            @param param The parameter format to fill
            @return True if no problem occurs */
        static bool formatParamTimeoutInSec(int timeoutInSec, JsonObject &param);

        /** @brief Add the image path attribute to a parameter format
            @see ISequenceInstanceModule::getModuleParametersFormat
            @param imagePath The path of the image to display with the widget
            @param param The parameter format to fill
            @return True if no problem occurs */
        static bool formatParamImagePath(const QString &imagePath, JsonObject &param);

        /** @brief Add the default value attribute to the parameters format with the string value
            @see ISequenceInstanceModule::getModuleParametersFormat
            @param value The default value of the parameter format
            @param param The parameter format to fill
            @return True if no problem occurs */
        static bool formatParamDefaultValueString(const QString &value, JsonObject &param);

        /** @brief Add the hide attribute to a parameter format
            @see ISequenceInstanceModule::getModuleParametersFormat
            @param hide True to hide the widget (to not display it)
            @param param The parameter format to fill
            @return True if no problem occurs */
        static bool formatParamHide(bool hide, JsonObject &param);

        /** @brief Add the required attribute to a parameter format
            @see ISequenceInstanceModule::getModuleParametersFormat
            @param required If true the input value is required
            @param param The parameter format to fill
            @return True if no problem occurs */
        static bool formatParamRequired(bool required, JsonObject &param);

        /** @brief Help to fill the string list parameter format given with the required attributes
            @see ISequenceInstanceModule::getModuleParametersFormat
            @param name The name of the parameter format
            @param description The description of the parameter format
            @param param The parameter format to fill
            @return True if no problem occurs */
        static bool formatParamStringListRequired(const QString &name,
                                                  const QString &description,
                                                  JsonObject &param);

        /** @brief Add the min and max length attribute to a string list parameter format
            @see ISequenceInstanceModule::getModuleParametersFormat
            @param param The parameter format to fill
            @param minListLength The minimum list length expected for the parameter string value
            @param maxListLength The maximum list length expected for the parameter string value
            @return True if no problem occurs */
        static bool formatParamMinMaxStringList(JsonObject &param,
                                                int minListLength = -1,
                                                int maxListLength = -1);

        /** @brief Add the default value attribute to the parameters format with the string list
                   values
            @see ISequenceInstanceModule::getModuleParametersFormat
            @param values The default values of the parameter format
            @param param The parameter format to fill
            @return True if no problem occurs */
        static bool formatParamDefaultValuesStringList(const QVector<QString> &values,
                                                       JsonObject &param);

        /** @brief Help to fill the bool parameter format given with the required attributes
            @see ISequenceInstanceModule::getModuleParametersFormat
            @note The JsonObject given is cleaned before to add attributes
            @param name The name of the parameter format
            @param description The description of the parameter format
            @param param The parameter format to fill
            @return True if no problem occurs */
        static bool formatParamBoolRequired(const QString &name,
                                            const QString &description,
                                            JsonObject &param,
                                            bool isList = false);

        /** @brief Add the default value attribute to the parameters format with the bool value
            @see ISequenceInstanceModule::getModuleParametersFormat
            @param value The default value of the parameter format
            @param param The parameter format to fill
            @return True if no problem occurs */
        static bool formatParamDefaultValueBool(bool value, JsonObject &param);

        /** @brief Help to fill the number parameter format given with the required attributes
            @see ISequenceInstanceModule::getModuleParametersFormat
            @note The JsonObject given is cleaned before to add attributes
            @param name The name of the parameter format
            @param description The description of the parameter format
            @param commaShift The shift of the comma in the previous unsigned integer given
            @param nbAfterCommaToDisplay The number of shift to display after the comma
            @param param The parameter format to fill
            @return True if no problem occurs */
        static bool formatParamNumberRequired(const QString &name,
                                              const QString &description,
                                              int commaShift,
                                              int nbAfterCommaToDisplay,
                                              JsonObject &param,
                                              bool isList = false);

        /** @brief Add the step, min and max value to a Number parameter format
            @see ISequenceInstanceModule::getModuleParametersFormat
            @param param The parameter format to fill
            @param min The value minimum expected for the parameter number value
            @param max The value maximum expected for the parameter number value
            @param step The input step for modifying the value
            @return True if no problem occurs */
        static bool formatParamStepMinMaxNumber(JsonObject &param,
                                                const Number &min = Number(),
                                                const Number &max = Number(),
                                                const Number &step = Number());

        /** @brief Add the unit attributes to a number parameter format
            @see ISequenceInstanceModule::getModuleParametersFormat
            @note If unit type is none, no attribute is added
            @param unit The unit to associate with the number value to get
            @param param The parameter format to fill
            @return True if no problem occurs */
        static bool formatParamUnitNumber(const Unit &unit, JsonObject &param);

        /** @brief Add the default value attribute to the parameters format with the number value
            @see ISequenceInstanceModule::getModuleParametersFormat
            @param value The default value of the parameter format
            @param param The parameter format to fill
            @return True if no problem occurs */
        static bool formatParamDefaultValueNumber(const Number &value, JsonObject &param);

        /** @brief Help to fill the date parameter format given with the required attributes
            @see ISequenceInstanceModule::getModuleParametersFormat
            @note The JsonObject given is cleaned before to add attributes
            @param name The name of the parameter format
            @param description The description of the parameter format
            @param param The parameter format to fill
            @return True if no problem occurs */
        static bool formatParamDateRequired(const QString &name,
                                            const QString &description,
                                            JsonObject &param);

        /** @brief Add the min and max attributes to a Date parameter format
            @see ISequenceInstanceModule::getModuleParametersFormat
            @param param The parameter format to fill
            @param min The value minimum expected for the parameter date value
            @param max The value maximum expected for the parameter date value
            @return True if no problem occurs */
        static bool formatParamMinMaxDate(JsonObject &param,
                                          const QDate &min = QDate(),
                                          const QDate &max  = QDate());

        /** @brief Add the default value attribute to the parameters format with the date value
            @see ISequenceInstanceModule::getModuleParametersFormat
            @param value The default value of the parameter format
            @param param The parameter format to fill
            @return True if no problem occurs */
        static bool formatParamDefaultValueDate(const QDate &value, JsonObject &param);

        /** @brief Help to fill the time parameter format given with the required attributes
            @see ISequenceInstanceModule::getModuleParametersFormat
            @note The JsonObject given is cleaned before to add attributes
            @param name The name of the parameter format
            @param description The description of the parameter format
            @param param The parameter format to fill
            @return True if no problem occurs */
        static bool formatParamTimeRequired(const QString &name,
                                            const QString &description,
                                            JsonObject &param);

        /** @brief Add the min and max attributes to a Time parameter format
            @see ISequenceInstanceModule::getModuleParametersFormat
            @param param The parameter format to fill
            @param min The value minimum expected for the parameter time value
            @param max The value maximum expected for the parameter time value
            @return True if no problem occurs */
        static bool formatParamMinMaxTime(JsonObject &param,
                                          const QTime &min = QTime(),
                                          const QTime &max  = QTime());

        /** @brief Add the default value attribute to the parameters format with the time value
            @see ISequenceInstanceModule::getModuleParametersFormat
            @param value The default value of the parameter format
            @param param The parameter format to fill
            @return True if no problem occurs */
        static bool formatParamDefaultValueTime(const QTime &value, JsonObject &param);

        /** @brief Help to fill the datetime parameter format given with the required attributes
            @see ISequenceInstanceModule::getModuleParametersFormat
            @note The JsonObject given is cleaned before to add attributes
            @param name The name of the parameter format
            @param description The description of the parameter format
            @param param The parameter format to fill
            @return True if no problem occurs */
        static bool formatParamDateTimeRequired(const QString &name,
                                                const QString &description,
                                                JsonObject &param);

        /** @brief Add the min and max attributes to a DateTime parameter format
            @see ISequenceInstanceModule::getModuleParametersFormat
            @param param The parameter format to fill
            @param min The value minimum expected for the parameter datetime value
            @param max The value maximum expected for the parameter datetime value
            @return True if no problem occurs */
        static bool formatParamMinMaxDateTime(JsonObject &param,
                                              const QDateTime &min = QDateTime(),
                                              const QDateTime &max  = QDateTime());

        /** @brief Add the default value attribute to the parameters format with the datetime value
            @see ISequenceInstanceModule::getModuleParametersFormat
            @param value The default value of the parameter format
            @param param The parameter format to fill
            @return True if no problem occurs */
        static bool formatParamDefaultValueDateTime(const QDateTime &value, JsonObject &param);

        /** @brief Help to fill the select parameter format given with the required attributes
            @see ISequenceInstanceModule::getModuleParametersFormat
            @note The JsonObject given is cleaned before to add attributes
            @param name The name of the parameter format
            @param description The description of the parameter format
            @param param The parameter format to fill
            @param multiple True to allow multiple result value
            @return True if no problem occurs */
        static bool formatParamSelectRequired(const QString &name,
                                              const QString &description,
                                              JsonObject &param,
                                              bool multiple = false);

        /** @brief Help to add a select option
            @see ISequenceInstanceModule::getModuleParametersFormat
            @param key The option key
            @param value The option value
            @param param The parameter format to fill
            @param hide True to hide the widget (to not display it)
            @return True if no problem occurs */
        static bool formatParamAddSelectOption(const QString &key,
                                               const QString &value,
                                               JsonObject &param,
                                               bool hide = false);

        /** @brief Help to add default options
            @see ISequenceInstanceModule::getModuleParametersFormat
            @param defaultValue The option default values
            @param param The parameter format to fill
            @return True if no problem occurs */
        static bool formatParamDefaultOption(const QVector<QString> &defaultValues,
                                             JsonObject &param);

        /** @brief Help to fill the check box list parameter format given with the required
                   attributes
            @see ISequenceInstanceModule::getModuleParametersFormat
            @note The JsonObject given is cleaned before to add attributes
            @param name The name of the parameter format
            @param description The description of the parameter format
            @param param The parameter format to fill
            @return True if no problem occurs */
        static bool formatParamCheckBoxListRequired(const QString &name,
                                                    const QString &description,
                                                    JsonObject &param);

        /** @brief Help to add a list elements
            @see ISequenceInstanceModule::getModuleParametersFormat
            @param key The option key
            @param param The parameter format to fill
            @param hide True to hide the widget (to not display it)
            @return True if no problem occurs */
        static bool formatParamAddListElem(const QString &key,
                                           JsonObject &param,
                                           bool hide = false);

        /** @brief Add the default value attribute to the parameters format with the checked list
                   value
            @see ISequenceInstanceModule::getModuleParametersFormat
            @param checkedList The default value of the parameter format
            @param param The parameter format to fill
            @return True if no problem occurs */
        static bool formatParamDefaultCheckedList(const QVector<QString> &checkedList,
                                                  JsonObject &param);

        /** @brief Help to fill the radio list parameter format given with the required attributes
            @see ISequenceInstanceModule::getModuleParametersFormat
            @note The JsonObject given is cleaned before to add attributes
            @param name The name of the parameter format
            @param description The description of the parameter format
            @param param The parameter format to fill
            @return True if no problem occurs */
        static bool formatParamRadioListRequired(const QString &name,
                                                 const QString &description,
                                                 JsonObject &param);

        /** @brief Add the default value attribute to the parameters format with the radio list
                   value
            @see ISequenceInstanceModule::getModuleParametersFormat
            @param defaultChecked The default value of the parameter format
            @param param The parameter format to fill
            @return True if no problem occurs */
        static bool formatParamRadioListDefault(const QString &defaultChecked,
                                                JsonObject &param);

        /** @brief Help to fill the radio list buttons parameter format given with the required
                   attributes
            @see ISequenceInstanceModule::getModuleParametersFormat
            @note The JsonObject given is cleaned before to add attributes
            @param name The name of the parameter format
            @param description The description of the parameter format
            @param param The parameter format to fill
            @return True if no problem occurs */
        static bool formatParamRadioListButtonsRequired(const QString &name,
                                                        const QString &description,
                                                        JsonObject &param);

        /** @brief Help to add a radio list button element
            @see ISequenceInstanceModule::getModuleParametersFormat
            @param key The option key
            @param label The label to display in the button
            @param param The parameter format to fill
            @param keyShortcut The key shortcut to tap in order to fire the button
            @param fontColor The button font color to display
            @param backgroundColor The button background color to display
            @param hide True to hide the widget (to not display it)
            @return True if no problem occurs */
        static bool formatParamAddListButtonElem(const QString &key,
                                                 const QString &label,
                                                 JsonObject &param,
                                                 char keyShortcut = -1,
                                                 const QString &fontColor = QString(),
                                                 const QString &backgroundColor = QString(),
                                                 bool hide = false);

        /** @brief Help to fill the ok button parameter format given with the required attributes
            @see ISequenceInstanceModule::getModuleParametersFormat
            @note The JsonObject given is cleaned before to add attributes
            @param name The name of the parameter format
            @param description The description of the parameter format
            @param key The button key
            @param label The label to display in the button
            @param param The parameter format to fill
            @param keyShortcut The key shortcut to tap in order to fire the button
            @param fontColor The button font color to display
            @param backgroundColor The button background color to display
            @param hide True to hide the widget (to not display it)
            @return True if no problem occurs */
        static bool formatParamOkButtonRequired(const QString &name,
                                                const QString &description,
                                                const QString &key,
                                                const QString &label,
                                                JsonObject &param,
                                                char keyShortcut = -1,
                                                const QString &fontColor = QString(),
                                                const QString &backgroundColor = QString(),
                                                bool hide = false);

        /** @brief Help to fill the color parameter format given with the required attributes
            @see ISequenceInstanceModule::getModuleParametersFormat
            @note The JsonObject given is cleaned before to add attributes
            @param name The name of the parameter format
            @param description The description of the parameter format
            @param param The parameter format to fill
            @return True if no problem occurs */
        static bool formatParamDefaultColor(const QString &name,
                                            const QString &description,
                                            JsonObject &param);

        /** @brief Help to fill the select file parameter format given with the required attributes
            @see ISequenceInstanceModule::getModuleParametersFormat
            @note The JsonObject given is cleaned before to add attributes
            @param name The name of the parameter format
            @param description The description of the parameter format
            @param param The parameter format to fill
            @param selectDir True if the element wanted is a directory, or False if it's a file
            @param filesExtensions The expected files extensions
            @param currentDirectory The directory where to open the file dialog
            @return True if no problem occurs */
        static bool formatParamSelectFileRequired(const QString &name,
                                                  const QString &description,
                                                  JsonObject &param,
                                                  bool selectDir = false,
                                                  const QVector<QString> &filesExtensions =
                                                                                QVector<QString>(),
                                                  const QString &currentDirectory = QString());

        /** @brief Help to fill the string input and output format with the possible attributes
            @param name The name of the input/output format
            @param inOut The input/output format to fill
            @param description The description of the parameter format
            @param defaultValue The default value of the input/output format
            @return True if no problem occurs */
        static bool formatInOutString(const QString &name,
                                      JsonObject &inOut,
                                      const QString &description = QString(),
                                      const QString &defaultValue = QString());

        /** @brief Help to fill the bool input and output format with the possible attributes
            @param name The name of the input/output format
            @param inOut The input/output format to fill
            @param description The description of the parameter format
            @param defaultValue The default value of the input/output format
            @return True if no problem occurs */
        static bool formatInOutBool(const QString &name,
                                    JsonObject &inOut,
                                    const QString &description = QString(),
                                    const bool *defaultValue = nullptr);

        /** @brief Help to fill the number input and output format with the possible attributes
            @param name The name of the input/output format
            @param inOut The input/output format to fill
            @param description The description of the parameter format
            @param defaultValue The default value of the input/output format
            @param unit The unit to associate with the number value to get
            @return True if no problem occurs */
        static bool formatInOutNumber(const QString &name,
                                      JsonObject &inOut,
                                      const QString &description = QString(),
                                      const Number &defaultValue = Number(),
                                      const Unit &unit = Unit());

        /** @brief Help to fill the string array input and output format with the possible
                   attributes
            @param name The name of the input/output format
            @param inOut The input/output format to fill
            @param description The description of the parameter format
            @param defaultValue The default values of the input/output format
            @return True if no problem occurs */
        static bool formatInOutStringArray(const QString &name,
                                           JsonObject &inOut,
                                           const QString &description = QString(),
                                           const QVector<QString> &defaultValues =
                                                                                QVector<QString>());

        /** @brief Help to fill the bool array input and output format with the possible attributes
            @param name The name of the input/output format
            @param inOut The input/output format to fill
            @param description The description of the parameter format
            @param defaultValue The default values of the input/output format
            @return True if no problem occurs */
        static bool formatInOutBoolArray(const QString &name,
                                         JsonObject &inOut,
                                         const QString &description = QString(),
                                         const QVector<bool> &defaultValues = QVector<bool>());

        /** @brief Help to fill the number array input and output format with the possible
                   attributes
            @param name The name of the input/output format
            @param description The description of the parameter format
            @param inOut The input/output format to fill
            @param defaultValue The default values of the input/output format
            @param unit The unit to associate with the number value to get
            @return True if no problem occurs */
        static bool formatInOutNumberArray(const QString &name,
                                           JsonObject &inOut,
                                           const QString &description = QString(),
                                           const QVector<Number> &defaultValues = QVector<Number>(),
                                           const Unit &unit = Unit());

    private:
        /** @brief Help to fill the default and required attributes of all parameters format
            @see ISequenceInstanceModule::getModuleParametersFormat
            @note The JsonObject given is cleaned before to add attributes
            @param name The name of the parameter format
            @param type The type of the parameter format
            @param description The description of the parameter format
            @param param The parameter format to fill
            @return True if no problem occurs */
        static bool formatParamDefaultAttrs(const QString &name,
                                            const QString &type,
                                            const QString &description,
                                            JsonObject &param);

        /** @brief Help to fill the default and required attributes of all input/output format
            @see ISequenceInstanceModule::getInputVariablesFormat
            @note The JsonObject given is cleaned before to add attributes
            @param name The name of the input/output format
            @param typeManaged The type of the input/output format
            @param inOut The input/output format to fill
            @param description The description of the input/output format
            @return True if no problem occurs */
        static bool formatInOutDefaultAttrs(const QString &name,
                                            TypeManaged::Enum typeManaged,
                                            JsonObject &inOut,
                                            const QString &description = QString());

        /** @brief Verify if the param given has an authorized type
            @param param The parameter format
            @param authorizedType The authorized types
            @return True if no problem occurs */
        static bool verifyParamType(const JsonObject &param,
                                    const QVector<QString> &authorizedType);

        /** @brief Help to fill the default attributes for a button element
            @see ISequenceInstanceModule::getModuleParametersFormat
            @note The JsonObject given is cleaned before to add attributes
            @param key The button key
            @param label The label to display in the button
            @param param The parameter format to fill
            @param keyShortcut The key shortcut to tap in order to fire the button
            @param fontColor The button font color to display
            @param backgroundColor The button background color to display
            @param hide True to hide the widget (to not display it)
            @return True if no problem occurs */
        static bool formatParamButtonElem(const QString &key,
                                          const QString &label,
                                          JsonObject &param,
                                          char keyShortcut = -1,
                                          const QString &fontColor = QString(),
                                          const QString &backgroundColor = QString(),
                                          bool hide = false);

};
