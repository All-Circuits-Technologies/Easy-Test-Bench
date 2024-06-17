// SPDX-FileCopyrightText: 2020 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "abstractpluginelements/defaultsequenceinstancemodule.hpp"

class QFile;


/** @brief This is the abstract class to communicate with the JLink box */
class AJLinkInterface : public DefaultSequenceInstanceModule
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param moduleClassName The class name of the module
            @param parent The class parent */
        explicit AJLinkInterface(const QString &moduleClassName, QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~AJLinkInterface() override;

    protected:
        /** @copydoc DefaultSequenceInstanceModule::initInputsFormat
            @note @ref InputJLinkPgmPathName : string (The JLink absolute program path) */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

        /** @copydoc DefaultSequenceInstanceModule::initParamsFormat
            @note @ref TimeoutMsParamName : number (Timeout in Ms) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @copydoc DefaultSequenceInstanceModule::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

        /** @brief This method has to be overriden by derived classes to manage the module process
            @param jsonObjectInputValues The module inputs values
            @param jLinkPgmPath The JLink program path
            @param output The module outputs
            @return True if no problem occurred */
        virtual bool mainJLinkProcess(const JsonArray &jsonObjectInputValues,
                                      const QString &jLinkPgmPath,
                                      JsonArray &output) = 0;

    protected:
        /** @brief Get the timeout in milliseconds */
        int getTimeoutInMs() const { return _timeoutInMs; }

        /** @brief Call the JLink program
            @param jLinkScriptFile The JLink script file to call
            @param jLinkPgmPath The path to of the JLink program
            @param timeoutInMs The process execution timeout, if -1 the timeout is disabled
            @param logProcessError If true and if an error occured, they will be written in logs
            @param processExitProperly If not null, this will say if the process exit properly,
                                       the param is equals to false except when the process succeeds
                                       and no error occurs
            @return True if no problem occurred */
        bool callJLinkPgm(const QFile &jLinkScriptFile,
                          const QString &jLinkPgmPath,
                          int timeoutInMs = -1,
                          bool logProcessError = true,
                          bool *processExitProperly = nullptr);

    protected slots:
        /** @brief Execute the task
            @copydoc DefaultSequenceInstanceModule::processModule */
        virtual void processModule(const JsonArray &jsonObjectInputValues) override;

    private:
        /** @brief Execute the process and extract the common inputs. The method calls
                   @ref mainJLinkProcess
            @param jsonObjectInputValues The module inputs values
            @param output The module outputs
            @return True if no problem occurred */
        bool localJLinkProcess(const JsonArray &jsonObjectInputValues, JsonArray &output);

    protected:
        /** @brief Create a temporary file which will contain all the commands to play in the
                   JLink.exe application
            @param commands The commands to add in temporary file, in order to be called by JLink
            @param tmpFile The temporary file to call in the JLink app
            @return True if a problem occurred */
        static bool createCommandScriptFile(const QStringList &commands, QFile &tmpFile);

    protected:
        /** @brief This is the default timeout in milliseconds to execute the JLink process*/
        static const constexpr int DefaultTimeoutInMs = 1500;

    private:
        static const constexpr char *InputJLinkPgmPathName = "jLinkPgmPath";
        static const constexpr char *InputJLinkPgmPathDesc = "The JLink absolute program path";

        static const constexpr char *TimeoutMsParamName = "timeoutInMs";
        static const constexpr char *TimeoutMsParamDesc = "Program timeout in Ms";

    private:
        /** @brief Character number limit, in order to not display all the output of JLink output
                   to debug, only the last char */
        static const constexpr int ProcessLogCharLimitToDisplay { 900 };

        /** @brief This is the suffix of JLink file */
        static const constexpr char *JLinkSuffix = ".jlink";

        /** @brief This is the default name of temporary JLink files */
        static const constexpr char *JLinkTmpFileBaseName = "tmp-testbed";

        /** @brief This pattern is used to add a directory path to a JLink file name */
        static const constexpr char *JLinkCompleteTmpFilePath = "%1/%2";

        /** @brief This is the pattern used with @ref QTemporaryFile to generate random file name */
        static const constexpr char *JLinkFileNameTmpPart = "%1-XXXXXX%2";

    private:
        int _timeoutInMs{DefaultTimeoutInMs};
};
