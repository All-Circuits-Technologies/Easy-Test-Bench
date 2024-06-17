// SPDX-FileCopyrightText: 2019 - 2020 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QHash>
#include <QVariant>

#include "core/knownsettings.hpp"
#include "testbedcore/testbedglobal.hpp"

class QReadWriteLock;
class QSettings;


/** @brief This manager helps to read and write settings from and to an INI file */
class TESTBEDLIB_DLLSPEC SettingsManager : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param parent The class parent */
        explicit SettingsManager(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~SettingsManager() override;

    public:
        /** @brief Load the settings from the INI file
            @note If a setting is not contained in the INI file, the settings manager will take the
                  default value in constants files.
            @note If the load method has already been called and has succeeded, this method will
                  return true do nothing.
            @return True if no problem occurs */
        bool load();

        /** @brief Clear all the elements and allow to load again the settings via the
                    @ref SettingsManager::load method */
        void clear();

        /** @brief Read the wanted setting
            @note This method is thread safe, and prevents to read and write at the same time, but
                  allow multiple reading at same time.
            @param setting The setting element to read
            @param value The value read
            @return True if no problem occurs */
        bool read(KnownSettings::Enum setting, QVariant &value) const;

        /** @brief Read the wanted setting and return a string
            @note This method is thread safe, and prevents to read and write at the same time, but
                  allow multiple reading at same time.
            @note If the element can't parsed to string, it returns a null QString
            @param setting The wanted setting
            @param value The string representation of the setting wanted
            @return True if no problem occurs */
        bool read(KnownSettings::Enum setting, QString &value) const;

        /** @brief Write the setting
            @note This method is thread safe, and prevents to read and write at the same time
            @note By default, the setting value is also written to the QSetting file, to prevent
                  that see param writeToIniFile
            @param setting The setting to write
            @param value The value to write
            @param writeToIniFile True to write also the value in QSetting file
            @return True if no problem occurs */
        bool write(KnownSettings::Enum setting, const QVariant &value, bool writeToIniFile = true);

    private:
        /** @brief Parse the application command line known option and get values
            @param commandLineValues The values parsed from the command line options
            @return True if no problem occurs */
        bool parseCommandLineOptionsValues(QHash<KnownSettings::Enum, QVariant> &commandLineValues);

    private:
        QHash<KnownSettings::Enum, QVariant> _cache {};
        QSettings *_settingsFile{nullptr};
        QReadWriteLock *_readWriteLock{nullptr};
};
