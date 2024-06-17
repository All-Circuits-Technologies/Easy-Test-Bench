// SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QVector>
#include <QString>

#include "jsonutility/jsontypedefs.hpp"


/** @brief This mixin is used to replace a key in a text with a string input linked to the
           argument key
    @note @ref _argumentsNb defines the number of arguments we manage in the module and adds the
          needed parameters and inputs */
class MixinMultiStringArgsModule
{
    public:
        /** @brief Class constructor
            @param argumentsNb The number of arguments to manage */
        explicit MixinMultiStringArgsModule(int argumentsNb);

        /** @brief Class destructor */
        virtual ~MixinMultiStringArgsModule();

    protected:
        /** @brief Adds the inputs format
            @note @ref InputArgumentName : string (This is the value of the %1nth argument used in
                                                   the linked module)
                  It may have several arguments with this name
            @param inputsFormat The inputs format
            @return True if no problem occurs */
        bool initArgsInputsFormat(JsonArray &inputsFormat) const;

        /** @brief Adds the parameters format
            @note @ref ArgumentKeyParamName : string (This contains the key to replace in the given
                                                      text and matches the %nth argument input.)
            @param paramsFormat The parameter format
            @return True if no problem occurs */
        bool initArgsParamsFormat(JsonArray &paramsFormat) const;

        /** @brief Validate and set the parameters into the module
            @param parameters The parameters to test given by the test lib
            @param errors If the parameters haven't been correctly set, this array is filled with
                          informations to display to the user; and so he can understand what to
                          correct
            @return True if no problem occurs */
        bool validateAndSetArgsParameters(const JsonArray &parameters, JsonArray &errors);

    protected:
        /** @brief Extract arguments values from inputs
            @param jsonObjectInputValues The module inputs values
            @param inputValues The input values linked to arguments, parsed and got from module
            @return True if no problem occurred */
        bool extractArgumentsFromInputs(const JsonArray &jsonObjectInputValues,
                                        QVector<QString> &inputValues) const;

        /** @brief Replace in the text the arguments with the input values
            @param inputValues The input values got and to replace in the text
            @return True if no problem occurred */
        bool replaceArgumentsInString(const QVector<QString> &inputValues,
                                      QString &textToReplaceIn);

        /** @brief Get the values in inputs and replace the arguments in the text with the input
                   values got
            @param jsonObjectInputValues The module json object input values
            @param textToReplaceIn Replace the argumentKeys by the linked input values got
            @return True if no problem occurred */
        bool replaceArgumentsInString(const JsonArray &jsonObjectInputValues,
                                      QString &textToReplaceIn);

        /** @brief Get the arguments number */
        int getArgumentsNb() const { return _argumentsNb; }

        /** @brief Get the arguments keys got from parameters */
        const QVector<QString> &getArgumentsKeys() const { return _argumentsKeys; }

    private:
        static const constexpr char *InputArgumentName = "argument%1";
        static const constexpr char *InputArgumentDesc = "This is the value of the %1nth argument";

        static const constexpr char *ArgumentKeyParamName = "argumentKey%1";
        static const constexpr char *ArgumentKeyParamDesc = "This contains the %1nth argument key "
                                                            "to replace with value got from input.";

    private:
        int _argumentsNb;
        QVector<QString> _argumentsKeys;
};
