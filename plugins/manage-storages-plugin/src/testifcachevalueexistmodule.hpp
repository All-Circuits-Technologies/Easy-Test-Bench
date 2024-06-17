// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "src/aprojectcachemodule.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief Test if a value exist in the project cache (if not null) */
class TestIfCacheValueExistModule : public AProjectCacheModule
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(TestIfCacheValueExistModule)

    public:
        /** @brief Class constructor
            @param parent The parent instance class */
        Q_INVOKABLE explicit TestIfCacheValueExistModule(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~TestIfCacheValueExistModule() override;

    public:
        /** @see AProjectCacheModule::initOutputsFormat
            @note @ref outputCacheValueExistName : boolean (True if the value exist in the project
                                                            cache) */
        virtual bool initOutputsFormat(JsonArray &outputsFormat) const override;

    protected:
        /** @see AProjectCacheModule::actOnCache */
        virtual bool actOnCache(const JsonArray &jsonObjectInputValue,
                                const QString &projectCacheKey,
                                JsonArray &output) override;

    private:
        static const constexpr char *OutputCacheValueExistName = "cacheValueExist";
        static const constexpr char *OutputCacheValueExistDesc = "True if the value exist in the "
                                                                 "project cache";
};
