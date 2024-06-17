// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "abstractpluginelements/defaultsequenceinstancemodule.hpp"

class ICommonInterface;


/** @brief Abstract class to manipulate the project cache */
class AProjectCacheModule : public DefaultSequenceInstanceModule
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param moduleClassName The module class name
            @param parent The parent class instance */
        explicit AProjectCacheModule(const QString &moduleClassName,
                                     QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~AProjectCacheModule() override = default;

    public:
        /** @see DefaultSequenceInstanceModule::initInputsFormat
            @note @ref inputProjectCacheKeyName : string (The key of project cache to store the
                                                          selected file) */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

    protected slots:
        /** @see DefaultSequenceInstanceModule::processModule */
        virtual void processModule(const JsonArray &jsonObjectInputValues) override;

    protected:
        /** @brief Get common interface */
        ICommonInterface &getCommonItf() const { return _commonItf; }

        /** @brief Allow to act on project cache
            @param jsonObjectInputValue The module inputs
            @param projectCacheKey The key of the project cache value
            @param output The module output
            @return True if no problem occurred */
        virtual bool actOnCache(const JsonArray &jsonObjectInputValue,
                                const QString &projectCacheKey,
                                JsonArray &output) = 0;

    private:
        static const constexpr char *InputProjectCacheKeyName = "projectCacheKey";
        static const constexpr char *InputProjectCacheKeyDesc = "The key of project cache to use.";

    private:
        ICommonInterface &_commonItf;
};
