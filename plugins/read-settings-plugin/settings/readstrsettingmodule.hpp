// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "settings/areadsettingmodule.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief The module reads a string setting from the test bed lib, thanks to the key given.
    @note The key has to be known by the test bed lib or an error is raised. */
class ReadStrSettingModule : public AReadSettingModule
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(ReadStrSettingModule)

    public:
        /** @brief Class constructor
            @param parent The parent instance class */
        Q_INVOKABLE explicit ReadStrSettingModule(QObject *parent = nullptr);

    protected:
        /** @see AReadSettingModule::initOutputsFormat
            @note @ref outputSettingValueName : string (The setting output value) */
        virtual bool initOutputsFormat(JsonArray &outputsFormat) const override;

        /** @see AReadSettingModule::formatOutputWithReadValue */
        virtual bool formatOutputWithReadValue(const QVariant &value, JsonArray &output) override;
};
