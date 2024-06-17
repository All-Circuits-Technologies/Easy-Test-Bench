// SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "serial-link-lib/acallscriptviaseriallink.hpp"
#include "multi-args-lib/mixinmultistringargsmodule.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief This module lets the user calls a script through a serial link
    @note The script in parameters may have an argument key in it to replace with a value got from
          the inputs */
class OneArgScriptCallViaSerialLink : public ACallScriptViaSerialLink,
                                      public MixinMultiStringArgsModule
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(OneArgScriptCallViaSerialLink)

    public:
        /** @brief Class constructor
            @param parent The parent object */
        Q_INVOKABLE explicit OneArgScriptCallViaSerialLink(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~OneArgScriptCallViaSerialLink() override;

    protected:
        /** @copydoc ACallScriptViaSerialLink::initInputsFormat
            @copydoc MixinMultiStringArgsModule::initArgsInputsFormat */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

        /** @copydoc ACallScriptViaSerialLink::initParamsFormat
            @copydoc MixinMultiStringArgsModule::initArgsParamsFormat */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @copydoc ACallScriptViaSerialLink::validateAndSetParameters
            @copydoc MixinMultiStringArgsModule::validateAndSetArgsParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

        /** @copydoc ACallScriptViaSerialLink::replaceArgumentsInString
            @note Update the @ref scriptToCall with the argument given and set it into
                  @ref managedScriptToCall */
        virtual bool manageScriptToCall(const JsonArray &jsonObjectInputValues,
                                        const QString &scriptToCall,
                                        QString &managedScriptToCall) override;
};
