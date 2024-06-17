// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "src/afilebrowsermodule.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief Useful module to browse file and store the value got in the project cache.
    @note The module memorizes in project cache the path chosen, and will reload it at start.
    @note The project cache is only alive until the user closes the current project or close the
          apps. */
class ProjectCacheBrowserModule : public AFileBrowserModule
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(ProjectCacheBrowserModule)

    public:
        /** @brief Class constructor
            @param parent The parent class instance */
        Q_INVOKABLE explicit ProjectCacheBrowserModule(QObject *parent = nullptr);

    public:
        /** @see AUpdateHexFile::initInputsFormat
            @note @ref inputProjectCacheKeyName : string (The key of project cache to store the
                                                          selected file) */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

    protected:
        /** @see FileBrowerModule::getDefaultValue */
        virtual bool getDefaultValue(const JsonArray &jsonObjectInputValues,
                                     QString &defaultValue) override;

        /** @see FileBrowerModule::saveChosenValue */
        virtual bool saveChosenValue(const QString &value) override;

    private:
        static const constexpr char *InputProjectCacheKeyName = "projectCacheKey";
        static const constexpr char *InputProjectCacheKeyDesc = "The key of project cache to "
                                                                "store the selected file";

    private:
        QString _projectCacheKey;
};
