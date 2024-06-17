// SPDX-FileCopyrightText: 2020 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "abstractpluginelements/defaultsequenceinstancemodule.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"
#include "numberutility/number.hpp"


/** @brief The module says if an error occured because a module failed or a business error log
           occured (a log with warning level).
    @note This module is used to analyse the result of a group of modules, two scenarios are tested.
          But for all, the last sequence logs (number of logs to get is given by a parameter) are
          retrieved:
          - If the "inputSequenceSuccess" input value is equals to "True", the module will get the
            business sequence logs in errors, if at least one is an error we detect that an error
            occured
          - If the "inputSequenceSuccess" input value is equals to "False", the module will get the
            business sequence logs in errors */
class AnalyseResults : public DefaultSequenceInstanceModule
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(AnalyseResults)

    public:
        /** @brief Class constructor
            @param parent The class parent */
        Q_INVOKABLE explicit AnalyseResults(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~AnalyseResults() override;

    protected slots:
        /** @see DefaultSequenceInstanceModule::processModule
            @note Display an HMI to manage user cancellation if needed */
        virtual void processModule(const JsonArray &jsonObjectInputValues) override;

    protected:
        /** @brief Get the parameters format
            @param paramFormat The format of parameters
            @return True if no problem occurs */
        virtual bool initParamsFormat(JsonArray &paramFormat) const override;

        /** @brief Get the inputs format
            @param inputFormat The format of inputs
            @return True if no problem occurs*/
        virtual bool initInputsFormat(JsonArray &inputFormat) const override;

        /** @brief Get the outputs format
            @param inputFormat The format of outputs
            @return True if no problem occurs*/
        virtual bool initOutputsFormat(JsonArray &outputFormat) const override;

        /** @see DefaultSequenceInstanceModule::validateAndSetParameters
            @note @ref tailErrorLogsNbParamName : number (Max logs error to get) */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

    private:
        /** @brief Got error logs and analyse results
            @param sequenceSuccess True if the sequence has successed until now
            @param lastLogs Return the last logs in errors and following the limited number
            @return True if no problem occurs */
        bool analyseResults(bool sequenceSuccess, QVector<QString> &lastLogs);

    private:
        static const constexpr char *TailErrorLogsNbParamName = "tailErrorLogsNb";
        static const constexpr char *TailErrorLogsNbParamDesc = "Number of error logs to get. "
                                                                "-1 to get all";

        static const constexpr char *InputSuccessName = "inputSequenceSuccess";
        static const constexpr char *InputSuccessDesc = "Current sequence state";

        static const constexpr char *OutputSuccessName = "outputSequenceSuccess";
        static const constexpr char *OutputSuccessDesc = "Result of the sequence success after "
                                                         "analyse";

        static const constexpr char *OutputLastErrorLogsName = "outputLastErrorLogs";
        static const constexpr char *OutputLastErrorLogsDesc = "Last error logs";

        static const constexpr char *LastErrorLogsTitle = "Last occured errors:";

    private:
        int _tailErrorLogsNb{0};
};
