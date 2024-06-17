// SPDX-FileCopyrightText: 2023-2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "functions/ajlinkinterface.hpp"
#include "multi-args-lib/mixinmultistringargsmodule.hpp"


/** @brief This is the abstract class to use external JLink file (commander script)
           The class allows to replace variables in the given JLink file by values given in input */
class AJLinkIntfFromFile : public AJLinkInterface, public MixinMultiStringArgsModule
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param argumentsNb The number of variables to replace in the external JLink file
            @param moduleClassName The class name of the module
            @param parent The class parent */
        Q_INVOKABLE explicit AJLinkIntfFromFile(int argumentsNb,
                                                const QString &moduleClassName,
                                                QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~AJLinkIntfFromFile() override;

    protected:
        /** @copydoc DefaultSequenceInstanceModule::initInputsFormat
            @note @ref InputJLinkFilePathName : string (The JLink file to use)
                  It may have several arguments with this name */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

        /** @copydoc DefaultSequenceInstanceModule::initParamsFormat
            @note @ref InputJLinkFilePathName : string (The JLink file to use)
                  It may have several arguments with this name */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @copydoc DefaultSequenceInstanceModule::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

        /** @copydoc AJLinkInterface::mainJLinkProcess */
        virtual bool mainJLinkProcess(const JsonArray &jsonObjectInputValues,
                                      const QString &jLinkPgmPath,
                                      JsonArray &output) override;

    private:
        /** @brief Operate the process, extract the arguments values and replace them in the
                   JLink file
            @param jsonObjectInputValues The module inputs values
            @param jLinkPgmPath The JLink program path
            @return True if no problem occurred */
        bool operate(const JsonArray &jsonObjectInputValues, const QString &jLinkPgmPath);

    private:
        /** @brief Create a temporary command script file, from the JLink file given, and replace
                   the arguments values
            @param commands The commands to add in temporary file, in order to be called by JLink
            @param tmpFile The temporary file to call in the JLink app
            @return True if a problem occurred */
        bool createCommandScriptFromFile(const QString &jLinkFile,
                                         const QVector<QString> &inputValues,
                                         QFile &tmpFile);

    private:
        static const constexpr char *InputJLinkFilePathName = "jLinkFile";
        static const constexpr char *InputJLinkFilePathDesc = "The JLink file to use";

        static const constexpr char *InputArgumentName = "argument%1";
        static const constexpr char *InputArgumentDesc = "This is the value of the %1nth argument "
                                                         "used in the JLinkFile";

        static const constexpr char *ArgumentKeyParamName = "argumentKey%1";
        static const constexpr char *ArgumentKeyParamDesc = "This contains the key to replace in "
                                                            "the linked JLinkFile and matches the "
                                                            "%nth argument input.";


        static const constexpr char* HmiName = "CallJLinkFile";
        static const constexpr char* HmiDescription = QT_TR_NOOP("(TR) We are interacting with the "
                                                                 "board, this can take a few "
                                                                 "seconds.");
};
