// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "src/aprojectcachemodule.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief Get a string value stored in the project cache, thanks to a key.
    @note If the given key doesn't exist in the project cache, a null string is returned. */
class GetStringCacheValueModule : public AProjectCacheModule
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(GetStringCacheValueModule)

    public:
        /** @brief Class constructor
            @param parent The parent instance class */
        Q_INVOKABLE explicit GetStringCacheValueModule(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~GetStringCacheValueModule() override;

    public:
    /** @see AProjectCacheModule::initOutputsFormat
        @note @ref outputCacheValueName : string (The value got from the project cache) */
    virtual bool initOutputsFormat(JsonArray &outputsFormat) const override;

    protected:
        /** @see AProjectCacheModule::actOnCache */
        virtual bool actOnCache(const JsonArray &jsonObjectInputValue,
                                const QString &projectCacheKey,
                                JsonArray &output) override;

    private:
        static const constexpr char *OutputCacheValueName = "cacheValue";
        static const constexpr char *OutputCacheValueDesc = "The value got from the project cache.";
};
