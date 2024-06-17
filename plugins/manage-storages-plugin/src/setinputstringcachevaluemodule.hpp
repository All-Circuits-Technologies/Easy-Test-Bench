// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "src/aprojectcachemodule.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief Set a string value to store in the project cache, found thanks to a key.
    @note If no value already exists in the cache with the key, it will be created. */
class SetInputStringCacheValueModule : public AProjectCacheModule
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(SetInputStringCacheValueModule)

    public:
        /** @brief Class constructor
            @param parent The parent instance class */
        Q_INVOKABLE explicit SetInputStringCacheValueModule(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~SetInputStringCacheValueModule() override;

    public:
        /** @see DefaultSequenceInstanceModule::initInputsFormat
            @note @ref inputValueToSetName : string (The string value to set) */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

    protected:
    /** @see AProjectCacheModule::actOnCache */
        virtual bool actOnCache(const JsonArray &jsonObjectInputValue,
                                const QString &projectCacheKey,
                                JsonArray &output) override;

    private:
        static const constexpr char *InputValueToSetName = "valueToSet";
        static const constexpr char *InputValueToSetDesc = "The string value to set";
};
