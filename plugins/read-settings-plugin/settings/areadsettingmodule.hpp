// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "abstractpluginelements/defaultsequenceinstancemodule.hpp"


/** @brief Abstract module to manage setting reading */
class AReadSettingModule : public DefaultSequenceInstanceModule
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param moduleClassName The class name of the module
            @param parent The parent class */
        explicit AReadSettingModule(const QString &moduleClassName,
                                    QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~AReadSettingModule() override = default;

    protected slots:
        /** @brief DefaultSequenceInstanceModule::processModule */
        virtual void processModule(const JsonArray &jsonObjectInputValues) override;

    protected:
        /** @brief Format and convert the value read to the right output value type
            @note The output can already contains values; therefore it's important to not clear the
                  object and only append new values
            @param value The value to convert
            @param output The output to append the value read in it */
        virtual bool formatOutputWithReadValue(const QVariant &value, JsonArray &output) = 0;

        /** @see DefaultSequenceInstanceModule::initOutputsFormat
            @note @ref outputSuccessName : boolean (This output is updated with the result of the
                                                    module action) */
        virtual bool initOutputsFormat(JsonArray &outputsFormat) const override;

        /** @see DefaultSequenceInstanceModule::initParamsFormat
            @note @ref settingKeyParamName : string (The setting key to get the value wanted)
            @note @ref emitErrorIfFailParamName : boolean (If the option is active and an error
                                                           occurs in the module, the module will
                                                           fail) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @see DefaultSequenceInstanceModule::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

    protected:
        /** @brief Get the key of the wanted setting */
        const QString &getSettingKey() const { return _settingKey; }

    private:
        /** @brief Do the setting reading and fill the output with the result
            @param output The output filled with the value read
            @return True if no problem occurs */
        bool processReading(JsonArray &output);

    protected:
        static const constexpr char * OutputSettingValueName = "settingValueOutput";
        static const constexpr char * OutputSettingValueDesc = "The setting output value";

    private:
        static const constexpr char *SettingKeyParamName = "settingKey";
        static const constexpr char *SettingKeyParamDesc = "The setting key to get the value "
                                                           "wanted";

        static const constexpr char * EmitErrorIfFailParamName = "exitIfFail";
        static const constexpr char * EmitErrorIfFailParamDesc = "If the option is active and an "
                                                                 "error occurs in the module, "
                                                                 "the module will fail";
        static const constexpr bool DefaultEmitErrorIfFailValue = false;

        static const constexpr char * OutputSuccessName = "successOutput";
        static const constexpr char * OutputSuccessDesc = "This output is updated with the result "
                                                          "of the module action";

    private:
        QString _settingKey;
        bool _exitIfFail{DefaultEmitErrorIfFailValue};
};
