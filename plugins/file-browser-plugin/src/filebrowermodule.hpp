// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "src/afilebrowsermodule.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief Simple module to browse file or folder (without getting or saving anything) */
class FileBrowerModule : public AFileBrowserModule
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(FileBrowerModule)

    public:
        /** @brief Class constructor
            @param parent The parent class instance */
        Q_INVOKABLE explicit FileBrowerModule(QObject *parent = nullptr);

    protected:
        /** @see FileBrowerModule::getDefaultValue */
        virtual bool getDefaultValue(const JsonArray &jsonObjectInputValues,
                                     QString &defaultValue) override;

        /** @see FileBrowerModule::saveChosenValue */
        virtual bool saveChosenValue(const QString &value) override;
};
