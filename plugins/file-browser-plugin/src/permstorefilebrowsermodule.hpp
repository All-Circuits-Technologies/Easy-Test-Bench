// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "src/afilebrowsermodule.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief Useful module to browse file and store the value got in the permanent storage
    @note The module memorizes in permanent storage the path chosen, and will reload it at start. */
class PermStoreFileBrowserModule : public AFileBrowserModule
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(PermStoreFileBrowserModule)

    public:
        /** @brief Class constructor
            @param parent The parent class instance */
        Q_INVOKABLE explicit PermStoreFileBrowserModule(QObject *parent = nullptr);

    public:
        /** @see AUpdateHexFile::initInputsFormat
            @note @ref inputPermStorageKeyName : string (The key of permanent storage to store the
                                                         selected file) */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

    protected:
        /** @see FileBrowerModule::getDefaultValue */
        virtual bool getDefaultValue(const JsonArray &jsonObjectInputValues,
                                     QString &defaultValue) override;

        /** @see FileBrowerModule::saveChosenValue */
        virtual bool saveChosenValue(const QString &value) override;

    private:
        static const constexpr char *InputPermStorageKeyName = "permStorageKey";
        static const constexpr char *InputPermStorageKeyDesc = "The key of permanent storage to "
                                                               "store the selected file";

    private:
        QString _permanentKey;
};
