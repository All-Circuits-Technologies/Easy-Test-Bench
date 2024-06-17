// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QHash>
#include <QVector>

#include "core/knownsettingdetails.hpp"


/** @brief List the known settings contains in the INI file */
class KnownSettings : public QObject
{
    Q_OBJECT

    public:
        /** @brief The known settings contains in the INI file
            @attention When adding a new KnownSetting, don't forget to complete the methods:
                        @ref KnownSettings::castStringValueToVariant and
                        @ref KnownSettings::initKnownSettingsDetails */
        enum Enum
        {
            PluginPath,
            ProjectFileDefaultPath,
            LogAppFolderPath,
            LogBusinessFolderPath,
            LogFileNamePattern,
            LogConsoleCriticity,
            LogFileSavingCriticity,
            TranslateFilesFolderPath,
            DatabaseFilePath,
            YqProgramPath,
            YamlWorkerDirPath,
            UnknownSetting
        };

        Q_ENUM(Enum)

    public:
        /** @brief Deleted constructor */
        explicit KnownSettings() = delete;

    public:
        /** @brief Parse the enum from string
            @param strKnownSetting The known setting string value to parsed
            @return The value parsed or unknown */
        static Enum parseFromString(const QString &strKnownSetting);

        /** @brief Stringify the known setting given
            @param value The enum to stringify
            @return The known setting name stringified */
        static QString toString(Enum value);

        /** @brief Get all the available elements
            @note Unknown enum is included
            @return The list of all enum values */
        static QVector<Enum> getAvailableElements();

        /** @brief Get the details of the setting value given
            @note If no KnownSettingDetails has been found for the known setting, the method returns
                  a structure with all its elements equals to null.
            @param value The known setting
            @return The setting details */
        static const KnownSettingDetails &getSettingDetails(Enum value);

        /** @brief Cast the value stringified of a known setting, to the expected value type
            @note For instance, if the value of a known setting is a quint32, the method will
                  transform:
                        "3" to a (quint32) 3
            @param knownSetting The known setting to cast value
            @param strValue The value stringified to parse to variant
            @return The value parsed and transformed to the right type */
        static QVariant castStringValueToVariant(KnownSettings::Enum knownSetting,
                                                 const QString &strValue);

    private:
        /** @brief Add details to the known settings
            @see KnownSettingDetails
            @note The known setting KnownSettingDetails::name value is the knownSetting string
                  returned by the method KnownSettings::toString() and lowered.
                  For instance: KnownSettings::ThisIsAnExample becomes "thisisanexample"
            @note The known setting KnownSettingDetails::optionValueName value is the knownSetting
                  string returned by the method KnownSettings::toString(), lowered and surrounded by
                  the two characters: "<" and ">".
                  For instance: KnownSettings::ThisIsAnExample becomes "<thisisanexample>"
            @return The details of the known settings */
        static QHash<Enum, KnownSettingDetails> initKnownSettingsDetails();

    private:
        /** @note Because this static variable is initialized before the Q_ENUM, we have to init
                  the static in the first call when you need it (that's why it's not const) */
        static QHash<Enum, KnownSettingDetails> _knownSettingsDetails;
        static const KnownSettingDetails _defaultSettingDetail;
};
